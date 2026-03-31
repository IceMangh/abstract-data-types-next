#pragma once
#include <cassert>
#include <iostream>
#include <stdexcept>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "ListSequence.h"
#include "Stack.h"
#include "Deque.h"
#include "PriorityQueue.h"

inline int Double(int x) {
    return x * 2;
}

inline bool IsEven(int x) {
    return x % 2 == 0;
}

inline int Sum(int a, int b) {
    return a + b;
}

inline int Multiply(int a, int b) {
    return a * b;
}

inline void TestDynamicArray() {
    int data[] = {1, 2, 3};
    DynamicArray<DynamicArray<int>> arr(3);
    DynamicArray<int> arr1(data, 3);
    DynamicArray<int> arr2(data, 3);
    DynamicArray<int> arr3(data, 3);
    arr.Set(0, arr1);
    arr.Set(1, arr2);
    arr.Set(2, arr3);
    assert(arr.Get(0).Get(0) == 1);
    assert(arr.Get(2).Get(2) == 3);
}

inline void TestDynamicArrayExceptions() {
    DynamicArray<int> arr(2);
    bool thrown = false;
    try {
        arr.Get(3);
    } catch (const IndexOutOfRange&) {
        thrown = true;
    }
    assert(thrown);
}

inline void TestLinkedList() {
    int data[] = {1, 2, 3};
    LinkedList<int> list(data, 3);
    assert(list.GetFirst() == 1);
    assert(list.GetLast() == 3);
    list.Append(4);
    list.Prepend(0);
    list.InsertAt(99, 2);
    assert(list.GetFirst() == 0);
    assert(list.Get(2) == 99);
    assert(list.GetLast() == 4);
}

inline void TestArraySequence() {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);
    seq.Append(4)->Prepend(0)->InsertAt(99, 2);
    assert(seq.GetFirst() == 0);
    assert(seq.Get(2) == 99);
    assert(seq.GetLast() == 4);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 1);
    delete sub;
}

inline void TestArraySequenceMap() {
    int data[] = {1, 2, 32};
    MutableArraySequence<int> seq(data, 3);
    Sequence<int>* mapped = seq.Map(Double);

    assert(mapped->GetLength() == 3);
    assert(mapped->Get(0) == 2);
    assert(mapped->Get(1) == 4);
    assert(mapped->Get(2) == 64);
    delete mapped;
}

inline void TestArraySequenceFrom() {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq = MutableArraySequence<int>::From(data, 3);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 1);
    assert(seq.Get(1) == 2);
    assert(seq.Get(2) == 3);
}

inline void TestArraySequenceReduce() {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    int sumResult = seq.Reduce(Sum, 0);
    int mulResult = seq.Reduce(Multiply, 1);

    assert(sumResult == 10);
    assert(mulResult == 24);
}

inline void TestListSequence() {
    int data[] = {1, 2, 3};
    ListSequence<int> seq(data, 3);
    seq.Append(4)->Prepend(0)->InsertAt(99, 2);
    assert(seq.GetFirst() == 0);
    assert(seq.Get(2) == 99);
    assert(seq.GetLast() == 4);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 1);
    delete sub;
}

inline void TestListSequenceMap() {
    int data[] = {1, 2, 3};
    ListSequence<int> seq(data, 3);
    Sequence<int>* mapped = seq.Map(Double);

    assert(mapped->GetLength() == 3);
    assert(mapped->Get(0) == 2);
    assert(mapped->Get(1) == 4);
    assert(mapped->Get(2) == 6);

    delete mapped;
}

inline void TestListSequenceFrom() {
    int data[] = {1, 2, 3};
    ListSequence<int> seq = ListSequence<int>::From(data, 3);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 1);
    assert(seq.Get(1) == 2);
    assert(seq.Get(2) == 3);
}

inline void TestListSequenceReduce() {
    int data[] = {1, 2, 3, 4};
    ListSequence<int> seq(data, 4);

    int sumResult = seq.Reduce(Sum, 0);
    int mulResult = seq.Reduce(Multiply, 1);

    assert(sumResult == 10);
    assert(mulResult == 24);
}

inline void TestConcat() {
    int a[] = {1, 2};
    int b[] = {3, 4};

    MutableArraySequence<int> seq1(a, 2);
    ListSequence<int> seq2(b, 2);

    Sequence<int>* joined = seq1 + seq2;
    assert(joined->GetLength() == 4);
    assert(joined->Get(0) == 1);
    assert(joined->Get(1) == 2);
    assert(joined->Get(2) == 3);
    assert(joined->Get(3) == 4);

    delete joined;
}

inline void TestClone() {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int>* copy = seq.Clone();

    assert(copy->GetCount() == 3);
    assert(copy->Get(0) == 1);
    assert(copy->Get(1) == 2);
    assert(copy->Get(2) == 3);

    delete copy;
}

inline void TestImmutableArraySequence() {
    int data[] = {1, 2, 3};
    ImmutableArraySequence<int> original(data, 3);

    Sequence<int>* appended = original.Append(4);
    Sequence<int>* prepended = original.Prepend(0);
    Sequence<int>* inserted = original.InsertAt(99, 1);

    assert(original.GetLength() == 3);
    assert(original.Get(0) == 1);
    assert(original.Get(1) == 2);
    assert(original.Get(2) == 3);

    assert(appended->GetLength() == 4);
    assert(appended->GetLast() == 4);

    assert(prepended->GetLength() == 4);
    assert(prepended->GetFirst() == 0);

    assert(inserted->GetLength() == 4);
    assert(inserted->Get(1) == 99);
    assert(inserted->Get(2) == 2);

    delete appended;
    delete prepended;
    delete inserted;
}

inline void TestImmutableArraySequenceMapReduce() {
    int data[] = {1, 2, 3, 4};
    ImmutableArraySequence<int> seq(data, 4);

    Sequence<int>* mapped = seq.Map(Double);
    Sequence<int>* sub = seq.GetSubsequence(1, 2);
    Sequence<int>* joined = seq + *sub;

    assert(mapped->GetLength() == 4);
    assert(mapped->Get(0) == 2);
    assert(mapped->Get(3) == 8);

    assert(sub->GetLength() == 2);
    assert(sub->Get(0) == 2);
    assert(sub->Get(1) == 3);

    assert(joined->GetLength() == 6);
    assert(joined->Get(4) == 2);
    assert(joined->Get(5) == 3);

    assert(seq.Reduce(Sum, 0) == 10);
    assert(seq.Reduce(Multiply, 1) == 24);

    delete mapped;
    delete sub;
    delete joined;
}

inline void TestMutableVsImmutableDifference() {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> mutableSeq(data, 3);
    ImmutableArraySequence<int> immutableSeq(data, 3);

    mutableSeq.Append(10);
    Sequence<int>* immutableResult = immutableSeq.Append(10);

    assert(mutableSeq.GetLength() == 4);
    assert(mutableSeq.GetLast() == 10);

    assert(immutableSeq.GetLength() == 3);
    assert(immutableSeq.GetLast() == 3);
    assert(immutableResult->GetLength() == 4);
    assert(immutableResult->GetLast() == 10);

    delete immutableResult;
}

inline void TestStackBasic() {
    Stack<int> stack;
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);

    assert(stack.GetSize() == 3);
    assert(stack.Top() == 3);
    assert(stack.Pop() == 3);
    assert(stack.Top() == 2);
    assert(stack.Get(0) == 1);
}

inline void TestStackAlgorithms() {
    int data[] = {1, 2, 3, 4};
    Stack<int> stack(data, 4);

    Stack<int> mapped = stack.Map(Double);
    Stack<int> filtered = stack.Where(IsEven);
    Stack<int> sub = stack.GetSubsequence(1, 2);

    assert(mapped.Get(0) == 2);
    assert(mapped.Get(3) == 8);
    assert(filtered.GetSize() == 2);
    assert(filtered.Get(0) == 2);
    assert(filtered.Get(1) == 4);
    assert(sub.GetSize() == 2);
    assert(sub.Get(0) == 2);
    assert(sub.Get(1) == 3);
    assert(stack.Reduce(Sum, 0) == 10);
}

inline void TestStackConcatAndSearch() {
    int leftData[] = {1, 2, 3};
    int rightData[] = {4, 5};
    int patternData[] = {2, 3, 4};

    Stack<int> left(leftData, 3);
    Stack<int> right(rightData, 2);
    Stack<int> pattern(patternData, 3);

    Stack<int> joined = left.Concat(right);
    assert(joined.GetSize() == 5);
    assert(joined.Top() == 5);
    assert(joined.FindSubsequence(pattern) == 1);
}

inline void TestStackExceptions() {
    Stack<int> stack;
    bool thrown = false;
    try {
        stack.Pop();
    } catch (const EmptyStructure&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        stack.GetSubsequence(0, 0);
    } catch (const IndexOutOfRange&) {
        thrown = true;
    }
    assert(thrown);
}

inline void TestDequeBasic() {
    Deque<int> deque;
    deque.PushBack(2);
    deque.PushFront(1);
    deque.PushBack(3);

    assert(deque.GetSize() == 3);
    assert(deque.Front() == 1);
    assert(deque.Back() == 3);
    assert(deque.PopFront() == 1);
    assert(deque.PopBack() == 3);
    assert(deque.Front() == 2);
}

inline void TestDequeAlgorithms() {
    int data[] = {5, 1, 4, 2, 3};
    Deque<int> deque(data, 5);

    Deque<int> mapped = deque.Map(Double);
    Deque<int> filtered = deque.Where(IsEven);
    Deque<int> sub = deque.GetSubsequence(1, 3);
    Deque<int> concat = deque.Concat(filtered);

    assert(mapped.Get(0) == 10);
    assert(mapped.Get(4) == 6);
    assert(filtered.GetSize() == 2);
    assert(filtered.Get(0) == 4);
    assert(filtered.Get(1) == 2);
    assert(sub.GetSize() == 3);
    assert(sub.Get(0) == 1);
    assert(sub.Get(2) == 2);
    assert(concat.GetSize() == 7);
    assert(deque.Reduce(Sum, 0) == 15);
}

inline void TestDequeSort() {
    int data[] = {5, 1, 4, 2, 3};
    Deque<int> deque(data, 5);
    deque.Sort();

    for (int i = 0; i < 5; ++i) {
        assert(deque.Get(i) == i + 1);
    }
}

inline void TestDequeExceptions() {
    Deque<int> deque;
    bool thrown = false;
    try {
        deque.PopFront();
    } catch (const EmptyStructure&) {
        thrown = true;
    }
    assert(thrown);

    thrown = false;
    try {
        deque.Get(0);
    } catch (const IndexOutOfRange&) {
        thrown = true;
    }
    assert(thrown);
}

inline void TestPriorityQueueBasic() {
    PriorityQueue<int> queue;
    queue.Push(10, 3);
    queue.Push(20, 1);
    queue.Push(30, 2);

    assert(queue.GetSize() == 3);
    assert(queue.Top() == 20);
    assert(queue.TopPriority() == 1);
    assert(queue.Pop() == 20);
    assert(queue.Top() == 30);
}

inline void RunAllTests() {
    TestDynamicArray();
    TestDynamicArrayExceptions();
    TestLinkedList();
    TestArraySequence();
    TestListSequence();
    TestArraySequenceFrom();
    TestArraySequenceMap();
    TestArraySequenceReduce();
    TestListSequenceFrom();
    TestListSequenceMap();
    TestListSequenceReduce();
    TestConcat();
    TestClone();
    TestImmutableArraySequence();
    TestImmutableArraySequenceMapReduce();
    TestMutableVsImmutableDifference();
    TestStackBasic();
    TestStackAlgorithms();
    TestStackConcatAndSearch();
    TestStackExceptions();
    TestDequeBasic();
    TestDequeAlgorithms();
    TestDequeSort();
    TestDequeExceptions();
    TestPriorityQueueBasic();
    std::cout << "All tests passed!\n";
}
