//
//  MutexMacro.hpp
//  Pods
//
//  Created by M Ivaniushchenko on 1/11/19.
//

#ifndef MutexMacro_h
#define MutexMacro_h

#include <thread>
#include <mutex>

/*
 * This macro creates RAII-style lock (which uses given mutex),
 * locking it when before entering user's code and
 * which destroys at the end of scope unlocking mutex
 *
 
 class Foo {
    std::mutex mutex_;
    std::vector<int> values_;
 
    bool badFoo() {
        mutex_.lock();
 
        if (...SOMETHING IN VALUES...) {
            mutex_.unlock();    // <---- has to be unlocked before every function exit
            return true;
        }
 
        mutex_.unlock();    // <---- has to be unlocked before every function exit
        return false;
    }
 
     bool raiiFoo() {
        std::lock_guard<std::mutex> lockGuard(mutex_);  // <---- A bit longer version which requires unique lock name in case of more than 1 mutex

        if (...SOMETHING IN VALUES...) {
             return true;
         }
 
         return false;
     }
 
    bool goodFoo() {
        synchronize_scope(mutex_);
 
        if (...SOMETHING IN VALUES...) {
            return true;
        }

        return false;
    }
 
    void partialLock() {
        {
            synchronize_scope(mutex_);
            // Do something synchronized
        }
 
        // Do the rest
    }
*/

#define synchronize_scope(M) \
std::lock_guard<std::mutex> M##_lockGuard(M)

#endif /* MutexMacro_h */
