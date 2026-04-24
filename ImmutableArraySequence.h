#pragma once

#include "ArraySequenceBase.h"

template <class T>
class ImmutableArraySequence : public ArraySequenceBase<T> {
protected:
    ArraySequenceBase<T>* Instance() override {
        return new ImmutableArraySequence<T>(*this);
    }

public:
    ImmutableArraySequence() : ArraySequenceBase<T>() {}

    ImmutableArraySequence(const T* items, int count) : ArraySequenceBase<T>(items, count) {}

    ImmutableArraySequence(const DynamicArray<T>& dynamicArray) : ArraySequenceBase<T>(dynamicArray) {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequenceBase<T>(other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

    static ImmutableArraySequence<T> From(const T* items, int count) {
        return ImmutableArraySequence<T>(items, count);
    }
};
