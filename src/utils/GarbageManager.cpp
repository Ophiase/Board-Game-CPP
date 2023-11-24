#include "utils/GarbageManager.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-incomplete"

GarbageManager::~GarbageManager() {
    for (auto& obj : objectsToDelete)
        delete static_cast<decltype(obj)>(obj);
}

#pragma GCC diagnostic pop