#pragma once

#include "LinkedList.h"
#include "Sequence.h"
#include "SequenceEnumerator.h"

template <class T>
class ListSequenceBase : public Sequence<T> {
protected:
    LinkedList<T> data_;

    virtual ListSequenceBase<T>* Instance() = 0;

    Sequence<T>* AppendInternal(const T& Item) {
        data_.Append(Item);
        return this;
    }

    Sequence<T>* PrependInternal(const T& Item) {
        data_.Prepend(Item);
        return this;
    }

    Sequence<T>* InsertAtInternal(const T& Item, int index) {
        data_.InsertAt(Item, index);
        return this;
    }

public:
    ListSequenceBase() : data_() {}

    ListSequenceBase(const T* Items, int Count) : data_(Items, Count) {}

    ListSequenceBase(const LinkedList<T>& List) : data_(List) {}

    ListSequenceBase(const ListSequenceBase<T>& Other) : data_(Other.data_) {}

    const T& GetFirst() const override {
        return data_.GetFirst();
    }

    const T& GetLast() const override {
        return data_.GetLast();
    }

    const T& Get(int index) const override {
        return data_.Get(index);
    }

    int GetLength() const override {
        return data_.GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= data_.GetLength()) {
            throw IndexOutOfRange();
        }

        Sequence<T>* Result = this->CreateEmpty();

        try {
            for (int i = startIndex; i <= endIndex; ++i) {
                this->AppendToResult(Result, data_.Get(i));
            }
            return Result;
        } catch (...) {
            delete Result;
            throw;
        }
    }

    IEnumerator<T>* GetEnumerator() const override {
        return new SequenceEnumerator<T>(*this);
    }

    Sequence<T>* Append(const T& Item) override {
        ListSequenceBase<T>* target = Instance();

        try {
            return target->AppendInternal(Item);
        } catch (...) {
            if (target != this) {
                delete target;
            }
            throw;
        }
    }

    Sequence<T>* Prepend(const T& Item) override {
        ListSequenceBase<T>* target = Instance();

        try {
            return target->PrependInternal(Item);
        } catch (...) {
            if (target != this) {
                delete target;
            }
            throw;
        }
    }

    Sequence<T>* InsertAt(const T& Item, int index) override {
        ListSequenceBase<T>* target = Instance();

        try {
            return target->InsertAtInternal(Item, index);
        } catch (...) {
            if (target != this) {
                delete target;
            }
            throw;
        }
    }

    Sequence<T>* Concat(const Sequence<T>& other) const override {
        Sequence<T>* Result = this->Clone();
        IEnumerator<T>* enumerator = other.GetEnumerator();

        try {
            while (enumerator->MoveNext()) {
                this->AppendToResult(Result, enumerator->Current());
            }
            delete enumerator;
            return Result;
        } catch (...) {
            delete enumerator;
            delete Result;
            throw;
        }
    }
};
