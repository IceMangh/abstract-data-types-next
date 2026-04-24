#pragma once

#include "MutableListSequence.h"
#include "Exceptions.h"

template <class T>
struct PriorityQueueItem {
    T value;
    int priority;

    PriorityQueueItem() : value(), priority(0) {}

    PriorityQueueItem(const T& value, int priority)
            : value(value), priority(priority) {}
};

template <class T>
class PriorityQueue {
private:
    using QueueItem = PriorityQueueItem<T>;
    MutableListSequence<QueueItem> items_;

public:
    PriorityQueue() : items_() {}

    PriorityQueue(const PriorityQueue<T>& other) = default;

    int GetSize() const {
        return items_.GetLength();
    }

    bool IsEmpty() const {
        return items_.IsEmpty();
    }

    const T& Top() const {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }
        return items_.GetFirst().value;
    }

    int TopPriority() const {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }
        return items_.GetFirst().priority;
    }

    const T& GetValue(int index) const {
        return items_.Get(index).value;
    }

    int GetPriority(int index) const {
        return items_.Get(index).priority;
    }

    void Push(const T& value, int priority) {
        QueueItem item(value, priority);

        int insertIndex = 0;
        while (insertIndex < GetSize() && items_.Get(insertIndex).priority <= priority) {
            ++insertIndex;
        }

        items_.InsertAt(item, insertIndex);
    }

    T Pop() {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }

        return items_.RemoveFirst().value;
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

    PriorityQueue<T>& operator=(const PriorityQueue<T>& other) = default;

    bool operator!=(const PriorityQueue<T>& other) const {
        return !(*this == other);
    }
};
