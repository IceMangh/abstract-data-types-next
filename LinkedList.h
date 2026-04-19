#pragma once
#include <stdexcept>
#include "Exceptions.h"

template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& data, Node* next = nullptr, Node* prev = nullptr)
                : data(data), next(next), prev(prev) {}
    };

    Node* head_;
    Node* tail_;
    int length_;

    Node* GetNode(int index) const {
        if (index < 0 || index >= length_) {
            throw IndexOutOfRange();
        }

        if (index < length_ / 2) {
            Node* cur = head_;
            for (int i = 0; i < index; ++i) {
                cur = cur->next;
            }
            return cur;
        } else {
            Node* cur = tail_;
            for (int i = length_ - 1; i > index; --i) {
                cur = cur->prev;
            }
            return cur;
        }
    }

public:
    LinkedList() : head_(nullptr), tail_(nullptr), length_(0) {}

    LinkedList(const T* Items, int Count) : head_(nullptr), tail_(nullptr), length_(0) {
        if (Count < 0) {
            throw std::invalid_argument("Negative Count");
        }

        for (int i = 0; i < Count; ++i) {
            Append(Items[i]);
        }
    }

    LinkedList(const LinkedList<T>& Other) : head_(nullptr), tail_(nullptr), length_(0) {
        Node* cur = Other.head_;
        while (cur != nullptr) {
            Append(cur->data);
            cur = cur->next;
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& Other) {
        if (this != &Other) {
            Clear();
            Node* cur = Other.head_;
            while (cur != nullptr) {
                Append(cur->data);
                cur = cur->next;
            }
        }
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    void Clear() {
        Node* cur = head_;
        while (cur != nullptr) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }

        head_ = nullptr;
        tail_ = nullptr;
        length_ = 0;
    }

    bool IsEmpty() const {
        return length_ == 0;
    }

    const T& GetFirst() const {
        if (length_ == 0) {
            throw EmptyStructure("List is empty");
        }
        return head_->data;
    }

    const T& GetLast() const {
        if (length_ == 0) {
            throw EmptyStructure("List is empty");
        }
        return tail_->data;
    }

    const T& Get(int index) const {
        return GetNode(index)->data;
    }

    void Set(int index, const T& value) {
        GetNode(index)->data = value;
    }

    int GetLength() const {
        return length_;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= length_ || endIndex >= length_ || startIndex > endIndex) {
            throw IndexOutOfRange();
        }

        LinkedList<T>* Result = new LinkedList<T>();
        for (int i = startIndex; i <= endIndex; ++i) {
            Result->Append(Get(i));
        }
        return Result;
    }

    void Append(const T& Item) {
        Node* node = new Node(Item, nullptr, tail_);

        if (length_ == 0) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            tail_ = node;
        }

        ++length_;
    }

    void Prepend(const T& Item) {
        Node* node = new Node(Item, head_, nullptr);

        if (length_ == 0) {
            head_ = tail_ = node;
        } else {
            head_->prev = node;
            head_ = node;
        }

        ++length_;
    }

    void InsertAt(const T& Item, int index) {
        if (index < 0 || index > length_) {
            throw IndexOutOfRange();
        }

        if (index == 0) {
            Prepend(Item);
            return;
        }

        if (index == length_) {
            Append(Item);
            return;
        }

        Node* current = GetNode(index);
        Node* previous = current->prev;
        Node* node = new Node(Item, current, previous);

        previous->next = node;
        current->prev = node;
        ++length_;
    }

    T RemoveFirst() {
        if (length_ == 0) {
            throw EmptyStructure("List is empty");
        }

        Node* node = head_;
        T value = node->data;

        if (length_ == 1) {
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            head_ = head_->next;
            head_->prev = nullptr;
        }

        delete node;
        --length_;
        return value;
    }

    T RemoveLast() {
        if (length_ == 0) {
            throw EmptyStructure("List is empty");
        }

        Node* node = tail_;
        T value = node->data;

        if (length_ == 1) {
            head_ = nullptr;
            tail_ = nullptr;
        } else {
            tail_ = tail_->prev;
            tail_->next = nullptr;
        }

        delete node;
        --length_;
        return value;
    }

    LinkedList<T>* Concat(const LinkedList<T>* Other) const {
        LinkedList<T>* Result = new LinkedList<T>(*this);
        for (int i = 0; i < Other->GetLength(); ++i) {
            Result->Append(Other->Get(i));
        }
        return Result;
    }

    T& operator[](int index) {
        return GetNode(index)->data;
    }
};