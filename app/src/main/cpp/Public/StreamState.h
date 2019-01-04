//
//  StreamState.h
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef StreamState_h
#define StreamState_h

typedef enum {
    StreamStateUnknown = 0,
    StreamStateStopped,
    StreamStateBuffering,
    StreamStateReady,
    StreamStateError
} StreamState;

#endif /* StreamState_h */
