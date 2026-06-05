#include "Tests.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "DynamicArray.h"
#include "LinkedList.h"
#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"
#include "Stack.h"
#include "Deque.h"
#include "PriorityQueue.h"
#include "Hanoi.h"

static int Double(int x) {
    return x * 2;
}

static bool IsEven(int x) {
    return x % 2 == 0;
}

static int Sum(int a, int b) {
    return a + b;
}

static int Multiply(int a, int b) {
    return a * b;
}

static bool Greater(int a, int b) {
    return a > b;
}

class CoutSilencer {
private:
    std::ostringstream buffer_;
    std::streambuf* original_;

public:
    CoutSilencer() : original_(std::cout.rdbuf(buffer_.rdbuf())) {}

    ~CoutSilencer() {
        std::cout.rdbuf(original_);
    }
};

class CinRedirect {
private:
    std::istringstream input_;
    std::streambuf* original_;

public:
    explicit CinRedirect(const std::string& text)
            : input_(text), original_(std::cin.rdbuf(input_.rdbuf())) {
        std::cin.clear();
    }

    ~CinRedirect() {
        std::cin.rdbuf(original_);
        std::cin.clear();
    }
};

template <class Exception, class Action>
void ExpectThrows(Action action) {
    bool thrown = false;
    try {
        action();
    } catch (const Exception&) {
        thrown = true;
    }
    assert(thrown);
}

void TestDynamicArray() {
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

void TestDynamicArrayExceptions() {
    DynamicArray<int> arr(2);
    ExpectThrows<IndexOutOfRange>([&arr]() {
        arr.Get(3);
    });
    ExpectThrows<IndexOutOfRange>([&arr]() {
        arr.Set(-1, 10);
    });
}

void TestLinkedList() {
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

    list.Set(2, 100);
    assert(list.Get(2) == 100);

    list[2] = 99;
    assert(list.Get(2) == 99);
}

void TestLinkedListNewOperations() {
    int data[] = {1, 2, 3, 4};
    LinkedList<int> list(data, 4);

    assert(!list.IsEmpty());
    assert(list.RemoveFirst() == 1);
    assert(list.RemoveLast() == 4);
    assert(list.GetLength() == 2);
    assert(list.GetFirst() == 2);
    assert(list.GetLast() == 3);

    list.Prepend(1);
    list.Append(4);
    LinkedList<int>* sub = list.GetSubList(1, 2);
    assert(sub->GetLength() == 2);
    assert(sub->Get(0) == 2);
    assert(sub->Get(1) == 3);

    LinkedList<int> copy = list;
    list.Set(1, 20);
    assert(copy.Get(1) == 2);
    assert(list.Get(1) == 20);

    list.Clear();
    assert(list.IsEmpty());
    ExpectThrows<EmptyStructure>([&list]() {
        list.GetFirst();
    });

    delete sub;
}

void TestArraySequence() {
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

void TestArraySequenceMap() {
    int data[] = {1, 2, 32};
    MutableArraySequence<int> seq(data, 3);
    Sequence<int>* mapped = seq.Map(Double);

    assert(mapped->GetLength() == 3);
    assert(mapped->Get(0) == 2);
    assert(mapped->Get(1) == 4);
    assert(mapped->Get(2) == 64);
    delete mapped;
}

void TestArraySequenceFrom() {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq = MutableArraySequence<int>::From(data, 3);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 1);
    assert(seq.Get(1) == 2);
    assert(seq.Get(2) == 3);
}

void TestArraySequenceReduce() {
    int data[] = {1, 2, 3, 4};
    MutableArraySequence<int> seq(data, 4);

    int sumResult = seq.Reduce(Sum, 0);
    int mulResult = seq.Reduce(Multiply, 1);

    assert(sumResult == 10);
    assert(mulResult == 24);
}

void TestMutableListSequence() {
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq(data, 3);
    seq.Append(4)->Prepend(0)->InsertAt(99, 2);
    assert(seq.GetFirst() == 0);
    assert(seq.Get(2) == 99);
    assert(seq.GetLast() == 4);

    Sequence<int>* sub = seq.GetSubsequence(1, 3);
    assert(sub->GetLength() == 3);
    assert(sub->Get(0) == 1);
    delete sub;
}

void TestMutableListSequenceMap() {
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq(data, 3);
    Sequence<int>* mapped = seq.Map(Double);

    assert(mapped->GetLength() == 3);
    assert(mapped->Get(0) == 2);
    assert(mapped->Get(1) == 4);
    assert(mapped->Get(2) == 6);

    delete mapped;
}

void TestMutableListSequenceFrom() {
    int data[] = {1, 2, 3};
    MutableListSequence<int> seq = MutableListSequence<int>::From(data, 3);

    assert(seq.GetLength() == 3);
    assert(seq.Get(0) == 1);
    assert(seq.Get(1) == 2);
    assert(seq.Get(2) == 3);
}

void TestMutableListSequenceReduce() {
    int data[] = {1, 2, 3, 4};
    MutableListSequence<int> seq(data, 4);

    int sumResult = seq.Reduce(Sum, 0);
    int mulResult = seq.Reduce(Multiply, 1);

    assert(sumResult == 10);
    assert(mulResult == 24);
}

void TestImmutableListSequence() {
    int data[] = {1, 2, 3};
    ImmutableListSequence<int> original(data, 3);

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

void TestImmutableListSequenceMapReduce() {
    int data[] = {1, 2, 3, 4};
    ImmutableListSequence<int> seq(data, 4);

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

void TestMutableVsImmutableListDifference() {
    int data[] = {1, 2, 3};
    MutableListSequence<int> mutableSeq(data, 3);
    ImmutableListSequence<int> immutableSeq(data, 3);

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

void TestConcat() {
    int a[] = {1, 2};
    int b[] = {3, 4};

    MutableArraySequence<int> seq1(a, 2);
    ImmutableListSequence<int> seq2(b, 2);

    Sequence<int>* joined = seq1 + seq2;
    assert(joined->GetLength() == 4);
    assert(joined->Get(0) == 1);
    assert(joined->Get(1) == 2);
    assert(joined->Get(2) == 3);
    assert(joined->Get(3) == 4);

    delete joined;
}

void TestClone() {
    int data[] = {1, 2, 3};
    MutableArraySequence<int> seq(data, 3);

    Sequence<int>* copy = seq.Clone();

    assert(copy->GetLength() == 3);
    assert(copy->Get(0) == 1);
    assert(copy->Get(1) == 2);
    assert(copy->Get(2) == 3);

    delete copy;
}

void TestImmutableArraySequence() {
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

void TestImmutableArraySequenceMapReduce() {
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

void TestMutableVsImmutableDifference() {
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

void TestStackBasic() {
    Stack<int> stack;
    assert(stack.IsEmpty());

    stack.Push(1);
    stack.Push(2);
    stack.Push(3);

    assert(stack.GetSize() == 3);
    assert(!stack.IsEmpty());
    assert(stack.Top() == 3);
    assert(stack.Get(0) == 1);
    assert(stack.Get(1) == 2);
    assert(stack.Get(2) == 3);
    assert(stack.Pop() == 3);
    assert(stack.Top() == 2);
    assert(stack.GetSize() == 2);
}

void TestStackAlgorithms() {
    int data[] = {1, 2, 3, 4};
    Stack<int> stack(data, 4);

    Stack<int> mapped = stack.Map(Double);
    Stack<int> filtered = stack.Where(IsEven);
    Stack<int>* sub = stack.GetSubsequence(1, 2);
    Stack<int> noMatches = stack.Where([](int value) {
        return value > 10;
    });

    assert(mapped.Get(0) == 2);
    assert(mapped.Get(3) == 8);
    assert(filtered.GetSize() == 2);
    assert(filtered.Get(0) == 2);
    assert(filtered.Get(1) == 4);
    assert(sub->GetSize() == 2);
    assert(sub->Get(0) == 2);
    assert(sub->Get(1) == 3);
    assert(noMatches.IsEmpty());
    assert(stack.Reduce(Sum, 0) == 10);
    assert(stack.Reduce(Multiply, 1) == 24);

    delete sub;
}

void TestStackConcatAndSearch() {
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
    assert(left.GetSize() == 3);
    assert(right.GetSize() == 2);
    assert(joined.Get(3) == 4);
    assert(joined.Get(4) == 5);
}

void TestStackSubsequenceSearchExamples() {
    int data[] = {1, 2, 3, 2, 3, 4};
    int repeatedPatternData[] = {2, 3};
    int missingPatternData[] = {3, 5};
    int longPatternData[] = {1, 2, 3, 4, 5, 6, 7};

    Stack<int> stack(data, 6);
    Stack<int> repeatedPattern(repeatedPatternData, 2);
    Stack<int> missingPattern(missingPatternData, 2);
    Stack<int> longPattern(longPatternData, 7);
    Stack<int> emptyPattern;

    assert(stack.FindSubsequence(repeatedPattern) == 1);
    assert(stack.FindSubsequence(missingPattern) == -1);
    assert(stack.FindSubsequence(longPattern) == -1);
    assert(stack.FindSubsequence(emptyPattern) == 0);

    Stack<int> copy = stack;
    assert(copy == stack);
    copy.Pop();
    assert(copy != stack);
}

void TestStackExceptions() {
    Stack<int> stack;
    ExpectThrows<EmptyStructure>([&stack]() {
        stack.Pop();
    });
    ExpectThrows<EmptyStructure>([&stack]() {
        stack.Top();
    });
    ExpectThrows<IndexOutOfRange>([&stack]() {
        stack.GetSubsequence(0, 0);
    });
    ExpectThrows<IndexOutOfRange>([&stack]() {
        stack.Get(0);
    });
}

void TestDequeBasic() {
    Deque<int> deque;
    assert(deque.IsEmpty());

    deque.PushBack(2);
    deque.PushFront(1);
    deque.PushBack(3);

    assert(deque.GetSize() == 3);
    assert(!deque.IsEmpty());
    assert(deque.Front() == 1);
    assert(deque.Back() == 3);
    assert(deque.Get(0) == 1);
    assert(deque.Get(1) == 2);
    assert(deque.Get(2) == 3);
    assert(deque.PopFront() == 1);
    assert(deque.PopBack() == 3);
    assert(deque.Front() == 2);
    assert(deque.Back() == 2);

    deque.Clear();
    assert(deque.IsEmpty());
}

void TestDequeAlgorithms() {
    int data[] = {5, 1, 4, 2, 3};
    Deque<int> deque(data, 5);

    Deque<int> mapped = deque.Map(Double);
    Deque<int> filtered = deque.Where(IsEven);
    Deque<int>* sub = deque.GetSubsequence(1, 3);
    Deque<int> concat = deque.Concat(filtered);

    assert(mapped.Get(0) == 10);
    assert(mapped.Get(4) == 6);
    assert(filtered.GetSize() == 2);
    assert(filtered.Get(0) == 4);
    assert(filtered.Get(1) == 2);
    assert(sub->GetSize() == 3);
    assert(sub->Get(0) == 1);
    assert(sub->Get(2) == 2);
    assert(concat.GetSize() == 7);
    assert(concat.Get(0) == 5);
    assert(concat.Get(4) == 3);
    assert(concat.Get(5) == 4);
    assert(concat.Get(6) == 2);
    assert(deque.Reduce(Sum, 0) == 15);
    assert(deque.Reduce(Multiply, 1) == 120);

    delete sub;
}

void TestDequeSort() {
    int data[] = {5, 1, 4, 2, 3};
    Deque<int> deque(data, 5);
    deque.Sort();

    for (int i = 0; i < 5; ++i) {
        assert(deque.Get(i) == i + 1);
    }

    deque.Sort(Greater);
    for (int i = 0; i < 5; ++i) {
        assert(deque.Get(i) == 5 - i);
    }
}

void TestDequeSetAndComparison() {
    int data[] = {1, 2, 3};
    Deque<int> first(data, 3);
    Deque<int> second(data, 3);

    assert(first == second);

    first.Set(1, 20);
    assert(first.Get(1) == 20);
    assert(first != second);

    second = first;
    assert(first == second);

    Deque<int>* sub = first.GetSubsequence(0, 1);
    assert(sub->GetSize() == 2);
    assert(sub->Get(0) == 1);
    assert(sub->Get(1) == 20);

    delete sub;
}

void TestDequeExceptions() {
    Deque<int> deque;
    ExpectThrows<EmptyStructure>([&deque]() {
        deque.PopFront();
    });
    ExpectThrows<EmptyStructure>([&deque]() {
        deque.PopBack();
    });
    ExpectThrows<EmptyStructure>([&deque]() {
        deque.Front();
    });
    ExpectThrows<EmptyStructure>([&deque]() {
        deque.Back();
    });
    ExpectThrows<IndexOutOfRange>([&deque]() {
        deque.Get(0);
    });
    ExpectThrows<IndexOutOfRange>([&deque]() {
        deque.GetSubsequence(0, 0);
    });
}

void TestPriorityQueueBasic() {
    PriorityQueue<int> queue;
    queue.Push(10, 3);
    queue.Push(20, 1);
    queue.Push(30, 2);
    queue.Push(40, 1);

    assert(queue.GetSize() == 4);
    assert(queue.Top() == 20);
    assert(queue.TopPriority() == 1);
    assert(queue.GetValue(0) == 20);
    assert(queue.GetPriority(0) == 1);
    assert(queue.GetValue(1) == 40);
    assert(queue.GetPriority(1) == 1);
    assert(queue.GetValue(2) == 30);
    assert(queue.GetPriority(2) == 2);
    assert(queue.GetValue(3) == 10);
    assert(queue.GetPriority(3) == 3);
    assert(queue.Pop() == 20);
    assert(queue.Pop() == 40);
    assert(queue.Top() == 30);
    assert(queue.TopPriority() == 2);
}

void TestPriorityQueueComparisonAndExceptions() {
    PriorityQueue<int> first;
    PriorityQueue<int> second;

    first.Push(1, 2);
    first.Push(2, 1);
    second.Push(1, 2);
    second.Push(2, 1);

    assert(first == second);
    assert(first.Pop() == 2);
    assert(first != second);

    PriorityQueue<int> empty;
    assert(empty.IsEmpty());
    ExpectThrows<EmptyStructure>([&empty]() {
        empty.Top();
    });
    ExpectThrows<EmptyStructure>([&empty]() {
        empty.TopPriority();
    });
    ExpectThrows<EmptyStructure>([&empty]() {
        empty.Pop();
    });
    ExpectThrows<IndexOutOfRange>([&empty]() {
        empty.GetValue(0);
    });
}

void TestHanoiLabels() {
    assert(std::string(GetItemTypeName(ITEM_RING)) == "Ring");
    assert(std::string(GetItemTypeName(ITEM_BOOK)) == "Book");
    assert(std::string(GetColorName(COLOR_RED)) == "Red");
    assert(std::string(GetColorName(COLOR_YELLOW)) == "Yellow");
    assert(std::string(GetItemSymbol(ITEM_BOX)) == "#");
    assert(std::string(GetItemSymbol(ITEM_DISC)) == "@");
}

void TestHanoiInvalidIntegerInput() {
    CoutSilencer silence;
    CinRedirect input("wrong\n3\n");

    assert(ReadHanoiInt("test: ") == 3);
}

void TestHanoiGeneratedItems() {
    DynamicArray<HanoiItem> emptyItems = GenerateHanoiItems(0);
    assert(emptyItems.GetLength() == 0);

    DynamicArray<HanoiItem> items = GenerateHanoiItems(6);
    assert(items.GetLength() == 6);

    for (int i = 0; i < items.GetLength(); ++i) {
        const HanoiItem& item = items.Get(i);
        assert(item.size == i + 1);
        assert(item.itemType >= ITEM_RING);
        assert(item.itemType <= ITEM_DISC);
        assert(item.color >= COLOR_RED);
        assert(item.color <= COLOR_YELLOW);
    }
}

void TestHanoiSolveThreeItems() {
    Stack<HanoiItem> rods[3];
    rods[0].Push({3, ITEM_DISC, COLOR_RED});
    rods[0].Push({2, ITEM_BOX, COLOR_BLUE});
    rods[0].Push({1, ITEM_RING, COLOR_GREEN});

    int moveCount = 0;
    {
        CoutSilencer silence;
        SolveHanoi(3, 0, 2, 1, rods, moveCount, 3);
    }

    assert(moveCount == 7);
    assert(rods[0].IsEmpty());
    assert(rods[1].IsEmpty());
    assert(rods[2].GetSize() == 3);
    assert(rods[2].Get(0).size == 3);
    assert(rods[2].Get(1).size == 2);
    assert(rods[2].Get(2).size == 1);
}

void TestHanoiInvalidMoveKeepsState() {
    Stack<HanoiItem> rods[3];
    rods[0].Push({2, ITEM_DISC, COLOR_RED});
    rods[1].Push({1, ITEM_RING, COLOR_BLUE});

    int moveCount = 0;
    ExpectThrows<EmptyStructure>([&rods, &moveCount]() {
        MoveHanoiItem(rods, 0, 1, moveCount, 2);
    });

    assert(moveCount == 0);
    assert(rods[0].GetSize() == 1);
    assert(rods[0].Top().size == 2);
    assert(rods[1].GetSize() == 1);
    assert(rods[1].Top().size == 1);
}

void RunAllTests() {
    TestDynamicArray();
    TestDynamicArrayExceptions();
    TestLinkedList();
    TestLinkedListNewOperations();
    TestArraySequence();
    TestMutableListSequence();
    TestArraySequenceFrom();
    TestArraySequenceMap();
    TestArraySequenceReduce();
    TestMutableListSequenceFrom();
    TestMutableListSequenceMap();
    TestMutableListSequenceReduce();
    TestConcat();
    TestClone();
    TestImmutableArraySequence();
    TestImmutableArraySequenceMapReduce();
    TestMutableVsImmutableDifference();
    TestImmutableListSequence();
    TestImmutableListSequenceMapReduce();
    TestMutableVsImmutableListDifference();
    TestStackBasic();
    TestStackAlgorithms();
    TestStackConcatAndSearch();
    TestStackSubsequenceSearchExamples();
    TestStackExceptions();
    TestDequeBasic();
    TestDequeAlgorithms();
    TestDequeSort();
    TestDequeSetAndComparison();
    TestDequeExceptions();
    TestPriorityQueueBasic();
    TestPriorityQueueComparisonAndExceptions();
    TestHanoiLabels();
    TestHanoiInvalidIntegerInput();
    TestHanoiGeneratedItems();
    TestHanoiSolveThreeItems();
    TestHanoiInvalidMoveKeepsState();
    std::cout << "All tests passed!\n";
}
