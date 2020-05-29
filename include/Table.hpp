#pragma once

#include <algorithm>
#include <chrono>
#include <map>
#include <string>
#include <random>
#include <utility>
#include <vector>

class Table {
public:
    struct Exception {
        int x;
        int y;
        std::pair<int, int> p;
    };

    Table(int size);

    void AddColors(std::pair<int, char> c);

    void Sort();

    std::string ToString();

private:
    std::map<int, char> colors;

    std::vector<std::pair<int, int>> values;
    std::vector<std::vector<std::pair<int, int>>> data;

    std::vector<std::pair<int, int>> topLeftBottomRightDiagonalCellCoords;
    std::vector<std::pair<int, int>> topRightBottomLeftDiagonalCellCoords;

    std::vector<Exception> exceptions;

    int iterations = 0;

    unsigned seed;

    /**
     * Print pair values for debugging purposes
     */
    std::string IntegerPairToString(std::pair<int, int> p);

    bool CellIsInTopLeftBottomRightDiagonal(int x, int y);

    bool CellIsInTopRightBottomLeftDiagonal(int x, int y);

    bool PairsAreSafe(std::pair<int,int> p1, std::pair<int,int> p2);

    int FindSafeValue(int pos, int x, int y);

    void Recycle(int newX, int newY);

    void Sort(int x, int y);
};