/*
 * Thread.cpp
 *
 *  Created on: Mar 1, 2013
 *      Author: zerksud
 */

#include "Thread.h"

#include "ZLogger.h"

#undef LOGV
#define LOGV(...)   ((void)0)

namespace ann {
namespace util {

void* runMainLoop(void* param) {
    Thread* queue = static_cast<Thread*>(param);
    queue->mainLoop();

    return nullptr;
}

Thread::Thread() {
    mThread = 0;    //TODO: get rid of such initialization
    mPendingTasks = new Tasks();
    mRunningTasks = new Tasks();

    pthread_mutex_init(&mSharedDataMutex, nullptr);
    pthread_cond_init(&mPendingQueueUpdatedCond, nullptr);
    pthread_cond_init(&mThreadStoppedCond, nullptr);

    mIsRunning = false;
    mShouldStop = false;
}

Thread::~Thread() {
    LOGV("Thread - destructor");
    pthread_mutex_lock(&mSharedDataMutex);
    if (mIsRunning) {
        mShouldStop = true;
        //"update" pending queue to unblock running thread if it waits for update
        pthread_cond_signal(&mPendingQueueUpdatedCond);

        LOGV("Thread - stop thread");
        while (mIsRunning) {
            LOGV("Thread - waiting for thread to stop");
            pthread_cond_wait(&mThreadStoppedCond, &mSharedDataMutex);
        }
        pthread_mutex_unlock(&mSharedDataMutex);
        LOGV("Thread - thread stopped");
    }

    pthread_cond_destroy(&mPendingQueueUpdatedCond);
    pthread_cond_destroy(&mThreadStoppedCond);
    pthread_mutex_destroy(&mSharedDataMutex);

    deleteTasks(mPendingTasks);
    deleteTasks(mRunningTasks);
    LOGV("Thread - destructor done");
}

void Thread::deleteTasks(Tasks* tasks) {
    while (!tasks->empty()) {
        RecurringTaskBox* task = tasks->front();
        tasks->pop();

        delete task;
    }
    delete tasks;
}

void Thread::addTask(Task task) {
    RecurringTaskBox* taskBox = new RecurringTaskBox([task](){
        task();
        return false;
    });
    addTaskEx(taskBox);
}

void Thread::addRecurringTask(RecurringTask task) {
    RecurringTaskBox* taskBox = new RecurringTaskBox(task);
    addTaskEx(taskBox);
}

void Thread::addTaskEx(RecurringTaskBox *taskBox) {
    LOGV("Thread - add new task");
    pthread_mutex_lock(&mSharedDataMutex);

    mPendingTasks->push(taskBox);

    pthread_cond_signal(&mPendingQueueUpdatedCond);
    pthread_mutex_unlock(&mSharedDataMutex);
}

bool Thread::run() {
    pthread_mutex_lock(&mSharedDataMutex);
    if (mIsRunning) {
        pthread_mutex_unlock(&mSharedDataMutex);
        LOGE("TaskQueue - can't run already running thread");
        return false;
    }
    mIsRunning = true;
    mShouldStop = false;
    pthread_mutex_unlock(&mSharedDataMutex);

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_create(&mThread, &attr, runMainLoop, static_cast<void*>(this));
    pthread_attr_destroy(&attr);

    return true;
}

void Thread::planToStop() {
    LOGV("Thread - planToStopThread");
    pthread_mutex_lock(&mSharedDataMutex);
    mShouldStop = true;
    //"update" pending queue to unblock running thread if it waits for update
    pthread_cond_signal(&mPendingQueueUpdatedCond);
    pthread_mutex_unlock(&mSharedDataMutex);
}

void Thread::mainLoop() {
    while (mIsRunning) {
        pthread_mutex_lock(&mSharedDataMutex);
        if (mRunningTasks->empty()) {
            LOGV("Thread - running tasks queue is empty");
            while (mPendingTasks->empty() && !mShouldStop) {
                LOGV("Thread - waiting for new pending tasks");
                pthread_cond_wait(&mPendingQueueUpdatedCond, &mSharedDataMutex);
            }
        }

        if (mShouldStop) {
            LOGV("Thread - should stop");
            mIsRunning = false;
            pthread_cond_signal(&mThreadStoppedCond);
            pthread_mutex_unlock(&mSharedDataMutex);
            return;
        } else if (mRunningTasks->empty()) {
            LOGV("Thread - got new pending tasks");
            std::swap(mPendingTasks, mRunningTasks);
        }
        pthread_mutex_unlock(&mSharedDataMutex);

        LOGV("Thread - next iteration");
        if (!mRunningTasks->empty()) {
            LOGV("Thread - next task");
            RecurringTaskBox* taskBox = mRunningTasks->front();
            mRunningTasks->pop();

            bool repeat = taskBox->task();

            if (repeat) {
                addTaskEx(taskBox);
            } else {
                delete taskBox;
            }
            LOGV("Thread - task done");
        }
    }
}

} /* namespace util */
} /* namespace ann */
