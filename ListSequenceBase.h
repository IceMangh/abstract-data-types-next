#pragma once

#include "LinkedList.h"
#include "Sequence.h"

template <class T>
class ListSequenceBase : public Sequence<T> {
protected:
    LinkedList<T> data_;

    virtual ListSequenceBase<T>* Instance() = 0;

    Sequence<T>* AppendInternal(const T& item) {
        data_.Append(item);
        return this;
    }

    Sequence<T>* PrependInternal(const T& item) {
        data_.Prepend(item);
        return this;
    }

    Sequence<T>* InsertAtInternal(const T& item, int index) {
        data_.InsertAt(item, index);
        return this;
    }

public:
    ListSequenceBase() : data_() {}

    ListSequenceBase(const T* items, int count) : data_(items, count) {}

    ListSequenceBase(const LinkedList<T>& list) : data_(list) {}

    ListSequenceBase(const ListSequenceBase<T>& other) : data_(other.data_) {}

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

        Sequence<T>* result = this->CreateEmpty();
        IEnumerator<T>* enumerator = data_.GetEnumerator();

        try {
            int i = -1;
            while (enumerator->MoveNext()) {
                ++i;
                if (i > endIndex) {
                    break;
                }
                if (i >= startIndex) {
                    this->AppendToResult(result, enumerator->Current());
                }
            }
            delete enumerator;
            return result;
        } catch (...) {
            delete enumerator;
            delete result;
            throw;
        }
    }

    IEnumerator<T>* GetEnumerator() const override {
        return data_.GetEnumerator();
    }

    Sequence<T>* Append(const T& item) override {
        ListSequenceBase<T>* target = Instance();

        try {
            return target->AppendInternal(item);
        } catch (...) {
            if (target != this) {
                delete target;
            }
            throw;
        }
    }

    Sequence<T>* Prepend(const T& item) override {
        ListSequenceBase<T>* target = Instance();

        try {
            return target->PrependInternal(item);
        } catch (...) {
            if (target != this) {
                delete target;
            }
            throw;
        }
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        ListSequenceBase<T>* target = Instance();

        try {
            return target->InsertAtInternal(item, index);
        } catch (...) {
            if (target != this) {
                delete target;
            }
            throw;
        }
    }

    Sequence<T>* Concat(const Sequence<T>& other) const override {
        Sequence<T>* result = this->Clone();
        IEnumerator<T>* enumerator = other.GetEnumerator();

        try {
            while (enumerator->MoveNext()) {
                this->AppendToResult(result, enumerator->Current());
            }
            delete enumerator;
            return result;
        } catch (...) {
            delete enumerator;
            delete result;
            throw;
        }
    }
};
