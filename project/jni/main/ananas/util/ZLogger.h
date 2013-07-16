/*
 * ZLogger.h
 *
 *  Created on: Feb 24, 2013
 *      Author: zerksud
 */

#ifndef ZLOGGER_H_
#define ZLOGGER_H_

#include <android/log.h>

#define LOGTAG(TAG)         (ann::util::ZLogger::getInstance().setGlobalTag(TAG))
#define LOGFORMAT(FORMAT)   (ann::util::ZLogger::getInstance().setLogFormat(FORMAT))

#define LOGV(...)   (ann::util::ZLogger::getInstance().print(ANDROID_LOG_VERBOSE, __FILE__, __LINE__, __func__, __VA_ARGS__))
#define LOGI(...)   (ann::util::ZLogger::getInstance().print(ANDROID_LOG_INFO, __FILE__, __LINE__, __func__, __VA_ARGS__))
#define LOGE(...)   (ann::util::ZLogger::getInstance().print(ANDROID_LOG_ERROR, __FILE__, __LINE__, __func__, __VA_ARGS__))
#define LOGD(...)   (ann::util::ZLogger::getInstance().print(ANDROID_LOG_DEBUG, __FILE__, __LINE__, __func__, __VA_ARGS__))

namespace ann {
namespace util {

class ZLogger {
public:
    static ZLogger& getInstance() { //TODO: move impl to cpp
        static ZLogger instance;
        return instance;
    }

    void setTagPrefix(const char* tag);
    void setTagFormat(const char* format);
    void print(int priority, const char* fileName, int lineNum, const char* funcName, const char* format, ...) const;

private:
    ZLogger();
    ZLogger(ZLogger& other);
    ZLogger& operator=(ZLogger const& other);
    virtual ~ZLogger();

    const char* mTagPrefix;
    const char* mTagFormat;
};

} /* namespace util */
} /* namespace ann */
#endif /* ZLOGGER_H_ */
