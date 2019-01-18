//
// Created by M Ivaniushchenko on 1/18/19.
//

#ifndef STREAMINGDEMOANDROID_JNIPOOL_H
#define STREAMINGDEMOANDROID_JNIPOOL_H

#include <mutex>
#include <vector>

namespace Core {
    /**
     * ObjectPool is used to store specific set of types associated with handles.
     *
     * This is used to bind java objects to native objects. Each underlying native object is
     * first registered in object pool, and its handle is obtained. Java objects hold a
     * long int var with this handle. Whenever native object is accessed via JNI call, a real object
     * is requested from ObjectPool passing in the handle.
     */
    template<typename ItemType, typename Deleter = std::default_delete<ItemType>>
    class JNIObjectPool {
    public:
        typedef int HandleType;

        JNIObjectPool() {}

        JNIObjectPool(const JNIObjectPool &) = delete;

        JNIObjectPool &operator=(const JNIObjectPool &) = delete;

        ~JNIObjectPool() {}

        /**
         * Adds an object instance to the pool.
         *
         * @return handle of the newly added object
         */
        HandleType add(ItemType *ptr) {
            // TODO: use MutexMacro
            std::lock_guard<std::mutex> lockGuard(mutex_);

            auto itr = std::find_if(pool_.begin(), pool_.end(),
                                    [](std::unique_ptr<ItemType, Deleter> &ptr) {
                                        return ptr == nullptr;
                                    });

            if (itr != pool_.end()) {
                itr->reset(ptr);
            } else {
                pool_.push_back(std::unique_ptr<ItemType, Deleter>(ptr));
                itr = --pool_.end();
            }

            return static_cast<HandleType>(itr - pool_.begin());
        }

        /**
         * Gets an object instance from the pool.
         *
         * @param handle object handle
         */
        ItemType *get(HandleType handle) {
            // TODO: use MutexMacro
            std::lock_guard<std::mutex> lockGuard(mutex_);

            if (handle < 0 || handle >= static_cast<HandleType>(pool_.size())) {
                return nullptr;
            }

            return pool_[handle].get();
        }

        /**
         * Removes object from pool.
         *
         * Object is destroyed.
         */
        void remove(HandleType handle) {
            // TODO: use MutexMacro
            std::lock_guard<std::mutex> lockGuard(mutex_);

            if (handle < 0 || handle >= static_cast<HandleType>(pool_.size())) {
                return;
            }

            pool_[handle] = nullptr;
        }

    private:
        std::vector<std::unique_ptr<ItemType, Deleter>> pool_;
        std::mutex mutex_;
    };
}

#endif //STREAMINGDEMOANDROID_JNIPOOL_H
