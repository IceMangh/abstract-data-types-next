#pragma once

#include <iostream>
#include "DynamicArray.h"
#include "Exceptions.h"

template <class T>
class PriorityQueue {
private:
    struct PriorityItem {
        T value;
        int priority;

        PriorityItem() : value(), priority(0) {}

        PriorityItem(const T& value, int priority)
                : value(value), priority(priority) {}
    };

    DynamicArray<PriorityItem> data_;

public:
    PriorityQueue() : data_() {}

    PriorityQueue(const PriorityQueue<T>& other) : data_(other.data_) {}

    PriorityQueue<T>& operator=(const PriorityQueue<T>& other) {
        if (this != &other) {
            data_ = other.data_;
        }
        return *this;
    }

    int GetSize() const {
        return data_.GetSize();
    }

    bool IsEmpty() const {
        return GetSize() == 0;
    }

    const T& Top() const {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }
        return data_.Get(0).value;
    }

    int TopPriority() const {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }
        return data_.Get(0).priority;
    }

    const T& GetValue(int index) const {
        return data_.Get(index).value;
    }

    int GetPriority(int index) const {
        return data_.Get(index).priority;
    }

    void Push(const T& value, int priority) {
        PriorityItem item(value, priority);

        int insertIndex = 0;
        while (insertIndex < GetSize() && data_.Get(insertIndex).priority <= priority) {
            ++insertIndex;
        }

        int oldSize = GetSize();
        data_.Resize(oldSize + 1);

        for (int i = oldSize; i > insertIndex; --i) {
            data_.Set(i, data_.Get(i - 1));
        }

        data_.Set(insertIndex, item);
    }

    T Pop() {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }

        T value = data_.Get(0).value;

        for (int i = 1; i < GetSize(); ++i) {
            data_.Set(i - 1, data_.Get(i));
        }

        data_.Resize(GetSize() - 1);
        return value;
    }

    bool operator==(const PriorityQueue<T>& other) const {
        if (GetSize() != other.GetSize()) {
            return false;
        }

        for (int i = 0; i < GetSize(); ++i) {
            if (GetValue(i) != other.GetValue(i) || GetPriority(i) != other.GetPriority(i)) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const PriorityQueue<T>& other) const {
        return !(*this == other);
    }
};

