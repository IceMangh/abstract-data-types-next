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

    ImmutableListSequence(const T* items, int count) : ListSequenceBase<T>(items, count) {}

    ImmutableListSequence(const LinkedList<T>& list) : ListSequenceBase<T>(list) {}

    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequenceBase<T>(other) {}

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ImmutableListSequence<T>();
    }

    static ImmutableListSequence<T> From(const T* items, int count) {
        return ImmutableListSequence<T>(items, count);
    }
};
