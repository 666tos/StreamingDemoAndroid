//
//  Log.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/27/18.
//

#include "Log.hpp"
#include <iostream>

#ifdef __ANDROID__
extern "C" {
#include <android/log.h>
}
#endif

using namespace StreamingEngine::Util;

Log::Severity Log::logLevel_ = Log::Severity::Verbose;

void Log::setLogLevel(Severity logLevel) {
    logLevel_ = logLevel;
}

Log::Log(Severity severity):
    severity_(severity) {
}

Log::~Log() {
    flush();
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