//
//  Log.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/27/18.
//

#include "Log.hpp"
#include <iostream>

extern "C" {
    
#ifdef __ANDROID__
#include <android/log.h>
#endif
    
#include "libavutil/log.h"

static void logCallback(void *ptr, int level, const char *fmt, va_list vargs);
}

using namespace StreamingEngine::Util;

Log::Severity Log::logLevel_ = Log::Severity::Verbose;

void Log::setLogLevel(Severity logLevel) {
    logLevel_ = logLevel;

    av_log_set_level(ffmpegLogLevel());
    av_log_set_callback(logCallback);
}

Log::Log(Severity severity):
    severity_(severity) {
}

Log::~Log() {
    flush();
}

int Log::ffmpegLogLevel() {
    switch (logLevel_) {
        case Log::Severity::Silent:     return AV_LOG_QUIET;
        case Log::Severity::Error:      return AV_LOG_ERROR;
        case Log::Severity::Warning:    return AV_LOG_WARNING;
        case Log::Severity::Info:       return AV_LOG_INFO;
        case Log::Severity::Verbose:    return AV_LOG_VERBOSE;
        case Log::Severity::Debugger:   return AV_LOG_DEBUG;
    }
}

Log::Severity Log::logLevelFromFFMpeg(int ffmpegLogLevel) {
    switch (ffmpegLogLevel) {
        case AV_LOG_QUIET:      return Log::Severity::Silent;
        case AV_LOG_ERROR:      return Log::Severity::Error;
        case AV_LOG_WARNING:    return Log::Severity::Warning;
        case AV_LOG_INFO:       return Log::Severity::Info;
        case AV_LOG_VERBOSE:    return Log::Severity::Verbose;
        case AV_LOG_DEBUG:      return Log::Severity::Debugger;
    }

    return Log::Severity::Silent;
}

void Log::flush() {
    outputLog(stream_.str());
}

std::string Log::severityString() {
    switch (severity_) {
        case Log::Severity::Silent:     return "";
        case Log::Severity::Error:      return "[Error] ";
        case Log::Severity::Warning:    return "[Warning] ";
        case Log::Severity::Info:       return "[Info] ";
        case Log::Severity::Verbose:    return "";
        case Log::Severity::Debugger:   return "";
    }
}

void Log::outputLog(const std::string& message) {
    if (severity_ > logLevel_) {
        return;
    }

    auto severity = severityString();

    //=== actually output

#ifdef __ANDROID__
    __android_log_print(androidLogLevel(), "TacxStreaming ", "%s%s", severity.c_str(), message.c_str());
#else
    std::clog << "TacxStreaming " << severity << message << std::endl;
#endif
}

#ifdef __ANDROID__
int Log::androidLogLevel() {
    switch (severity_) {
        case Log::Severity::Silent:     return ANDROID_LOG_SILENT;
        case Log::Severity::Error:      return ANDROID_LOG_ERROR;
        case Log::Severity::Warning:    return ANDROID_LOG_WARN;
        case Log::Severity::Info:       return ANDROID_LOG_INFO;
        case Log::Severity::Verbose:    return ANDROID_LOG_VERBOSE;
        case Log::Severity::Debugger:   return ANDROID_LOG_DEBUG;
    }
}

#endif

extern "C" {
static void logCallback(void *ptr, int level, const char *fmt, va_list vargs) {
    char *stringBuffer = nullptr;
    vasprintf(&stringBuffer, fmt, vargs);
    Log(Log::logLevelFromFFMpeg(level)) << "[FFMpeg] " << stringBuffer;
    free(stringBuffer);
}
}
