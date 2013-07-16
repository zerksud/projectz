/*
 * Thread.h
 *
 *  Created on: Mar 1, 2013
 *      Author: zerksud
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

#include <functional>
#include <queue>

namespace ann {
namespace util {

typedef std::function<void ()> Task;
typedef std::function<bool ()> RecurringTask;

class Thread {
public:
    Thread();
    virtual ~Thread();

    void addTask(Task task);
    void addRecurringTask(RecurringTask task);
    bool run();
    void planToStop();

    friend void* runMainLoop(void* param);

private:
    struct RecurringTaskBox {
        RecurringTask task;

        RecurringTaskBox(RecurringTask taskPtr) {
            task = taskPtr;
        }
    };

    typedef std::queue<RecurringTaskBox*> Tasks;

    void addTaskEx(RecurringTaskBox *taskBox);
    void mainLoop();
    void stopThread();
    static void deleteTasks(Tasks* tasks);  //TODO: change to friend helper function?

    bool mIsRunning;
    bool mShouldStop;
    pthread_t mThread;
    pthread_mutex_t mSharedDataMutex;
    pthread_cond_t mPendingQueueUpdatedCond;
    pthread_cond_t mThreadStoppedCond;

    Tasks* mPendingTasks;
    Tasks* mRunningTasks;
};

} /* namespace util */
} /* namespace ann */
#endif /* THREAD_H_ */
