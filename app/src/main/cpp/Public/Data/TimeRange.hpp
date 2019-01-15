//
//  TimeRange.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/14/19.
//

#ifndef TimeRange_hpp
#define TimeRange_hpp

#include "Timestamp.hpp"

namespace StreamingEngine {
    struct TimeRange {
    public:
        TimeRange(const Timestamp& start, double duration): start_(start), duration_(duration) {}
        TimeRange(const TimeRange&) = default;
        
        double duration() const { return duration_; }
        Timestamp end() const { return start_ + duration_; }
        
        void setStart(const Timestamp &start) { start_ = start; }
        void setDuration(double duration) { duration_ = duration; }
        
        bool contains(const Timestamp &timestamp) const {
            return (timestamp > start_) && (timestamp < end());
        }
        
    private:
        Timestamp start_;
        double duration_;
    };
}

#endif /* TimeRange_hpp */
