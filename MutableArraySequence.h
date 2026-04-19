#pragma once

#include "ArraySequenceBase.h"

template <class T>
class MutableArraySequence : public ArraySequenceBase<T> {
protected:
    ArraySequenceBase<T>* Instance() override {
        return this;
    }

public:
    MutableArraySequence() : ArraySequenceBase<T>() {}

    MutableArraySequence(const T* Items, int Count) : ArraySequenceBase<T>(Items, Count) {}

    MutableArraySequence(const DynamicArray<T>& dynamicArray) : ArraySequenceBase<T>(dynamicArray) {}

    MutableArraySequence(const MutableArraySequence<T>& Other) : ArraySequenceBase<T>(Other) {}

    Sequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new MutableArraySequence<T>();
    }

    static MutableArraySequence<T> From(const T* Items, int Count) {
        return MutableArraySequence<T>(Items, Count);
    }
};
