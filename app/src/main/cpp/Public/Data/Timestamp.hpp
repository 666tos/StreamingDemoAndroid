//
//  Timestamp.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/14/19.
//

#ifndef Timestamp_hpp
#define Timestamp_hpp

#include <stdio.h>
#include "Config.hpp"
#include "Utils.hpp"

namespace StreamingEngine {
    struct Timestamp {
    public:
        Timestamp(double value, double delta = Config::defaultConfig().frameTimestampDelta_): value_(value), delta_(delta) {}
        Timestamp(const Timestamp&) = default;
        
        Timestamp operator + (double duration) const {
            return Timestamp(value_ + duration, delta_);
        }
      
        bool operator < (const Timestamp& ts) const {
            // Check if timestamp is smaller than others's minimum value
            return (value_ < ts.value_ - delta_);
        }
        
        bool operator > (const Timestamp& ts) const {
            // Check if timestamp is greater than others's maximum value
            return (value_ > ts.value_ + delta_);
        }
        
        bool operator == (const Timestamp& ts) const {
            return approximatelyEqual(*this, ts, delta_);
        }
        
        static bool approximatelyEqual(const Timestamp& lhs, const Timestamp& rhs, double delta) {
            return Utils::approximatelyEqual(lhs.value_, rhs.value_, lhs.delta_);
        }
        
        /**
         *  Overloaded operator for '<<' struct output like `Log(level) << timestamp`
         */
        friend std::ostream& operator << (std::ostream& ostream, const Timestamp& timestamp) {
            ostream << timestamp.value_;
            return ostream;
        }
        
        static const Timestamp zero;
        
    private:
        double value_;
        double delta_;
    };
}

#endif /* Timestamp_hpp */
