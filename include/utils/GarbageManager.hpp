#pragma once

#include <vector>

class GarbageManager {
private:
    std::vector<void*> objectsToDelete;

public:
    template <typename T>
    void addObjectToDelete(T* object) {
        objectsToDelete.push_back(static_cast<void*>(object));
    }

    ~GarbageManager();
};