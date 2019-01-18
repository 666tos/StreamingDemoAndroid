//
//  Decryptor.hpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/9/19.
//

#ifndef Decryptor_hpp
#define Decryptor_hpp

#include "RawData.hpp"
#include "PlaylistItem.hpp"
#include "IDecryptorKeyLoaderService.hpp"
#include "DecryptorKeyStorage.hpp"
#include <stdio.h>

namespace StreamingEngine {
    class Decryptor {
    private:
        DecryptorKeyStorage *storage_;
        IDecryptorKeyLoaderService *loaderService_;
        
        RawDataRef performDecrypt(RawDataRef encryptedData, RawDataRef iv, RawDataRef key);
        
    public:
        Decryptor(DecryptorKeyStorage *storage, IDecryptorKeyLoaderService *loaderService);
        ~Decryptor();
        
        RawDataRef decrypt(RawDataRef encryptedData, Playlist::ItemRef item);
    };
}

#endif /* Decryptor_hpp */
