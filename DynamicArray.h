#pragma once
#include <stdexcept>
#include "Exceptions.h"

template <class T>
class DynamicArray {
private:
    T* data_;
    int size_;

    void CheckIndex(int index) const {
        if (index < 0 || index >= size_) {
            throw IndexOutOfRange();
        }
    }

public:
    DynamicArray() : data_(nullptr), size_(0) {}

    explicit DynamicArray(int size) : data_(nullptr), size_(size) {
        if (size < 0) {
            throw std::invalid_argument("Negative size");
        }
        if (size_ > 0) {
            data_ = new T[size_]{};
        }
    }

    DynamicArray(const T* Items, int Count) : data_(nullptr), size_(Count) {
        if (Count < 0) {
            throw std::invalid_argument("Negative size");
        }
        if (size_ > 0) {
            data_ = new T[size_];
            for (int i = 0; i < size_; ++i) {
                data_[i] = Items[i];
            }
        }
    }

    DynamicArray(const DynamicArray<T>& Other) : data_(nullptr), size_(Other.size_) {
        if (size_ > 0) {
            data_ = new T[size_];
            for (int i = 0; i < size_; ++i) {
                data_[i] = Other.data_[i];
            }
        }
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& Other) {
        if (this != &Other) {
            T* NewData = nullptr;
            if (Other.size_ > 0) {
                NewData = new T[Other.size_];
                for (int i = 0; i < Other.size_; ++i) {
                    NewData[i] = Other.data_[i];
                }
            }
            delete[] data_;
            data_ = NewData;
            size_ = Other.size_;
        }
        return *this;
    }

    ~DynamicArray() {
        delete[] data_;
    }

    const T& Get(int index) const {
        CheckIndex(index);
        return data_[index];
    }


    int GetSize() const {
        return size_;
    }

    void Set(int index, const T& value) {
        CheckIndex(index);
        data_[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw std::invalid_argument("Negative size");
        }

        T* NewData = nullptr;
        if (newSize > 0) {
            NewData = new T[newSize]{};
            int copySize = (newSize < size_) ? newSize : size_;
            for (int i = 0; i < copySize; ++i) {
                NewData[i] = data_[i];
            }
        }

        delete[] data_;
        data_ = NewData;
        size_ = newSize;
    }

    T& operator[](int index) {
        CheckIndex(index);
        return data_[index];
    }

};


