//
//  Playlist.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/8/19.
//

#ifndef Playlist_hpp
#define Playlist_hpp

#include "Timestamp.hpp"
#include <string>
#include <vector>

namespace StreamingEngine {
    class Playlist {
    public:
        class Item;
        class XKey;
        
        using ItemRef = std::shared_ptr<Item>;
        using XKeyRef = std::shared_ptr<XKey>;
        
        Playlist(const std::vector<ItemRef> &parts);
        ~Playlist();
        
        ItemRef findNext(ItemRef item);
        ItemRef findItem(int64_t tag);
        ItemRef item(size_t index);
        size_t itemIndexForTimestamp(const Timestamp& timestamp);
        size_t numberOfParts();
        
    private:
        std::vector<ItemRef> parts_;
        void alignParts();
    };
}

#endif /* Playlist_hpp */
