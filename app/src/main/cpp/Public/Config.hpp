//
//  Config.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/27/18.
//

#ifndef Config_hpp
#define Config_hpp

#include <stdio.h>
#include "Log.hpp"

#if defined(__APPLE__)

#define __IOS_HARDWARE_DECODING__

#elif defined(__ANDROID__)

#define __ANDROID_HARDWARE_DECODING__

#endif

namespace StreamingEngine {
    class Config {
    public:
        /**
         * Sets minimal bitrate of stream picked from master playlist.
         * If target bitrate not reached, stream with highest bitrate is selected
         */
        const int64_t targetBitrate_;
        
        /**
         * Sets maximal number of cached frames stored. If playback is at 1x speed (30fps),
         * framebuffer size of 90 means 90/30 = 3s of playback being cached.
         */
        const int64_t framebufferSize_;
        
        /**
         * Sets maximal number of video parts downloaded in advance. If playback is at 1x speed (30fps),
         * - 0 means donwloading only part which is currently playing, causing lag on start of each part.
         * - N means downloading currently playing part and next N parts.
         * New download only starts when previous is finished, one-by-one, not in parallel.
         */
        const int64_t advanceDownloadStep_;
        
        /**
         * Sets delta of frame timestamp comparison, if frame in the middle of the stream was not decoded.
         * - 0 means only matching frame timestamp precisely (using frameTimestampDelta_).
         * - N means trying to match frames in range [X - frameTimestampDelta_ ... X + frameTimestampTolerance_], where X is currently requested timestamp.
         */
        const double frameTimestampTolerance_;
        
        /**
         * Sets delta of frame timestamp of comparison.
         * - 0 means only matching frame timestamp precisely, and should never be used since floats are hard to be equal.
         * - N means trying to match frames in range [X -frameTimestampDelta_ ... X + frameTimestampDelta_], where X is currently requested timestamp.
         */
        const double frameTimestampDelta_;
        
        /**
         * Sets minimal log severity that's displayed
         */
        const Util::Log::Severity logLevel_;
        
        static const Config &defaultConfig();
        
    private:
        Config(int64_t targetBitrate, int64_t framebufferSize, int64_t advanceDownloadStep,
               double frameTimestampTolerance, double frameTimestampDelta, Util::Log::Severity logLevel);
    };
}

#endif /* Config_hpp */
