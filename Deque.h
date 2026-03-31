#pragma once

#include "LinkedList.h"
#include "Exceptions.h"

template <class T>
class Deque {
private:
    LinkedList<T> data_;

public:
    Deque() : data_() {}

    Deque(const T* items, int count) : data_(items, count) {}

    Deque(const Deque<T>& other) : data_(other.data_) {}

    Deque<T>& operator=(const Deque<T>& other) {
        if (this != &other) {
            data_ = other.data_;
        }
        return *this;
    }

    ~Deque() = default;

    void Clear() {
        data_.Clear();
    }

    int GetSize() const {
        return data_.GetLength();
    }

    bool IsEmpty() const {
        return data_.IsEmpty();
    }

    const T& Front() const {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }
        return data_.GetFirst();
    }

    const T& Back() const {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }
        return data_.GetLast();
    }

    const T& Get(int index) const {
        return data_.Get(index);
    }

    void Set(int index, const T& value) {
        data_.Set(index, value);
    }

    void PushFront(const T& item) {
        data_.Prepend(item);
    }

    void PushBack(const T& item) {
        data_.Append(item);
    }

    T PopFront() {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }

        return data_.RemoveFirst();
    }

    T PopBack() {
        if (IsEmpty()) {
            throw EmptyStructure("Deque is empty");
        }

        return data_.RemoveLast();
    }

    Deque<T> Concat(const Deque<T>& other) const {
        Deque<T> result(*this);
        for (int i = 0; i < other.GetSize(); ++i) {
            result.PushBack(other.Get(i));
        }
        return result;
    }

    Deque<T> GetSubsequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetSize() || endIndex >= GetSize() || startIndex > endIndex) {
            throw IndexOutOfRange();
        }

        Deque<T> result;
        for (int i = startIndex; i <= endIndex; ++i) {
            result.PushBack(Get(i));
        }
        return result;
    }

    template <class Mapper>
    Deque<T> Map(Mapper mapper) const {
        Deque<T> result;
        for (int i = 0; i < GetSize(); ++i) {
            result.PushBack(mapper(Get(i)));
        }
        return result;
    }

    template <class Predicate>
    Deque<T> Where(Predicate predicate) const {
        Deque<T> result;
        for (int i = 0; i < GetSize(); ++i) {
            if (predicate(Get(i))) {
                result.PushBack(Get(i));
            }
        }
        return result;
    }

    template <class Reducer>
    T Reduce(Reducer reducer, T start) const {
        T result = start;
        for (int i = 0; i < GetSize(); ++i) {
            result = reducer(result, Get(i));
        }
        return result;
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

    bool operator==(const Deque<T>& other) const {
        if (GetSize() != other.GetSize()) {
            return false;
        }

        for (int i = 0; i < GetSize(); ++i) {
            if (Get(i) != other.Get(i)) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Deque<T>& other) const {
        return !(*this == other);
    }
};