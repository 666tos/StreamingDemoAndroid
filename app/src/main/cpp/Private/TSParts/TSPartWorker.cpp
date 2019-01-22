//
//  TSPartWorker.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 12/14/18.
//  Copyright © 2018 Tacx. All rights reserved.
//

#include "TSPartWorker.hpp"
#include "PlaylistItem.hpp"
#include "ITSPartLoaderService.hpp"
#include "ITSPartWorkerDelegate.hpp"
#include "Log.hpp"
#include "Decryptor.hpp"
#include "MutexMacro.hpp"

#include <unistd.h>

using namespace std;
using namespace StreamingEngine;

TSPartWorker::TSPartWorker(Decode::Worker *decodeWorker, ITSPartLoaderService *tsPartLoader, ITSPartWorkerDelegate *delegate, Decryptor *decryptor, int64_t advanceDownloadStep):
    decodeWorker_(decodeWorker),
    tsPartLoader_(tsPartLoader),
    delegate_(delegate),
    decryptor_(decryptor),
    advanceDownloadStep_(advanceDownloadStep) {
        
    start();
}

TSPartWorker::~TSPartWorker() {
}

void TSPartWorker::setData(RawDataRef rawData, Playlist::ItemRef item) {
    auto task = make_shared<WorkerTask>(item->tag(), rawData);
    
    auto xKey = item->xKey();
    
    if (xKey != nullptr) {
        addDecryptTask(task);
        item->markAsLoaded();
    }
    else {
        decodeWorker_->addTask(task);
        item->markAsDecrypted();
    }
}

void TSPartWorker::findAndLoadPart() {
    auto part = delegate_->getPart();
    
    for (int i = 0; i <= advanceDownloadStep_; i++) {
        if (!part) {
            return;
        }
        
        switch (part->state()) {
            case Playlist::Item::State::Idle:
                Util::Log(Util::Log::Severity::Info) << "LOADING TSPART: " << part->tag();
                tsPartLoader_->load(part);
                return;
                
            case Playlist::Item::State::Loading:
                return;
                
            case Playlist::Item::State::Loaded: {
                auto decryptTask = getDecryptTask(part->tag());
                
                if (decryptTask != nullptr) {
                    performDecryption(decryptTask, part);
                }
                else {
                    Util::Log(Util::Log::Severity::Error) << "Decrypt task not found: " << part->tag();
                }
                break;
            }
                
            case Playlist::Item::State::Decrypted:
                break;
        }
        
        part = delegate_->nextPart(part);
    }
}

void TSPartWorker::addDecryptTask(WorkerTaskRef decryptTask) {
    synchronize_scope(mutex_);
    decryptTasks_[decryptTask->index()] = decryptTask;
}

WorkerTaskRef TSPartWorker::getDecryptTask(int64_t tag) {
    synchronize_scope(mutex_);
    auto it = decryptTasks_.find(tag);
    
    if (it != decryptTasks_.end()) {
        return it->second;
    }
    
    return nullptr;
}

void TSPartWorker::removeDecryptTask(int64_t tag) {
    synchronize_scope(mutex_);
    decryptTasks_.erase(tag);
}

void TSPartWorker::performDecryption(WorkerTaskRef decryptTask, Playlist::ItemRef item) {    
    auto decryptedData = decryptor_->decrypt(decryptTask->rawData(), item);
    
    if (decryptedData != nullptr) {
        Util::Log(Util::Log::Severity::Info) << "Decrypted: " << decryptTask->index();
        
        item->markAsDecrypted();
        removeDecryptTask(decryptTask->index());
        decodeWorker_->addTask(decryptTask);
    }
}

void TSPartWorker::run() {
    findAndLoadPart();
    this_thread::sleep_for(chrono::milliseconds(PROCESSING_INTERVAL));
}
