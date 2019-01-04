//
//  FrameReader.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/18/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef FrameReader_hpp
#define FrameReader_hpp

struct AVPacket;
struct AVFrame;

namespace StreamingEngine {
    
    class IStreamFrameAccessor;
    
    namespace Decode {
        class Info;
        class IWorkerDelegate;
        
        class FrameReader {
        private:
            IStreamFrameAccessor *streamFrameAccessor_;
            IWorkerDelegate *delegate_;
            
            bool shouldHandlePacket(const AVPacket *packet, Info *decodeInfo);
            
        public:
            FrameReader(IStreamFrameAccessor *streamFrameAccessor, IWorkerDelegate *delegate);
            ~FrameReader();

            AVFrame *readFrame(Info *decodeInfo);
        };
    }
}

#endif /* FrameReader_hpp */
