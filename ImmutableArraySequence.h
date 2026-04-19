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

    ImmutableArraySequence(const T* Items, int Count) : ArraySequenceBase<T>(Items, Count) {}

    ImmutableArraySequence(const DynamicArray<T>& dynamicArray) : ArraySequenceBase<T>(dynamicArray) {}

    ImmutableArraySequence(const ImmutableArraySequence<T>& Other) : ArraySequenceBase<T>(Other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

    static ImmutableArraySequence<T> From(const T* Items, int Count) {
        return ImmutableArraySequence<T>(Items, Count);
    }
};
