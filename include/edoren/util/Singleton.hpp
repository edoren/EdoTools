#pragma once

#include <edoren/util/Config.hpp>

namespace edoren {

template <class T>
class Singleton {
public:
    Singleton() {
        EDOTOOLS_ASSERT(!sInstance, "Instance not found");
        sInstance = static_cast<T*>(this);
    }

    ~Singleton() {
        EDOTOOLS_ASSERT(sInstance, "Instance not found");
        sInstance = nullptr;
    }

    static T& GetInstance() {
        EDOTOOLS_ASSERT(sInstance, "Instance not found");
        return (*sInstance);
    }

    static T* GetInstancePtr() {
        return sInstance;
    }

    Singleton(const Singleton<T>&) = delete;
    Singleton<T>& operator=(const Singleton<T>&) = delete;

protected:
    static inline T* sInstance = nullptr;
};

}  // namespace edoren
