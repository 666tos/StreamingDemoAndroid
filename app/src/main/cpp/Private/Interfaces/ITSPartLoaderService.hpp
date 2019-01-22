//
//  ITSPartLoaderService.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef ITSPartLoaderService_hpp
#define ITSPartLoaderService_hpp

#include "PlaylistItem.hpp"

namespace StreamingEngine {
    class ITSPartLoaderService {
    public:
        virtual ~ITSPartLoaderService() {};
        virtual void load(Playlist::ItemRef item) final {
            item->markAsLoading();
            load(item->url(), item->tag());
        }

    protected:
        virtual void load(const std::string& url, int64_t tag) = 0;
    };
}

#endif /* ITSPartLoaderService_hpp */
