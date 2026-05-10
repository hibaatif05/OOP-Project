#ifndef STORAGE_H
#define STORAGE_H
#include <iostream>
using namespace std;
template <typename T>
class Storage {
private:
    T data[100];
    int count;
public:
    Storage() : count(0) {}
    bool add(const T& item) {
        if (count >= 100) {
            return false;
        }
        data[count++] = item;
        return true;
    }
    bool removeAt(int index) {
        if (index < 0 || index >= count) {
            return false;
        }
        for (int i = index; i < count - 1; ++i) {
            data[i] = data[i + 1];
        }
        --count;
        return true;
    }
    bool removeById(int id) {
        for (int i = 0; i < count; ++i) {
            if (data[i].getId() == id) {
                return removeAt(i);
            }
        }
        return false;
    }
    T* findById(int id) {
        for (int i = 0; i < count; ++i) {
            if (data[i].getId() == id) {
                return &data[i];
            }
        }
        return nullptr;
    }
    const T* findById(int id) const {
        for (int i = 0; i < count; ++i) {
            if (data[i].getId() == id) {
                return &data[i];
            }
        }
        return nullptr;
    }
    T& getAt(int i) {
        return data[i];
    }
    const T& getAt(int i) const {
        return data[i];
    }
    int size() const {
        return count;
    }
    void clear() {
        count = 0;
    }
};
#endif