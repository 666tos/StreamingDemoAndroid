//
//  Frame.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/3/19.
//

#ifndef Frame_hpp
#define Frame_hpp

#include <memory>
#include "Config.hpp"

struct AVFrame;

namespace StreamingEngine {
    class RawData;
    
    class Frame {
    public:
        enum class Plane {
        #ifdef __IOS_HARDWARE_DECODING__
            Y = 0, UV,
        #else
            Y = 0, U, V,
        #endif
            Count
        };
        
        Frame(AVFrame* frame, int64_t index);
        ~Frame();
        
        void setFrame(AVFrame* frame, int64_t index);
        int64_t getIndex();
        
        int getWidth();
        int getHeight();
        
        RawData* getPlane(Plane plane);
        
    private:
        RawData *planes_[static_cast<int>(Plane::Count)] = { nullptr };
        
        int64_t index_;
        int width_;
        int height_;
        
        void copyPlaneData(void *sourceData, int plane, size_t dataSize);
        
#ifdef __IOS_HARDWARE_DECODING__
        void copyFrameData420(AVFrame *frame);
#else
        void copyFrameData422(AVFrame *frame);
        void copyFrameData(AVFrame *frame, int plane, int height);
#endif
    };

    using FrameRef = std::shared_ptr<StreamingEngine::Frame>;
}

#endif /* Frame_hpp */
