#include "Table.hpp"

Table::Table(int size) {
    // Pre-determine the first row and add the first n values to the list
        std::vector<std::pair<int, int>> r1;
        for (int i = 1; i <= size; i++) {
            r1.push_back(std::make_pair(i, i));
            if (i != 1) {
                values.push_back(std::make_pair(1, i));
            }
        }
        data.push_back(r1);

        // Make the rest of the (n-1)xn table with 0,0's and list with all possible values (excluding the already used values in the predetermined row)
        for (int i = 2; i <= size; i++) {
            std::vector<std::pair<int, int>> row;
            for (int j = 1; j <= size; j++) {
                row.push_back(std::make_pair(0, 0));
                if (i != j) {
                    values.push_back(std::make_pair(i, j));
                }
            }
            data.push_back(row);
        }

        seed = std::chrono::system_clock::now().time_since_epoch().count();
}

void Table::AddColors(std::pair<int, char> c) {
    colors.insert(c);
}

void Table::Sort() {
    Sort(1, 0);
}

std::string Table::ToString() {
    std::string s = "Table\n";
    for (size_t i = 0; i < data.size(); i++ ) {
        for (size_t j = 0; j < data[i].size(); j++ ) {
            auto p = data[i][j];
            s += std::to_string(p.first) + colors[p.second] + " ";
        }
        s += "\n";
    }
    s += "List (should be empty)\n";
    for (size_t i = 0; i < values.size(); i++) {
        auto v = values[i];
        s += std::to_string(v.first) + colors[v.second] + " ";
    }
    s += " Length: " + values.size();
    return s;
}

std::string Table::IntegerPairToString(std::pair<int, int> p) {
    return std::to_string(p.first) + "," + std::to_string(p.second);
}

bool Table::CellIsInTopLeftBottomRightDiagonal(int x, int y) {
    return x == y;
}

bool Table::CellIsInTopRightBottomLeftDiagonal(int x, int y) {
    return (size_t)x + (size_t)y == data.size() - 1;
}

bool Table::PairsAreSafe(std::pair<int,int> p1, std::pair<int,int> p2) {
    return p1.first != p2.first && p1.second != p2.second;
}

int Table::FindSafeValue(int pos, int x, int y) {
    // Check if value is already in the table
    for (size_t i = 0; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            if (values[pos] == data[i][j]) {
                return -1;
            }
        }
    }
    // Check if value is the same as what was tried in prevous attempts
    for (auto e : exceptions) {
        if (values[pos] == e.p && x == e.x && y == e.y) {
            return -1;
        }
    }
    // Check all elements before the current cell in the same row
    for (int j = y - 1; j >= 0; j--) {
        if (!PairsAreSafe(values[pos], data[x][j])) {
            return -1;
        }
    }
    // Check all elements before the current cell in the same column
    for (int i = x - 1; i >= 0; i--) {
        if (!PairsAreSafe(values[pos], data[i][y])) {
            return -1;
        }
    }
    // Check all elements before the current cell in the same central diagonal
    // (if (x, y) is in a central diagonal, of course)
    if (CellIsInTopLeftBottomRightDiagonal(x, y)) {
        for (size_t z = 0; z < data.size(); z++) {
            if (!PairsAreSafe(values[pos], data[z][z])) {
                return -1;
            }
        }
    }
    if (CellIsInTopRightBottomLeftDiagonal(x, y))  {
        int max = data[y].size() - 1;
        for (int z = 0; z <= max; z++) {
            if (!PairsAreSafe(values[pos], data[z][max - z])) {
                return -1;
            }
        }
    }
    return pos;
}

void Table::Recycle(int newX, int newY) {
    // This means the solver completely restarted
    // so exceptions must be cleared
    if (newX == 1 && newY == 0) {
        exceptions.clear();
    }

    exceptions.push_back({newX, newY, data[newX][newY]});

    // Clear new cell and the values in front of it in its row
    for (size_t j = newY; j < data[newX].size(); j++) {
        data[newX][j] = std::make_pair(0, 0);
    }
    // Clear all of the subsequent rows
    for (size_t i = newX + 1; i < data.size(); i++) {
        for (size_t j = 0; j < data[i].size(); j++) {
            data[i][j] = std::make_pair(0, 0);
        }
    }

    std::shuffle(std::begin(values), std::end(values), std::default_random_engine(seed));
}

void Table::Sort(int x, int y) {
    if (!values.empty()) {
        size_t counter = 0;
        int pos = -1;
        for (counter = 0; pos == -1 && counter < values.size(); counter++) {
            pos = FindSafeValue(counter, x, y);
        }
        // If counter is greater than the list size, that means we could not 
        // find any working options from the list, so backtrack one cell
        if (counter >= values.size()) {
            int newX, newY;
            if (y == 0) {
                newX = x - 1;
                newY = data[newX].size() - 1;
            } else {
                newX = x;
                newY = y - 1;
            }

            Recycle(newX, newY);
            Sort(newX, newY);
        } else {
            data[x][y] = values[pos];

            int newX, newY;
            if ((size_t)y == data[x].size() - 1) {
                newX = x + 1;
                newY = 0;
            } else {
                newX = x;
                newY = y + 1;
            }
            Sort(newX, newY);
        }
    }
}