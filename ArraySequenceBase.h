#pragma once

#include <memory>
#include "DynamicArray.h"
#include "Exceptions.h"
#include "Sequence.h"
#include "SequenceEnumerator.h"

template <class T>
class ArraySequenceBase : public Sequence<T> {
protected:
    DynamicArray<T> data_;

    virtual ArraySequenceBase<T>* Instance() = 0;

    Sequence<T>* AppendInternal(const T& item) {
        const int oldSize = data_.GetSize();
        data_.Resize(oldSize + 1);
        data_.Set(oldSize, item);
        return this;
    }

    Sequence<T>* PrependInternal(const T& item) {
        const int oldSize = data_.GetSize();
        data_.Resize(oldSize + 1);

        for (int i = oldSize; i > 0; --i) {
            data_.Set(i, data_.Get(i - 1));
        }
        data_.Set(0, item);
        return this;
    }

    Sequence<T>* InsertAtInternal(const T& item, int index) {
        const int length = data_.GetSize();
        if (index < 0 || index > length) {
            throw IndexOutOfRange();
        }

        if (index == length) {
            return AppendInternal(item);
        }

        data_.Resize(length + 1);
        for (int i = length; i > index; --i) {
            data_.Set(i, data_.Get(i - 1));
        }
        data_.Set(index, item);
        return this;
    }

public:
    ArraySequenceBase() : data_(0) {}

    ArraySequenceBase(const T* items, int count) : data_(items, count) {}

    ArraySequenceBase(const DynamicArray<T>& dynamicArray) : data_(dynamicArray) {}

    ArraySequenceBase(const ArraySequenceBase<T>& other) : data_(other.data_) {}

    const T& GetFirst() const override {
        if (data_.GetSize() == 0) {
            throw EmptyStructure();
        }
        return data_.Get(0);
    }

    const T& GetLast() const override {
        if (data_.GetSize() == 0) {
            throw EmptyStructure();
        }
        return data_.Get(data_.GetSize() - 1);
    }

    const T& Get(int index) const override {
        return data_.Get(index);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= data_.GetSize()) {
            throw IndexOutOfRange();
        }

        std::unique_ptr<Sequence<T>> result(this->CreateEmpty());
        for (int i = startIndex; i <= endIndex; ++i) {
            this->AppendToResult(result, data_.Get(i));
        }

        return result.release();
    }

    int GetLength() const override {
        return data_.GetSize();
    }

    IEnumerator<T>* GetEnumerator() const override {
        return new SequenceEnumerator<T>(*this);
    }

    Sequence<T>* Append(const T& item) override {
        ArraySequenceBase<T>* target = Instance();
        std::unique_ptr<ArraySequenceBase<T>> targetGuard((target == this) ? nullptr : target);

        Sequence<T>* result = target->AppendInternal(item);
        if (result == target) {
            targetGuard.release();
        }
        return result;
    }

    Sequence<T>* Prepend(const T& item) override {
        ArraySequenceBase<T>* target = Instance();
        std::unique_ptr<ArraySequenceBase<T>> targetGuard((target == this) ? nullptr : target);

        Sequence<T>* result = target->PrependInternal(item);
        if (result == target) {
            targetGuard.release();
        }
        return result;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        ArraySequenceBase<T>* target = Instance();
        std::unique_ptr<ArraySequenceBase<T>> targetGuard((target == this) ? nullptr : target);

        Sequence<T>* result = target->InsertAtInternal(item, index);
        if (result == target) {
            targetGuard.release();
        }
        return result;
    }

    Sequence<T>* Concat(const Sequence<T>& list) const override {
        std::unique_ptr<Sequence<T>> result(this->Clone());
        std::unique_ptr<IEnumerator<T>> enumerator(list.GetEnumerator());

        while (enumerator->MoveNext()) {
            this->AppendToResult(result, enumerator->Current());
        }

        return result.release();
    }
};
