//
//  Config.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/27/18.
//

#include "Config.hpp"

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
    return new Config(2000000, 180, 2, 2, Util::Log::Severity::Verbose);
}
