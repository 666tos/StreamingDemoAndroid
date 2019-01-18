//
//  Decryptor.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/9/19.
//

#include "Decryptor.hpp"
#include "Log.hpp"

extern "C" {
#include "libavutil/aes.h"
}

using namespace std;
using namespace StreamingEngine;

Decryptor::Decryptor(DecryptorKeyStorage *storage, IDecryptorKeyLoaderService *loaderService):
    storage_(storage),
    loaderService_(loaderService){
    
}

Decryptor::~Decryptor() {
    
}

RawDataRef Decryptor::decrypt(RawDataRef encryptedData, Playlist::ItemRef item) {
    auto ivData = item->xKey()->ivData();
    
    if ((ivData == nullptr) || (ivData->getSize() != 16 /*16 bytes = 128 bits*/)) {
        Util::Log(Util::Log::Severity::Error) << "[Decryptor] No IV data for part: " << item->tag() << " decryption not possible";
        return nullptr;
    }
    
    auto keyURI = item->xKey()->uri();
    
    if (keyURI.length() == 0) {
        Util::Log(Util::Log::Severity::Error) << "[Decryptor] No key URL, for part: " << item->tag() << " decryption not possible";
        return nullptr;
    }
    
    auto decryptionKey = storage_->key(keyURI);
    
    if ((decryptionKey == nullptr) || (decryptionKey->getSize() != 16 /*16 bytes = 128 bits*/)) {
        Util::Log(Util::Log::Severity::Info) << "[Decryptor] No decryption key for part: " << item->tag();
        loaderService_->load(keyURI);
        return nullptr;
    }
    
    Util::Log(Util::Log::Severity::Debugger) << "[Decryptor] Decryption key found, decrypting part: " << item->tag();
    return performDecrypt(encryptedData, ivData, decryptionKey);
}

RawDataRef Decryptor::performDecrypt(RawDataRef encryptedData, RawDataRef iv, RawDataRef key) {
    size_t blocks = encryptedData->getSize() >> 4; // trailing bytes are not encrypted
    
    AVAES *aes_decrypt = av_aes_alloc();
    av_aes_init(aes_decrypt, key->getData(), 128, 1);
    av_aes_crypt(aes_decrypt, encryptedData->getData(), encryptedData->getData(), (int)blocks, iv->getData(), 1);
    
    return encryptedData;
}
