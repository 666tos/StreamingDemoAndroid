//
//  Config.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/27/18.
//

#include "Config.hpp"

extern "C" {
#include "libavutil/log.h"
}

using namespace StreamingEngine;
using namespace StreamingEngine::Util;

Config::Config(int64_t targetBitrate, int64_t framebufferSize, int64_t advanceDownloadStep, int64_t frameIndexTolerance, Util::Log::Severity logLevel):
    targetBitrate_(targetBitrate),
    framebufferSize_(framebufferSize),
    advanceDownloadStep_(advanceDownloadStep),
    frameIndexTolerance_(frameIndexTolerance),
    logLevel_(logLevel) {
    
}

Config* Config::defaultConfig() {
    return new Config(2000000, 180, 2, 2, Util::Log::Severity::Debugger);
}

int Config::ffmpegLogLevel() const {
    switch (logLevel_) {
        case Log::Severity::Silent:     return AV_LOG_QUIET;
        case Log::Severity::Error:      return AV_LOG_ERROR;
        case Log::Severity::Warning:    return AV_LOG_WARNING;
        case Log::Severity::Info:       return AV_LOG_INFO;
        case Log::Severity::Verbose:    return AV_LOG_VERBOSE;
        case Log::Severity::Debugger:   return AV_LOG_DEBUG;
    }
}
