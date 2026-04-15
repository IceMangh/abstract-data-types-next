#pragma once

#include "MutableListSequence.h"
#include "Exceptions.h"

template <class T>
class Stack {
private:
    MutableListSequence<T> Items_;

public:
    Stack() : Items_() {}

    Stack(const T* Items, int Count) : Items_(Items, Count) {}

    Stack(const Stack<T>& Other) = default;

    int GetSize() const {
        return Items_.GetLength();
    }

    bool IsEmpty() const {
        return Items_.IsEmpty();
    }

    const T& Top() const {
        if (IsEmpty()) {
            throw EmptyStructure("Stack is empty");
        }
        return Items_.GetLast();
    }

    const T& Get(int index) const {
        return Items_.Get(index);
    }

    void Push(const T& Item) {
        Items_.Append(Item);
    }

    T Pop() {
        if (IsEmpty()) {
            throw EmptyStructure("Stack is empty");
        }

        return Items_.RemoveLast();
    }

    Stack<T> Concat(const Stack<T>& Other) const {
        Stack<T> Result(*this);
        for (int i = 0; i < Other.GetSize(); ++i) {
            Result.Push(Other.Get(i));
        }
        return Result;
    }

    Stack<T>* GetSubsequence(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= GetSize() || endIndex >= GetSize() || startIndex > endIndex) {
            throw IndexOutOfRange();
        }

        Stack<T>* Result = new Stack<T>();
        for (int i = startIndex; i <= endIndex; ++i) {
            Result->Push(Get(i));
        }
        return Result;
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
        Stack<T> Result;
        for (int i = 0; i < GetSize(); ++i) {
            Result.Push(mapper(Get(i)));
        }
        return Result;
    }

    template <class Predicate>
    Stack<T> Where(Predicate predicate) const {
        Stack<T> Result;
        for (int i = 0; i < GetSize(); ++i) {
            if (predicate(Get(i))) {
                Result.Push(Get(i));
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

    bool operator==(const Stack<T>& Other) const {
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

    Stack<T>& operator=(const Stack<T>& Other) = default;

    bool operator!=(const Stack<T>& Other) const {
        return !(*this == Other);
    }
};
