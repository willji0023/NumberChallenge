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
        unsigned char x;
        unsigned char y;
        std::pair<unsigned char, unsigned char> p;
    };

    Table(const unsigned char& size);

    void AddColors(const std::pair<unsigned char, char>& c);

    void Sort();

    std::string ToString();

private:
    std::map<unsigned char, char> colors;

    std::vector<std::pair<unsigned char, unsigned char>> values;
    std::vector<std::vector<std::pair<unsigned char, unsigned char>>> data;

    std::vector<std::pair<unsigned char, unsigned char>> topLeftBottomRightDiagonalCellCoords;
    std::vector<std::pair<unsigned char, unsigned char>> topRightBottomLeftDiagonalCellCoords;

    std::vector<Exception> exceptions;

    unsigned seed;

    bool CellIsInTopLeftBottomRightDiagonal(const unsigned char& x, const unsigned char& y);

    bool CellIsInTopRightBottomLeftDiagonal(const unsigned char& x, const unsigned char& y);

    bool PairsAreSafe(const std::pair<unsigned char,unsigned char>& p1, const std::pair<unsigned char,unsigned char>& p2);

    char FindSafeValue(const unsigned char& pos, const unsigned char& x, const unsigned char& y);

    void Recycle(const unsigned char& newX, const unsigned char& newY);

    void Sort(const unsigned char& x, const unsigned char& y);
};