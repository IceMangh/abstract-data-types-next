#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "MutableArraySequence.h"
#include "ImmutableArraySequence.h"
#include "MutableListSequence.h"
#include "ImmutableListSequence.h"
#include "Stack.h"
#include "Deque.h"
#include "Hanoi.h"
#include "Tests.h"
#include "PriorityQueue.h"

int ReadInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }

        std::cout << "Ошибка ввода. Нужно целое число.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

std::vector<int> ReadElements(const std::string& name = "последовательности") {
    const int length = ReadInt("Введите длину " + name + ": ");
    if (length < 0) {
        throw std::invalid_argument("Длина не может быть отрицательной");
    }

    std::vector<int> values(length);
    for (int i = 0; i < length; ++i) {
        values[i] = ReadInt("Элемент [" + std::to_string(i) + "]: ");
    }

    return values;
}

template <class SequenceType>
SequenceType BuildSequence(const std::vector<int>& values) {
    if (values.empty()) {
        return SequenceType();
    }

    return SequenceType(values.data(), static_cast<int>(values.size()));
}

template <class SequenceType>
void PrintSequence(const SequenceType& seq, const std::string& title) {
    std::cout << title << ": [";
    for (int i = 0; i < seq.GetLength(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << seq.Get(i);
    }
    std::cout << "]\n";
}

void PrintStack(const Stack<int>& stack, const std::string& title) {
    std::cout << title << " (bottom -> top): [";
    for (int i = 0; i < stack.GetSize(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << stack.Get(i);
    }
    std::cout << "]\n";
}

void PrintDeque(const Deque<int>& deque, const std::string& title) {
    std::cout << title << ": [";
    for (int i = 0; i < deque.GetSize(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << deque.Get(i);
    }
    std::cout << "]\n";
}

template <class SequenceType>
void PrintCommonInfo(const SequenceType& seq) {
    std::cout << "Длина: " << seq.GetLength() << '\n';
    if (seq.GetLength() == 0) {
        std::cout << "Последовательность пустая\n";
        return;
    }

    std::cout << "Первый элемент: " << seq.GetFirst() << '\n';
    std::cout << "Последний элемент: " << seq.GetLast() << '\n';
}

void PrintPriorityQueue(const PriorityQueue<int>& queue, const std::string& title) {
    std::cout << title << ": [";
    for (int i = 0; i < queue.GetSize(); ++i) {
        if (i > 0) {
            std::cout << ", ";
        }
        std::cout << "(" << queue.GetValue(i)
                  << ", p=" << queue.GetPriority(i) << ")";
    }
    std::cout << "]\n";
}

void ShowAlgorithms(const Sequence<int>& seq) {
    Sequence<int>* mapped = seq.Map(Double);
    PrintSequence(*mapped, "Map(x2)");
    delete mapped;

    std::cout << "Reduce(sum): " << seq.Reduce(Sum, 0) << '\n';

    if (seq.GetLength() >= 2) {
        Sequence<int>* sub = seq.GetSubsequence(0, seq.GetLength() - 2);
        PrintSequence(*sub, "Subsequence(0, n-2)");
        delete sub;
    }
}

void DemoMutableArraySequence() {
    std::cout << "\n=== Mutable ArraySequence ===\n";
    MutableArraySequence<int> sequence = BuildSequence<MutableArraySequence<int>>(ReadElements());
    PrintSequence(sequence, "Исходная последовательность");

    const int appendValue = ReadInt("Значение для Append: ");
    sequence.Append(appendValue);
    PrintSequence(sequence, "После Append");

    const int prependValue = ReadInt("Значение для Prepend: ");
    sequence.Prepend(prependValue);
    PrintSequence(sequence, "После Prepend");

    const int insertValue = ReadInt("Значение для InsertAt: ");
    const int insertIndex = ReadInt("Индекс для InsertAt (0.." + std::to_string(sequence.GetLength()) + "): ");
    sequence.InsertAt(insertValue, insertIndex);
    PrintSequence(sequence, "После InsertAt");

    PrintCommonInfo(sequence);
    ShowAlgorithms(sequence);
    std::cout << "Вывод: mutable-версия меняет исходный объект.\n";
}

void DemoImmutableArraySequence() {
    std::cout << "\n=== Immutable ArraySequence ===\n";
    ImmutableArraySequence<int> original = BuildSequence<ImmutableArraySequence<int>>(ReadElements());
    PrintSequence(original, "Исходная последовательность");

    const int appendValue = ReadInt("Значение для Append: ");
    Sequence<int>* appended = original.Append(appendValue);
    PrintSequence(original, "Исходный объект после Append");
    PrintSequence(*appended, "Новый объект после Append");

    const int prependValue = ReadInt("Значение для Prepend: ");
    Sequence<int>* prepended = appended->Prepend(prependValue);
    PrintSequence(*appended, "Предыдущий объект после Prepend");
    PrintSequence(*prepended, "Новый объект после Prepend");

    const int insertValue = ReadInt("Значение для InsertAt: ");
    const int insertIndex = ReadInt("Индекс для InsertAt (0.." + std::to_string(prepended->GetLength()) + "): ");
    Sequence<int>* inserted = prepended->InsertAt(insertValue, insertIndex);
    PrintSequence(*prepended, "Предыдущий объект после InsertAt");
    PrintSequence(*inserted, "Новый объект после InsertAt");

    PrintCommonInfo(*inserted);
    ShowAlgorithms(*inserted);
    std::cout << "Вывод: immutable-версия не меняет исходный объект, а возвращает новую последовательность.\n";

    delete appended;
    delete prepended;
    delete inserted;
}

void DemoMutableListSequence() {
    std::cout << "\n=== Mutable ListSequence ===\n";
    MutableListSequence<int> sequence = BuildSequence<MutableListSequence<int>>(ReadElements());
    PrintSequence(sequence, "Исходная последовательность");

    const int appendValue = ReadInt("Значение для Append: ");
    sequence.Append(appendValue);
    PrintSequence(sequence, "После Append");

    const int prependValue = ReadInt("Значение для Prepend: ");
    sequence.Prepend(prependValue);
    PrintSequence(sequence, "После Prepend");

    const int insertValue = ReadInt("Значение для InsertAt: ");
    const int insertIndex = ReadInt("Индекс для InsertAt (0.." + std::to_string(sequence.GetLength()) + "): ");
    sequence.InsertAt(insertValue, insertIndex);
    PrintSequence(sequence, "После InsertAt");

    PrintCommonInfo(sequence);
    ShowAlgorithms(sequence);
    std::cout << "Вывод: mutable-версия меняет исходный объект.\n";
}

void DemoImmutableListSequence() {
    std::cout << "\n=== Immutable ListSequence ===\n";
    ImmutableListSequence<int> original = BuildSequence<ImmutableListSequence<int>>(ReadElements());
    PrintSequence(original, "Исходная последовательность");

    const int appendValue = ReadInt("Значение для Append: ");
    Sequence<int>* appended = original.Append(appendValue);
    PrintSequence(original, "Исходный объект после Append");
    PrintSequence(*appended, "Новый объект после Append");

    const int prependValue = ReadInt("Значение для Prepend: ");
    Sequence<int>* prepended = appended->Prepend(prependValue);
    PrintSequence(*appended, "Предыдущий объект после Prepend");
    PrintSequence(*prepended, "Новый объект после Prepend");

    const int insertValue = ReadInt("Значение для InsertAt: ");
    const int insertIndex = ReadInt("Индекс для InsertAt (0.." + std::to_string(prepended->GetLength()) + "): ");
    Sequence<int>* inserted = prepended->InsertAt(insertValue, insertIndex);
    PrintSequence(*prepended, "Предыдущий объект после InsertAt");
    PrintSequence(*inserted, "Новый объект после InsertAt");

    PrintCommonInfo(*inserted);
    ShowAlgorithms(*inserted);
    std::cout << "Вывод: immutable-версия не меняет исходный объект, а возвращает новую последовательность.\n";

    delete appended;
    delete prepended;
    delete inserted;
}

void DemoStack() {
    std::cout << "\n=== Stack ===\n";
    Stack<int> stack = BuildSequence<Stack<int>>(ReadElements("стека"));
    PrintStack(stack, "Исходный стек");

    const int pushValue = ReadInt("Значение для Push: ");
    stack.Push(pushValue);
    PrintStack(stack, "После Push");
    std::cout << "Top: " << stack.Top() << '\n';

    std::cout << "Pop: " << stack.Pop() << '\n';
    PrintStack(stack, "После Pop");

    Stack<int> mapped = stack.Map(Double);
    PrintStack(mapped, "Map(x2)");

    Stack<int> filtered = stack.Where(IsEven);
    PrintStack(filtered, "Where(even)");

    std::cout << "Reduce(sum): " << stack.Reduce(Sum, 0) << '\n';

    if (stack.GetSize() >= 2) {
        const int start = ReadInt("Начальный индекс подпоследовательности: ");
        const int end = ReadInt("Конечный индекс подпоследовательности: ");
        Stack<int>* sub = stack.GetSubsequence(start, end);
        PrintStack(*sub, "Извлеченная подпоследовательность");
        delete sub;
    }

    std::cout << "--- Конкатенация и поиск подпоследовательности ---\n";
    Stack<int> second = BuildSequence<Stack<int>>(ReadElements("второго стека"));
    Stack<int> joined = stack.Concat(second);
    PrintStack(joined, "Concat");

    Stack<int> pattern = BuildSequence<Stack<int>>(ReadElements("шаблона для поиска"));
    std::cout << "Индекс вхождения шаблона: " << joined.FindSubsequence(pattern) << '\n';
}

void DemoDeque() {
    std::cout << "\n=== Deque ===\n";
    Deque<int> deque = BuildSequence<Deque<int>>(ReadElements("дека"));
    PrintDeque(deque, "Исходный дек");

    const int pushFrontValue = ReadInt("Значение для PushFront: ");
    deque.PushFront(pushFrontValue);
    PrintDeque(deque, "После PushFront");

    const int pushBackValue = ReadInt("Значение для PushBack: ");
    deque.PushBack(pushBackValue);
    PrintDeque(deque, "После PushBack");

    std::cout << "PopFront: " << deque.PopFront() << '\n';
    std::cout << "PopBack: " << deque.PopBack() << '\n';
    PrintDeque(deque, "После извлечений");

    Deque<int> mapped = deque.Map(Double);
    PrintDeque(mapped, "Map(x2)");

    Deque<int> filtered = deque.Where(IsEven);
    PrintDeque(filtered, "Where(even)");

    std::cout << "Reduce(sum): " << deque.Reduce(Sum, 0) << '\n';

    if (deque.GetSize() >= 2) {
        const int start = ReadInt("Начальный индекс извлечения: ");
        const int end = ReadInt("Конечный индекс извлечения: ");
        Deque<int>* sub = deque.GetSubsequence(start, end);
        PrintDeque(*sub, "Извлеченная часть дека");
        delete sub;
    }

    Deque<int> second = BuildSequence<Deque<int>>(ReadElements("второго дека"));
    Deque<int> joined = deque.Concat(second);
    PrintDeque(joined, "Concat");

    joined.Sort();
    PrintDeque(joined, "После Sort");
}

void DemoPriorityQueue() {
    std::cout << "\n=== PriorityQueue ===\n";

    PriorityQueue<int> queue;
    const int count = ReadInt("Введите количество элементов: ");
    if (count < 0) {
        throw std::invalid_argument("Количество элементов не может быть отрицательным");
    }

    for (int i = 0; i < count; ++i) {
        const int value = ReadInt("Введите значение: ");
        const int priority = ReadInt("Введите приоритет: ");
        queue.Push(value, priority);
    }

    PrintPriorityQueue(queue, "Очередь после заполнения");

    const int pushValue = ReadInt("Значение для Push: ");
    const int pushPriority = ReadInt("Приоритет для Push: ");
    queue.Push(pushValue, pushPriority);
    PrintPriorityQueue(queue, "После Push");

    if (!queue.IsEmpty()) {
        std::cout << "Top: " << queue.Top()
                  << " (priority=" << queue.TopPriority() << ")\n";
    }

    if (!queue.IsEmpty()) {
        std::cout << "Pop: " << queue.Pop() << '\n';
        PrintPriorityQueue(queue, "После Pop");
    }

    if (!queue.IsEmpty()) {
        std::cout << "Top после Pop: " << queue.Top()
                  << " (priority=" << queue.TopPriority() << ")\n";
    }
}

void PrintMenu() {
    std::cout << "\n========== МЕНЮ ==========\n";
    std::cout << "1. Запустить тесты\n";
    std::cout << "2. Демонстрация mutable ArraySequence\n";
    std::cout << "3. Демонстрация immutable ArraySequence\n";
    std::cout << "4. Демонстрация mutable ListSequence\n";
    std::cout << "5. Демонстрация immutable ListSequence\n";
    std::cout << "6. Демонстрация Stack\n";
    std::cout << "7. Демонстрация Deque\n";
    std::cout << "8. Демонстрация PriorityQueue\n";
    std::cout << "9. Демонстрация Ханойской башни\n";
    std::cout << "0. Выход\n";
}

int main() {
    bool isRunning = true;

    while (isRunning) {
        try {
            PrintMenu();
            const int choice = ReadInt("Выберите пункт: ");

            switch (choice) {
                case 1:
                    RunAllTests();
                    break;
                case 2:
                    DemoMutableArraySequence();
                    break;
                case 3:
                    DemoImmutableArraySequence();
                    break;
                case 4:
                    DemoMutableListSequence();
                    break;
                case 5:
                    DemoImmutableListSequence();
                    break;
                case 6:
                    DemoStack();
                    break;
                case 7:
                    DemoDeque();
                    break;
                case 8:
                    DemoPriorityQueue();
                    break;
                case 9:
                    DemoHanoi();
                    break;
                case 0:
                    isRunning = false;
                    break;
                default:
                    std::cout << "Неизвестный пункт меню\n";
                    break;
            }
        } catch (const std::exception& error) {
            std::cout << "Ошибка: " << error.what() << '\n';
        }
    }

    return 0;
}
