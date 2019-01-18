//
//  Utils.cpp
//  TacxStreaming
//
//  Created by M Ivaniushchenko on 1/3/19.
//

#include "Utils.hpp"

#include <unistd.h>
#include <math.h>

#ifdef __IOS_HARDWARE_DECODING__
#include <mach/mach.h>
#endif

using namespace StreamingEngine;

size_t Utils::ceilToMultiple(size_t value, size_t multiple) {
    const double ratio = double(value) / double(multiple);
    const auto iratio = ceil(ratio);
    return iratio * multiple;
}

bool Utils::approximatelyEqual(double a, double b, double epsilon) {
    return fabs(a - b) < epsilon;
}

#ifdef __IOS_HARDWARE_DECODING__

size_t Utils::ceilToPageSize(size_t value) {
    size_t pageSize = getpagesize();
    size_t allocationSize = ceilToMultiple(value, pageSize);
    return allocationSize;
}

size_t Utils::allocateVirtualMemory(void **data, size_t size) {
    /* required byte count, rounded up to next multiple of page size */
    size_t allocationSize = ceilToPageSize(size);
    
    kern_return_t result = vm_allocate((vm_map_t) mach_task_self(), (vm_address_t*)data, allocationSize, VM_FLAGS_ANYWHERE);
    
    if (result != KERN_SUCCESS) {
        *data = nullptr;
        return 0;
    }
    
    return allocationSize;
}

void Utils::deallocateVirtualMemory(void *data, size_t size) {
    vm_deallocate(mach_task_self(), (vm_address_t)data, size);
}

#endif
