//
//  Config.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/27/18.
//

#include "Config.hpp"

using namespace StreamingEngine;
using namespace StreamingEngine::Util;

Config::Config(int64_t targetBitrate, int64_t framebufferSize, int64_t advanceDownloadStep,
               double frameTimestampTolerance, double frameTimestampDelta, Util::Log::Severity logLevel):
    targetBitrate_(targetBitrate),
    framebufferSize_(framebufferSize),
    advanceDownloadStep_(advanceDownloadStep),
    frameTimestampTolerance_(frameTimestampTolerance),
    frameTimestampDelta_(frameTimestampDelta),
    logLevel_(logLevel) {
    
}

const Config& Config::defaultConfig() {
    static Config instance(2000000, 180, 2,
                           3.0 * 1.0/30.0, 0.5 * 1.0/30.0,
                           Util::Log::Severity::Verbose);
    
    return instance;
}
