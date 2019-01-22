//
//  IDecryptorKeyLoaderService.hpp
//  Pods
//
//  Created by M Ivaniushchenko on 1/11/19.
//

#ifndef IDecryptorKeyLoaderService_hpp
#define IDecryptorKeyLoaderService_hpp

#include <set>
#include "Log.hpp"

namespace StreamingEngine {
    class IDecryptorKeyLoaderService {
    public:
        virtual ~IDecryptorKeyLoaderService() {};
        virtual void load(const std::string &url) final {
            if (loadingURLs_.find(url) != loadingURLs_.end()) {
                Util::Log(Util::Log::Severity::Debugger) << "[DecryptorKeyLoaderService] Already downloading key: " << url;
                return;
            }

            Util::Log(Util::Log::Severity::Info) << "[DecryptorKeyLoaderService] Downloading key: " << url;
            loadingURLs_.insert(url);
            performLoad(url);
        }
        
    protected:
        virtual void performLoad(const std::string &url) = 0;
        
    private:
        std::set<std::string> loadingURLs_;
    };
}

#endif /* IDecryptorKeyLoaderService_hpp */
