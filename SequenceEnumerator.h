#pragma once
#include "IEnumerator.h"
#include "Sequence.h"

template <class T>
class SequenceEnumerator : public IEnumerator<T> {
private:
    const Sequence<T>& sequence_;
    int index_;

public:
    SequenceEnumerator(const Sequence<T>& sequence)
            : sequence_(sequence), index_(-1) {}

    bool MoveNext() override {
        if (index_ + 1 < sequence_.GetLength()) {
            ++index_;
            return true;
        }
        return false;
    }

    T Current() const override {
        return sequence_.Get(index_);
    }
};