#pragma once

#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <thread>
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
    HanoiItemType itemType;
    HanoiColor color;
};

inline int ReadHanoiInt(const char* prompt) {
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

inline int ReadHanoiInputMode() {
    while (true) {
        std::cout << "\nКак задать предметы?\n";
        std::cout << "1. Ввести вручную\n";
        std::cout << "2. Сгенерировать автоматически\n";

        int value = ReadHanoiInt("Выберите режим: ");
        if (value == 1 || value == 2) {
            return value;
        }
        std::cout << "Неверный выбор.\n";
    }
}

inline HanoiItem GenerateHanoiItem(int size) {
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> itemTypeDistribution(ITEM_RING, ITEM_DISC);
    std::uniform_int_distribution<int> colorDistribution(COLOR_RED, COLOR_YELLOW);

    HanoiItem item;
    item.size = size;
    item.itemType = static_cast<HanoiItemType>(itemTypeDistribution(generator));
    item.color = static_cast<HanoiColor>(colorDistribution(generator));
    return item;
}

inline DynamicArray<HanoiItem> GenerateHanoiItems(int count) {
    DynamicArray<HanoiItem> items(count);
    for (int i = 0; i < count; ++i) {
        items.Set(i, GenerateHanoiItem(i + 1));
    }
    return items;
}

inline DynamicArray<HanoiItem> ReadManualHanoiItems(int count) {
    DynamicArray<HanoiItem> items(count);

    std::cout << "\nВведите предметы сверху вниз.\n";
    std::cout << "Размер назначается автоматически: 1, 2, 3, ...\n\n";

    for (int i = 0; i < count; ++i) {
        std::cout << "Предмет " << i + 1 << ":\n";

        HanoiItem item;
        item.size = i + 1;
        item.itemType = ReadItemType();
        item.color = ReadColor();

        items.Set(i, item);
        std::cout << '\n';
    }

    return items;
}

inline void PrintHanoiItems(const DynamicArray<HanoiItem>& items) {
    std::cout << "\nПредметы сверху вниз:\n";
    for (int i = 0; i < items.GetLength(); ++i) {
        const HanoiItem& item = items.Get(i);
        std::cout << i + 1 << ". "
                  << GetItemTypeName(item.itemType) << " "
                  << GetColorName(item.color)
                  << ", размер " << item.size << '\n';
    }
    std::cout << '\n';
}

inline void PrintCenteredItem(const HanoiItem& item, int maxWidth) {
    int width = item.size * 2 + 1;
    int spaces = (maxWidth - width) / 2;

    for (int i = 0; i < spaces; ++i) {
        std::cout << ' ';
    }

    std::cout << GetAnsiColor(item.color);

    for (int i = 0; i < width; ++i) {
        if (i == width / 2) {
            std::cout << GetItemSymbol(item.itemType);
        } else {
            std::cout << '=';
        }
    }

    std::cout << GetAnsiReset();

    for (int i = 0; i < spaces; ++i) {
        std::cout << ' ';
    }
}

inline void PrintItemBody(const HanoiItem& item) {
    int width = item.size * 2 + 1;

    std::cout << GetAnsiColor(item.color);
    for (int i = 0; i < width; ++i) {
        if (i == width / 2) {
            std::cout << GetItemSymbol(item.itemType);
        } else {
            std::cout << '=';
        }
    }
    std::cout << GetAnsiReset();
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

inline void PrintHanoiState(const Stack<HanoiItem> rods[3],
                            int maxHeight,
                            const std::string& statusLine = "",
                            bool clearScreen = false,
                            bool includeAirLevel = false) {
    int maxWidth = maxHeight * 2 + 3;

    if (clearScreen) {
        std::cout << "\033[2J\033[H";
    }

    if (!statusLine.empty()) {
        std::cout << statusLine << '\n';
    }

    std::cout << "\nТекущее состояние башни:\n\n";

    if (includeAirLevel) {
        for (int rod = 0; rod < 3; ++rod) {
            PrintEmptyLevel(maxWidth);
            std::cout << "   ";
        }
        std::cout << '\n';
    }

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

inline int GetHanoiRodCenterColumn(int rod, int maxWidth) {
    const int spacing = 3;
    return rod * (maxWidth + spacing) + maxWidth / 2;
}

inline int GetHanoiAnimationTopRow(const std::string& statusLine) {
    return statusLine.empty() ? 4 : 5;
}

inline void SleepHanoiAnimationFrame() {
    std::this_thread::sleep_for(std::chrono::milliseconds(45));
}

inline std::string BuildHanoiMoveDescription(const HanoiItem& item, int moveCount, int from, int to) {
    return std::string("Ход ") + std::to_string(moveCount) + ": "
           + GetItemTypeName(item.itemType) + " "
           + GetColorName(item.color)
           + " размера " + std::to_string(item.size)
           + " : " + std::to_string(from + 1) + " -> " + std::to_string(to + 1);
}

inline void PrintHanoiAnimationFrame(const Stack<HanoiItem> rods[3],
                                     int maxHeight,
                                     const HanoiItem& item,
                                     int itemRow,
                                     int itemCenterColumn,
                                     const std::string& statusLine) {
    const int maxWidth = maxHeight * 2 + 3;
    const int itemWidth = item.size * 2 + 1;
    const int itemColumn = itemCenterColumn - itemWidth / 2 + 1;
    const int screenRow = GetHanoiAnimationTopRow(statusLine) + itemRow;
    const int screenColumn = itemColumn < 1 ? 1 : itemColumn;

    PrintHanoiState(rods, maxHeight, statusLine, true, true);
    std::cout << "\033[" << screenRow << ';' << screenColumn << 'H';
    PrintItemBody(item);
    std::cout << "\033[" << (GetHanoiAnimationTopRow(statusLine) + maxHeight + 7) << ";1H";
    std::cout.flush();
}

inline void AnimateHanoiMove(const Stack<HanoiItem> rods[3],
                             int from,
                             int to,
                             const HanoiItem& item,
                             int maxHeight,
                             const std::string& statusLine) {
    const int maxWidth = maxHeight * 2 + 3;
    const int fromCenter = GetHanoiRodCenterColumn(from, maxWidth);
    const int toCenter = GetHanoiRodCenterColumn(to, maxWidth);
    const int startRow = maxHeight - rods[from].GetSize();
    const int topRow = 0;
    const int targetRow = maxHeight - rods[to].GetSize();

    for (int row = startRow; row >= topRow; --row) {
        PrintHanoiAnimationFrame(rods, maxHeight, item, row, fromCenter, statusLine);
        SleepHanoiAnimationFrame();
    }

    const int distance = toCenter - fromCenter;
    const int horizontalSteps = std::max(6, std::abs(distance) / 2);
    for (int step = 1; step <= horizontalSteps; ++step) {
        const int center = fromCenter + distance * step / horizontalSteps;
        PrintHanoiAnimationFrame(rods, maxHeight, item, topRow, center, statusLine);
        SleepHanoiAnimationFrame();
    }

    for (int row = topRow + 1; row <= targetRow; ++row) {
        PrintHanoiAnimationFrame(rods, maxHeight, item, row, toCenter, statusLine);
        SleepHanoiAnimationFrame();
    }
}

inline void MoveHanoiItem(Stack<HanoiItem> rods[3],
                          int from,
                          int to,
                          int& moveCount,
                          int maxHeight,
                          bool animate = false) {
    HanoiItem item = rods[from].Top();

    if (!rods[to].IsEmpty() && rods[to].Top().size < item.size) {
        throw EmptyStructure("Нельзя класть больший предмет на меньший");
    }

    rods[from].Pop();
    ++moveCount;
    const std::string statusLine = BuildHanoiMoveDescription(item, moveCount, from, to);

    if (animate) {
        AnimateHanoiMove(rods, from, to, item, maxHeight, statusLine);
    }

    rods[to].Push(item);

    if (animate) {
        PrintHanoiState(rods, maxHeight, statusLine, true, true);
        SleepHanoiAnimationFrame();
    } else {
        std::cout << statusLine << '\n';
        PrintHanoiState(rods, maxHeight);
    }
}

inline void SolveHanoi(int count,
                       int from,
                       int to,
                       int buffer,
                       Stack<HanoiItem> rods[3],
                       int& moveCount,
                       int maxHeight,
                       bool animate = false) {
    if (count == 0) {
        return;
    }

    SolveHanoi(count - 1, from, buffer, to, rods, moveCount, maxHeight, animate);
    MoveHanoiItem(rods, from, to, moveCount, maxHeight, animate);
    SolveHanoi(count - 1, buffer, to, from, rods, moveCount, maxHeight, animate);
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

    DynamicArray<HanoiItem> items;
    const int inputMode = ReadHanoiInputMode();
    if (inputMode == 1) {
        items = ReadManualHanoiItems(count);
    } else {
        items = GenerateHanoiItems(count);
        PrintHanoiItems(items);
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
    SolveHanoi(count, from, to, buffer, rods, moveCount, count, true);

    std::cout << "Всего ходов: " << moveCount << '\n';
}
