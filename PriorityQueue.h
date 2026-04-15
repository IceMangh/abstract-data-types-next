#pragma once

#include "MutableListSequence.h"
#include "Exceptions.h"

template <class T>
struct PriorityQueueItem {
    T Value;
    int Priority;

    PriorityQueueItem() : Value(), Priority(0) {}

    PriorityQueueItem(const T& Value, int Priority)
            : Value(Value), Priority(Priority) {}
};

template <class T>
class PriorityQueue {
private:
    using Item = PriorityQueueItem<T>;
    MutableListSequence<Item> Items_;

public:
    PriorityQueue() : Items_() {}

    PriorityQueue(const PriorityQueue<T>& Other) = default;

    int GetSize() const {
        return Items_.GetLength();
    }

    bool IsEmpty() const {
        return Items_.IsEmpty();
    }

    const T& Top() const {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }
        return Items_.GetFirst().Value;
    }

    int TopPriority() const {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }
        return Items_.GetFirst().Priority;
    }

    const T& GetValue(int index) const {
        return Items_.Get(index).Value;
    }

    int GetPriority(int index) const {
        return Items_.Get(index).Priority;
    }

    void Push(const T& Value, int Priority) {
        Item item(Value, Priority);

        int insertIndex = 0;
        while (insertIndex < GetSize() && Items_.Get(insertIndex).Priority <= Priority) {
            ++insertIndex;
        }

        Items_.InsertAt(item, insertIndex);
    }

    T Pop() {
        if (IsEmpty()) {
            throw EmptyStructure("PriorityQueue is empty");
        }

        return Items_.RemoveFirst().Value;
    }

    bool operator==(const PriorityQueue<T>& Other) const {
        if (GetSize() != Other.GetSize()) {
            return false;
        }

        for (int i = 0; i < GetSize(); ++i) {
            if (GetValue(i) != Other.GetValue(i) || GetPriority(i) != Other.GetPriority(i)) {
                return false;
            }
        }

        return true;
    }

    PriorityQueue<T>& operator=(const PriorityQueue<T>& Other) = default;

    bool operator!=(const PriorityQueue<T>& Other) const {
        return !(*this == Other);
    }
};
