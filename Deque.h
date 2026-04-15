#pragma once

#include "MutableListSequence.h"
#include "Exceptions.h"

template <class T>
class Deque {
private:
    MutableListSequence<T> Items_;

public:
    Deque() : Items_() {}

    Deque(const T* Items, int Count) : Items_(Items, Count) {}

    Deque(const Deque<T>& Other) = default;

    ~Deque() = default;

    void Clear() {
        Items_.Clear();
    }

    int GetSize() const {
        return Items_.GetLength();
    }

    bool IsEmpty() const {
        return Items_.IsEmpty();
    }

    const T& Front() const {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }
        return Items_.GetFirst();
    }

    const T& Back() const {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }
        return Items_.GetLast();
    }

    const T& Get(int index) const {
        return Items_.Get(index);
    }

    void Set(int index, const T& value) {
        Items_.Set(index, value);
    }

    void PushFront(const T& Item) {
        Items_.Prepend(Item);
    }

    void PushBack(const T& Item) {
        Items_.Append(Item);
    }

    T PopFront() {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }

        return Items_.RemoveFirst();
    }

    T PopBack() {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }

        return Items_.RemoveLast();
    }

    Deque<T> Concat(const Deque<T>& Other) const {
        Deque<T> Result(*this);
        for (int i = 0; i < Other.GetSize(); ++i) {
            Result.PushBack(Other.Get(i));
        }
        return Result;
    }

    Deque<T>* GetSubsequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetSize() || endIndex >= GetSize() || startIndex > endIndex) {
            throw IndexOutOfRange();
        }

        Deque<T>* Result = new Deque<T>();
        for (int i = startIndex; i <= endIndex; ++i) {
            Result->PushBack(Get(i));
        }
        return Result;
    }

    template <class Mapper>
    Deque<T> Map(Mapper mapper) const {
        Deque<T> Result;
        for (int i = 0; i < GetSize(); ++i) {
            Result.PushBack(mapper(Get(i)));
        }
        return Result;
    }

    template <class Predicate>
    Deque<T> Where(Predicate predicate) const {
        Deque<T> Result;
        for (int i = 0; i < GetSize(); ++i) {
            if (predicate(Get(i))) {
                Result.PushBack(Get(i));
            }
        }
        return Result;
    }

    template <class Reducer>
    T Reduce(Reducer reducer, T start) const {
        T Result = start;
        for (int i = 0; i < GetSize(); ++i) {
            Result = reducer(Result, Get(i));
        }
        return Result;
    }

    template <class Comparator>
    void Sort(Comparator comparator) {
        if (GetSize() < 2) {
            return;
        }

        for (int i = 0; i < GetSize() - 1; ++i) {
            int bestIndex = i;
            for (int j = i + 1; j < GetSize(); ++j) {
                if (comparator(Get(j), Get(bestIndex))) {
                    bestIndex = j;
                }
            }
            if (bestIndex != i) {
                T temp = Get(i);
                Set(i, Get(bestIndex));
                Set(bestIndex, temp);
            }
        }
    }

    void Sort() {
        Sort([](const T& left, const T& right) {
            return left < right;
        });
    }

    bool operator==(const Deque<T>& Other) const {
        if (GetSize() != Other.GetSize()) {
            return false;
        }

        for (int i = 0; i < GetSize(); ++i) {
            if (Get(i) != Other.Get(i)) {
                return false;
            }
        }
        return true;
    }

    Deque<T>& operator=(const Deque<T>& Other) = default;

    bool operator!=(const Deque<T>& Other) const {
        return !(*this == Other);
    }
};
