//
//  WorkerTask.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/17/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#ifndef WorkerTask_hpp
#define WorkerTask_hpp

#include <RawData.hpp>

namespace StreamingEngine {
    class Info;
    
    class WorkerTask {
    private:
        int64_t index_;
        RawDataRef rawData_;
        
    public:
        WorkerTask(int64_t index, RawDataRef rawData);
        ~WorkerTask();
        
        int64_t index();
        RawDataRef rawData();
    };
    
    using WorkerTaskRef = std::shared_ptr<WorkerTask>;
}

#endif /* WorkerTask_hpp */
