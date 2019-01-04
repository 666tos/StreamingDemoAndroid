//
//  Log.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/27/18.
//

#ifndef Log_hpp
#define Log_hpp

#include <sstream>
#include <string>

namespace StreamingEngine {
    namespace Util {
        
        /**
         * Class that is used to write messages to debug console
         */
        class Log {
        public:
            enum class Severity {
                Silent = 0,
                Error,
                Warning,
                Info,
                Verbose,
                Debugger,
            };

            /**
             * Sets minimal severity that's displayed
             */
            static void setLogLevel(Severity logLevel);

            /**
             *  Default c-tor
             *
             * @param severity specifies message severity, between Severity::DEBUG and Severity::ERROR
             */
            Log(Severity severity = Severity::Info);

            /**
             * Flushes output
             */
            void flush();

            /**
             * D-tor. Performs implicit flush.
             */
            ~Log();

            /**
             * Overloading operator<<
             */
            template<typename T>
            Log &operator<<(T value) {
                if (severity_ <= logLevel_) {
                    stream_ << value;
                }

                return *this;
            }

        private:
            static Severity logLevel_;

            std::ostringstream stream_;
            Severity severity_;

            /**
             * Returns string describing severity level
             */
            std::string severityString();

            /**
             * Outputs given message if @severity is higher than @logLevel_
             */
            void outputLog(const std::string &message);

        #ifdef __ANDROID__
            int androidLogLevel();
        #endif
        };
    }
}
#endif /* Log_hpp */
