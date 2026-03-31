#pragma once

#include <iostream>
#include "Stack.h"
#include "DynamicArray.h"
#include "Exceptions.h"

struct HanoiItem {
    int size;
    int itemType;
    int color;
};

inline int ReadHanoiInt(const char* prompt) {
    int value = 0;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

inline void PrintHanoiItem(const HanoiItem& item) {
    std::cout << "{item=" << item.itemType
              << ", color=" << item.color
              << ", size=" << item.size << "}";
}

inline void PrintHanoiState(const Stack<HanoiItem> rods[3]) {
    for (int i = 0; i < 3; ++i) {
        std::cout << "Стержень " << i + 1 << " (bottom -> top): [";
        for (int j = 0; j < rods[i].GetSize(); ++j) {
            if (j > 0) {
                std::cout << ", ";
            }
            PrintHanoiItem(rods[i].Get(j));
        }
        std::cout << "]\n";
    }
}

inline void MoveHanoiItem(Stack<HanoiItem> rods[3], int from, int to, int& moveCount) {
    HanoiItem item = rods[from].Pop();

    if (!rods[to].IsEmpty() && rods[to].Top().size < item.size) {
        throw EmptyStructure("Нельзя класть больший предмет на меньший");
    }

    rods[to].Push(item);
    ++moveCount;

    std::cout << "Ход " << moveCount << ": "
              << from + 1 << " -> " << to + 1 << '\n';
    PrintHanoiState(rods);
    std::cout << '\n';
}

inline void SolveHanoi(int count, int from, int to, int buffer, Stack<HanoiItem> rods[3], int& moveCount) {
    if (count == 0) {
        return;
    }

    SolveHanoi(count - 1, from, buffer, to, rods, moveCount);
    MoveHanoiItem(rods, from, to, moveCount);
    SolveHanoi(count - 1, buffer, to, from, rods, moveCount);
}

inline void DemoHanoi() {
    std::cout << "\n=== Hanoi Tower ===\n";

    const int count = ReadHanoiInt("Введите количество предметов: ");
    if (count < 0) {
        throw EmptyStructure("Количество предметов не может быть отрицательным");
    }

    const int startRodNumber = ReadHanoiInt("Введите номер начального стержня (1..3): ");
    if (startRodNumber < 1 || startRodNumber > 3) {
        throw IndexOutOfRange();
    }

    DynamicArray<HanoiItem> items(count);

    std::cout << "Введите предметы сверху вниз.\n";
    std::cout << "Предмет и цвет задаются числами.\n";

    for (int i = 0; i < count; ++i) {
        HanoiItem item;
        item.size = i + 1;
        item.itemType = ReadHanoiInt("Введите код предмета: ");
        item.color = ReadHanoiInt("Введите код цвета: ");
        items.Set(i, item);
    }

    Stack<HanoiItem> rods[3];
    for (int i = count - 1; i >= 0; --i) {
        rods[startRodNumber - 1].Push(items.Get(i));
    }

    std::cout << "Начальное состояние:\n";
    PrintHanoiState(rods);
    std::cout << '\n';

    const int from = startRodNumber - 1;
    const int to = (from + 1) % 3;
    const int buffer = 3 - from - to;

    int moveCount = 0;
    SolveHanoi(count, from, to, buffer, rods, moveCount);

    std::cout << "Всего ходов: " << moveCount << '\n';
}