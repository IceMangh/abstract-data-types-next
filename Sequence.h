#pragma once
#include <memory>
#include "IEnumerable.h"
#include "ICollection.h"

template <class T>
class Sequence : public IEnumerable<T>, public ICollection<T> {
protected:
    void AppendToResult(std::unique_ptr<Sequence<T>>& result, const T& item) const {
        Sequence<T>* current = result.get();
        Sequence<T>* updated = current->Append(item);

        if (updated != current) {
            result.release();
            delete current;
            result.reset(updated);
        }
    }

public:
    virtual ~Sequence() = default;

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const override = 0;
    virtual int GetLength() const = 0;

    int GetCount() const override {
        return GetLength();
    }

    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>& other) const = 0;

    virtual Sequence<T>* Clone() const override = 0;
    virtual Sequence<T>* CreateEmpty() const = 0;

    template <class Mapper>
    Sequence<T>* Map(Mapper mapper) const {
        std::unique_ptr<Sequence<T>> result(CreateEmpty());
        std::unique_ptr<IEnumerator<T>> enumerator(this->GetEnumerator());

        while (enumerator->MoveNext()) {
            AppendToResult(result, mapper(enumerator->Current()));
        }

        return result.release();
    }

    template <class Reducer>
    T Reduce(Reducer reducer, T start) const {
        T result = start;
        std::unique_ptr<IEnumerator<T>> enumerator(this->GetEnumerator());

        while (enumerator->MoveNext()) {
            result = reducer(result, enumerator->Current());
        }

        return result;
    }

    bool operator==(const Sequence<T>& other) const {
        if (GetLength() != other.GetLength()) {
            return false;
        }

        for (int i = 0; i < GetLength(); ++i) {
            if (Get(i) != other.Get(i)) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const Sequence<T>& other) const {
        return !(*this == other);
    }

    Sequence<T>* operator+(const Sequence<T>& other) const {
        return Concat(other);
    }
};
