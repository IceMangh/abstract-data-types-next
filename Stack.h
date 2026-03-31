#pragma once

#include "LinkedList.h"
#include "Exceptions.h"

template <class T>
class Stack {
private:
    LinkedList<T> data_;

public:
    Stack() : data_() {}

    Stack(const T* items, int count) : data_(items, count) {}

    Stack(const Stack<T>& other) : data_(other.data_) {}

    Stack<T>& operator=(const Stack<T>& other) {
        if (this != &other) {
            data_ = other.data_;
        }
        return *this;
    }

    int GetSize() const {
        return data_.GetLength();
    }

    bool IsEmpty() const {
        return data_.IsEmpty();
    }

    const T& Top() const {
        if (IsEmpty()) {
            throw EmptyStructure("Stack is empty");
        }
        return data_.GetLast();
    }

    const T& Get(int index) const {
        return data_.Get(index);
    }

    void Push(const T& item) {
        data_.Append(item);
    }

    T Pop() {
        if (IsEmpty()) {
            throw EmptyStructure("Stack is empty");
        }

        return data_.RemoveLast();
    }

    Stack<T> Concat(const Stack<T>& other) const {
        Stack<T> result(*this);
        for (int i = 0; i < other.GetSize(); ++i) {
            result.Push(other.Get(i));
        }
        return result;
    }

    Stack<T> GetSubsequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetSize() || endIndex >= GetSize() || startIndex > endIndex) {
            throw IndexOutOfRange();
        }

        Stack<T> result;
        for (int i = startIndex; i <= endIndex; ++i) {
            result.Push(Get(i));
        }
        return result;
    }

    int FindSubsequence(const Stack<T>& pattern) const {
        if (pattern.GetSize() == 0) {
            return 0;
        }
        if (pattern.GetSize() > GetSize()) {
            return -1;
        }

        for (int start = 0; start <= GetSize() - pattern.GetSize(); ++start) {
            bool matches = true;
            for (int j = 0; j < pattern.GetSize(); ++j) {
                if (Get(start + j) != pattern.Get(j)) {
                    matches = false;
                    break;
                }
            }
            if (matches) {
                return start;
            }
        }
        return -1;
    }

    template <class Mapper>
    Stack<T> Map(Mapper mapper) const {
        Stack<T> result;
        for (int i = 0; i < GetSize(); ++i) {
            result.Push(mapper(Get(i)));
        }
        return result;
    }

    template <class Predicate>
    Stack<T> Where(Predicate predicate) const {
        Stack<T> result;
        for (int i = 0; i < GetSize(); ++i) {
            if (predicate(Get(i))) {
                result.Push(Get(i));
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

    bool operator==(const Stack<T>& other) const {
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

    bool operator!=(const Stack<T>& other) const {
        return !(*this == other);
    }
};