#pragma once

#include "ListSequenceBase.h"

template <class T>
class MutableListSequence : public ListSequenceBase<T> {
protected:
    ListSequenceBase<T>* Instance() override {
        return this;
    }

public:
    MutableListSequence() : ListSequenceBase<T>() {}

    MutableListSequence(const T* items, int count) : ListSequenceBase<T>(items, count) {}

    MutableListSequence(const LinkedList<T>& list) : ListSequenceBase<T>(list) {}

    MutableListSequence(const MutableListSequence<T>& other) : ListSequenceBase<T>(other) {}

    bool IsEmpty() const {
        return this->data_.IsEmpty();
    }

    void Clear() {
        this->data_.Clear();
    }

    void Set(int index, const T& value) {
        this->data_.Set(index, value);
    }

    T RemoveFirst() {
        return this->data_.RemoveFirst();
    }

    T RemoveLast() {
        return this->data_.RemoveLast();
    }

    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new MutableListSequence<T>();
    }

    static MutableListSequence<T> From(const T* items, int count) {
        return MutableListSequence<T>(items, count);
    }
};
