#pragma once

#include "ListSequenceBase.h"

template <class T>
class ImmutableListSequence : public ListSequenceBase<T> {
protected:
    ListSequenceBase<T>* Instance() override {
        return new ImmutableListSequence<T>(*this);
    }

public:
    ImmutableListSequence() : ListSequenceBase<T>() {}

    ImmutableListSequence(const T* Items, int Count) : ListSequenceBase<T>(Items, Count) {}

    ImmutableListSequence(const LinkedList<T>& List) : ListSequenceBase<T>(List) {}

    ImmutableListSequence(const ImmutableListSequence<T>& Other) : ListSequenceBase<T>(Other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableListSequence<T>();
    }

    static ImmutableListSequence<T> From(const T* Items, int Count) {
        return ImmutableListSequence<T>(Items, Count);
    }
};
