//
//  ITSPartWorkerDelegate.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef ITSPartWorkerDelegate_hpp
#define ITSPartWorkerDelegate_hpp

#include "PlaylistItem.hpp"

namespace StreamingEngine {
    class ITSPartWorkerDelegate {
    public:
        virtual Playlist::ItemRef getPart() = 0;
        virtual Playlist::ItemRef nextPart(Playlist::ItemRef part) = 0;
    };
}

#endif /* ITSPartWorkerDelegate_hpp */
