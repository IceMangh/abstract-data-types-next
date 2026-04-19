#pragma once

#include <iostream>
#include "Stack.h"
#include "DynamicArray.h"
#include "Exceptions.h"

enum HanoiItemType {
    ITEM_RING = 1,
    ITEM_BOOK = 2,
    ITEM_BOX = 3,
    ITEM_DISC = 4
};

enum HanoiColor {
    COLOR_RED = 1,
    COLOR_BLUE = 2,
    COLOR_GREEN = 3,
    COLOR_YELLOW = 4
};

struct HanoiItem {
    int size;
    HanoiItemType ItemType;
    HanoiColor Color;
};

inline int ReadHanoiInt(const char* prompt) {
    int value = 0;
    std::cout << prompt;
    std::cin >> value;
    return value;
}

inline const char* GetItemTypeName(HanoiItemType type) {
    switch (type) {
        case ITEM_RING: return "Ring";
        case ITEM_BOOK: return "Book";
        case ITEM_BOX: return "Box";
        case ITEM_DISC: return "Disc";
        default: return "Unknown";
    }
}

inline const char* GetColorName(HanoiColor color) {
    switch (color) {
        case COLOR_RED: return "Red";
        case COLOR_BLUE: return "Blue";
        case COLOR_GREEN: return "Green";
        case COLOR_YELLOW: return "Yellow";
        default: return "Unknown";
    }
}

inline const char* GetAnsiColor(HanoiColor color) {
    switch (color) {
        case COLOR_RED: return "\033[31m";
        case COLOR_BLUE: return "\033[34m";
        case COLOR_GREEN: return "\033[32m";
        case COLOR_YELLOW: return "\033[33m";
        default: return "\033[0m";
    }
}

inline const char* GetAnsiReset() {
    return "\033[0m";
}

inline const char* GetItemSymbol(HanoiItemType type) {
    switch (type) {
        case ITEM_RING: return "O";
        case ITEM_BOOK: return "B";
        case ITEM_BOX: return "#";
        case ITEM_DISC: return "@";
        default: return "?";
    }
}

inline void PrintItemTypes() {
    std::cout << "Доступные предметы:\n";
    std::cout << "1. Ring\n";
    std::cout << "2. Book\n";
    std::cout << "3. Box\n";
    std::cout << "4. Disc\n";
}

inline void PrintColors() {
    std::cout << "Доступные цвета:\n";
    std::cout << "1. Red\n";
    std::cout << "2. Blue\n";
    std::cout << "3. Green\n";
    std::cout << "4. Yellow\n";
}

inline HanoiItemType ReadItemType() {
    while (true) {
        PrintItemTypes();
        int value = ReadHanoiInt("Выберите предмет: ");
        if (value >= 1 && value <= 4) {
            return static_cast<HanoiItemType>(value);
        }
        std::cout << "Неверный выбор.\n";
    }
}

inline HanoiColor ReadColor() {
    while (true) {
        PrintColors();
        int value = ReadHanoiInt("Выберите цвет: ");
        if (value >= 1 && value <= 4) {
            return static_cast<HanoiColor>(value);
        }
        std::cout << "Неверный выбор.\n";
    }
}

inline void PrintCenteredItem(const HanoiItem& item, int maxWidth) {
    int width = item.size * 2 + 1;
    int spaces = (maxWidth - width) / 2;

    for (int i = 0; i < spaces; ++i) {
        std::cout << ' ';
    }

    std::cout << GetAnsiColor(item.Color);

    for (int i = 0; i < width; ++i) {
        if (i == width / 2) {
            std::cout << GetItemSymbol(item.ItemType);
        } else {
            std::cout << '=';
        }
    }

    std::cout << GetAnsiReset();

    for (int i = 0; i < spaces; ++i) {
        std::cout << ' ';
    }
}

inline void PrintEmptyLevel(int maxWidth) {
    for (int i = 0; i < maxWidth / 2; ++i) {
        std::cout << ' ';
    }
    std::cout << '|';
    for (int i = 0; i < maxWidth / 2; ++i) {
        std::cout << ' ';
    }
}

inline void PrintRodVisual(const Stack<HanoiItem>& rod, int level, int maxHeight, int maxWidth) {
    int visibleStart = maxHeight - rod.GetSize();

    if (level < visibleStart) {
        PrintEmptyLevel(maxWidth);
        return;
    }

    int relativeLevel = level - visibleStart;
    int itemIndex = rod.GetSize() - 1 - relativeLevel;

    const HanoiItem& item = rod.Get(itemIndex);
    PrintCenteredItem(item, maxWidth);
}

inline void PrintHanoiState(const Stack<HanoiItem> rods[3], int maxHeight) {
    int maxWidth = maxHeight * 2 + 3;

    std::cout << "\nТекущее состояние башни:\n\n";

    for (int level = 0; level < maxHeight; ++level) {
        for (int rod = 0; rod < 3; ++rod) {
            PrintRodVisual(rods[rod], level, maxHeight, maxWidth);
            std::cout << "   ";
        }
        std::cout << '\n';
    }

    for (int rod = 0; rod < 3; ++rod) {
        for (int i = 0; i < maxWidth; ++i) {
            std::cout << '-';
        }
        std::cout << "   ";
    }
    std::cout << '\n';

    for (int rod = 0; rod < 3; ++rod) {
        for (int i = 0; i < maxWidth / 2; ++i) {
            std::cout << ' ';
        }
        std::cout << rod + 1;
        for (int i = 0; i < maxWidth / 2; ++i) {
            std::cout << ' ';
        }
        std::cout << "   ";
    }
    std::cout << "\n\n";

    std::cout << "Обозначения: "
              << GetAnsiColor(COLOR_RED) << "O" << GetAnsiReset() << "=Ring, "
              << GetAnsiColor(COLOR_BLUE) << "B" << GetAnsiReset() << "=Book, "
              << GetAnsiColor(COLOR_GREEN) << "#" << GetAnsiReset() << "=Box, "
              << GetAnsiColor(COLOR_YELLOW) << "@" << GetAnsiReset() << "=Disc\n\n";
}

inline void MoveHanoiItem(Stack<HanoiItem> rods[3], int from, int to, int& moveCount, int maxHeight) {
    HanoiItem item = rods[from].Pop();

    if (!rods[to].IsEmpty() && rods[to].Top().size < item.size) {
        throw EmptyStructure("Нельзя класть больший предмет на меньший");
    }

    rods[to].Push(item);
    ++moveCount;

    std::cout << "Ход " << moveCount << ": "
              << GetItemTypeName(item.ItemType) << " "
              << GetColorName(item.Color)
              << " размера " << item.size
              << " : " << from + 1 << " -> " << to + 1 << "\n";

    PrintHanoiState(rods, maxHeight);
}

inline void SolveHanoi(int count, int from, int to, int buffer, Stack<HanoiItem> rods[3], int& moveCount, int maxHeight) {
    if (count == 0) {
        return;
    }

    SolveHanoi(count - 1, from, buffer, to, rods, moveCount, maxHeight);
    MoveHanoiItem(rods, from, to, moveCount, maxHeight);
    SolveHanoi(count - 1, buffer, to, from, rods, moveCount, maxHeight);
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

    std::cout << "\nВведите предметы сверху вниз.\n";
    std::cout << "Размер назначается автоматически: 1, 2, 3, ...\n\n";

    for (int i = 0; i < count; ++i) {
        std::cout << "Предмет " << i + 1 << ":\n";

        HanoiItem item;
        item.size = i + 1;
        item.ItemType = ReadItemType();
        item.Color = ReadColor();

        items.Set(i, item);
        std::cout << '\n';
    }

    Stack<HanoiItem> rods[3];
    for (int i = count - 1; i >= 0; --i) {
        rods[startRodNumber - 1].Push(items.Get(i));
    }

    std::cout << "Начальное состояние:\n";
    PrintHanoiState(rods, count);

    const int from = startRodNumber - 1;
    const int to = (from == 2) ? 1 : 2;
    const int buffer = 3 - from - to;

    int moveCount = 0;
    SolveHanoi(count, from, to, buffer, rods, moveCount, count);

    std::cout << "Всего ходов: " << moveCount << '\n';
}