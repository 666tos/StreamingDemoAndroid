//
//  InputStreamDataProvider.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef InputStreamDataProvider_hpp
#define InputStreamDataProvider_hpp

#include <stdio.h>
#include <memory>
#include "RawData.hpp"

struct AVIOContext;

namespace StreamingEngine {
    class IInputStreamDataProviderDelegate {
    public:
        virtual void getNextData() = 0;
    };
    
    class InputStreamDataProvider {
    private:
        const int bufferSize_ = 1 << 14;
        uint8_t *buffer_;

        int64_t offset_ = 0;
        RawDataRef rawData_;

        AVIOContext *ioContext_;
        
        static int read(void *opaque, uint8_t *buffer, int size);
        int read(uint8_t *buffer, int size);
        
        static int64_t seek(void *opaque, int64_t pos, int whence);
        int64_t seek(int64_t pos, int whence);
    public:
        IInputStreamDataProviderDelegate *delegate_;
        
        InputStreamDataProvider(IInputStreamDataProviderDelegate *delegate);
        ~InputStreamDataProvider();
        
        void setData(RawDataRef rawData);
        
        AVIOContext* ioContext();
    };
}

#endif /* InputStreamDataProvider_hpp */
