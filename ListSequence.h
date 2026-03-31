#pragma once
#include "LinkedList.h"
#include "Sequence.h"
#include "SequenceEnumerator.h"

template <class T>
class ListSequence : public Sequence<T> {
private:
    LinkedList<T> items_;

public:
    ListSequence() : items_() {}

    ListSequence(const T* items, int count) : items_(items, count) {}

    ListSequence(const LinkedList<T>& list) : items_(list) {}

    ListSequence(const ListSequence<T>& other) : items_(other.items_) {}

    const T& GetFirst() const override {
        return items_.GetFirst();
    }

    const T& GetLast() const override {
        return items_.GetLast();
    }

    const T& Get(int index) const override {
        return items_.Get(index);
    }

    int GetLength() const override {
        return items_.GetLength();
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        LinkedList<T>* sub = items_.GetSubList(startIndex, endIndex);
        Sequence<T>* result = new ListSequence<T>(*sub);
        delete sub;
        return result;
    }

    Sequence<T>* Append(const T& item) override {
        items_.Append(item);
        return this;
    }

    Sequence<T>* Prepend(const T& item) override {
        items_.Prepend(item);
        return this;
    }

    Sequence<T>* InsertAt(const T& item, int index) override {
        items_.InsertAt(item, index);
        return this;
    }

    Sequence<T>* Concat(const Sequence<T>& other) const override {
        std::unique_ptr<Sequence<T>> result(this->Clone());
        for (int i = 0; i < other.GetLength(); ++i) {
            this->AppendToResult(result, other.Get(i));
        }
        return result.release();
    }

    Sequence<T>* Clone() const override {
        return new ListSequence<T>(*this);
    }

    Sequence<T>* CreateEmpty() const override {
        return new ListSequence<T>();
    }

    static ListSequence<T> From(const T* items, int count) {
        return ListSequence<T>(items, count);
    }

    IEnumerator<T>* GetEnumerator() const override {
        return new SequenceEnumerator<T>(*this);
    }
};
