// (Unoptimized) Solver for Dell'armo's Number Color Challenge. 

#include <algorithm>
#include <ctime>
#include <iostream>
#include <utility>
#include <vector>
#include <deque>
#include <map>
#include <string>

class Table {
public:
    struct Exception {
        int x;
        int y;
        std::pair<int, int> p;
    };

    Table(int size) {
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

        // Shuffle value list to hopefully achieve a different solution than the last
        std::srand ( unsigned ( std::time(0) ) );
        // std::random_shuffle(values.begin(), values.end());
        std::cout << "Table initialized" << std::endl;
    }

    int Sort() {
        return Sort(1, 0);
    }

    std::vector<std::vector<std::pair<int, int>>> GetData() const {
        return data;
    }

    std::deque<std::pair<int, int>> GetValues() const {
        return values;
    }

    friend std::ostream& operator<< (std::ostream &out, const Table &table);

    static std::string pairToString(std::pair<int, int> p) {
        return std::to_string(p.first) + "," + std::to_string(p.second);
    }

private:
    std::deque<std::pair<int, int>> values;
    std::vector<std::vector<std::pair<int, int>>> data;

    std::vector<std::pair<int, int>> topLeftBottomRightDiagonalCellCoords;
    std::vector<std::pair<int, int>> topRightBottomLeftDiagonalCellCoords;

    std::vector<Exception> exceptions;

    int iterations = 0;

    bool CellIsInTopLeftBottomRightDiagonal(int x, int y) {
        return x == y;
    }

    bool CellIsInTopRightBottomLeftDiagonal(int x, int y) {
        return x + y == data.size() - 1;
    }

    bool PairsAreSafe(std::pair<int,int> p1, std::pair<int,int> p2) {
        return p1.first != p2.first && p1.second != p2.second;
    }

    bool ListFrontIsSafe(std::pair<int, int> val, int x, int y) {
        // Check if value is the same as what was tried in prevous attempts
        for (auto e : exceptions) {
            if (val == e.p && x == e.x && y == e.y) {
                std::cout << "Caught exception for " << pairToString(val) << "at (" << x << ", " << y << ")" << std::endl;
                return false;
            }
        }
        // Check all elements before the current cell in the same row
        for (int j = y - 1; j >= 0; j--) {
            if (!PairsAreSafe(val, data[x][j])) {
                return false;
            }
        }
        // Check all elements before the current cell in the same column
        for (int i = x - 1; i >= 0; i--) {
            if (!PairsAreSafe(val, data[i][y])) {
                return false;
            }
        }
        // Check all elements before the current cell in the same central diagonal
        // (if (x, y) is in a central diagonal, of course)
        if (CellIsInTopLeftBottomRightDiagonal(x, y)) {
            for (int z = 0; z < data.size(); z++) {
                if (!PairsAreSafe(val, data[z][z])) {
                    return false;
                }
            }
        }
        if (CellIsInTopRightBottomLeftDiagonal(x, y))  {
            int max = data[y].size() - 1;
            for (int z = 0; z <= max; z++) {
                if (!PairsAreSafe(val, data[z][max - z])) {
                    return false;
                }
            }
        }
        return true;
    }


    // New X and Y should always be further back in the table than Old X and Y
    // Because we are only BACK-tracking to a new location
    std::pair<int,int> Recycle(int oldX, int oldY, int newX, int newY) {
        // This means the solver completely restarted
        // so exceptions must be cleared
        if (newX == 1 && newY == 0) {
            exceptions.clear();
            std::cout << "Cleared exceptions" << std::endl;
        }

        auto faultPair = data[newX][newY];

        // If new and old cell are on the same row
        if (newX == oldX) {
            for (int j = newY; j < oldY; j++) {
                std::cout << "Pushing back: " << pairToString(data[newX][j]) << std::endl;            
                // Checks if p has an element
                values.push_back(data[newX][j]);
                data[newX][j] == std::make_pair(0, 0);
            }
        } else {
            // Clear new cell and the values in front of it in its row
            for (int j = newY; j <= data[newX].size() - 1; j++) {
                std::cout << "Pushing back: " << pairToString(data[newX][j]) << std::endl;                        
                    values.push_back(data[newX][j]);
                    data[newX][j] == std::make_pair(0, 0);

            }
            // Clear old cell and the values behind it in the its row
            for (int j = oldY - 1; j >= 0; j--) {
                std::cout << "Pushing back: " << pairToString(data[oldX][j]) << std::endl;                        
                    values.push_back(data[oldX][j]);
                    data[oldX][j] == std::make_pair(0, 0);
    
            }
            // Clear all cells in rows inbetween the new and old cells
            for (int i = newX + 1; i <= oldX - 1; i++) {
                for (int j = 0; j <= values.size() - 1; j++) {
                    std::cout << "Pushing back: " << pairToString(data[i][j]) << std::endl;                        
                        values.push_back(data[i][j]);
                        data[i][j] == std::make_pair(0, 0);
                }
            }
        }
        std::random_shuffle(values.begin(), values.end());
        return faultPair;
    }

    int Sort(int x, int y) {
        iterations++;
        if (values.empty()) {
            return iterations;
        } else {
            int counter = 0;
            std::cout << "New cell" << std::endl;
            while (!ListFrontIsSafe(values.front(), x, y) && counter < values.size()) {
                counter++;
                auto temp = values.front();
                values.pop_front();
                values.push_back(temp);
            }
            // If counter is greater than the list size, that means we could not 
            // find any working options from the list, so backtrack one cell
            if (counter >= values.size()) {
                std::cout << "Failed. Reverting one cell back at (" << x << ", " << y << ")" << std::endl; // TODO: remove after debugging
                int newX, newY;
                if (y == 0) {
                    newX = x - 1;
                    newY = data[newX].size() - 1;
                } else {
                    newX = x;
                    newY = y - 1;
                }

                std::cout << "List: ";
                for (int i = 0; i < values.size(); i++) {
                    std::cout << pairToString(values[i]) << " ";
                }
                std::cout << std::endl;

                std::cout << "Made " << pairToString(data[newX][newY]) << " an exception at (" << newX << ", " << newY << ")" << std::endl;
                exceptions.push_back({newX, newY, Recycle(x, y, newX, newY)});
                return Sort(newX, newY);
            } else {
                std::cout << "Successful value " << pairToString(values.front()) << " found for (" << x << ", " << y << ")" << std::endl;
                data[x][y] = values.front();
                values.pop_front();

                std::cout << "List: ";
                for (int i = 0; i < values.size(); i++) {
                    std::cout << pairToString(values[i]) << " ";
                }
                std::cout << std::endl;
                std::cout << "List items left: " << values.size() << std::endl;
                std::cout << "Table spots left: " << data.size() * data.size() - (x * data.size() + y) - 1 << std::endl;

                int newX, newY;
                if (y == data[x].size() - 1) {
                    newX = x + 1;
                    newY = 0;
                } else {
                    newX = x;
                    newY = y + 1;
                }
                return Sort(newX, newY);
            }
        }
    }
};

std::ostream& operator<<(std::ostream& os, const Table& table) {
    os.clear();
    os << "Table\n";
    for (int i = 0; i < table.GetData().size(); i++ ) {
        for (int j = 0; j < table.GetData()[i].size(); j++ ) {
            os << Table::pairToString(table.GetData()[i][j]) << " ";
        }
        os << "\n";
    }
    os << "List (should be empty)\n";
    for (int i = 0; i < table.GetValues().size(); i++) {
        os << Table::pairToString(table.GetValues()[i]) << " ";
    }
    os << " Length: " << table.GetValues().size();
    return os;
}

int main() {
    // int s = 0;
    // std::cout << "Side length of square table?" << std::endl;
    // std::cin >> s;
    // std::cout << s << "x" << s << " table" << std::endl;
    int s = 5; // TODO: change to be any number
    Table table{s};
    std::cout << table;

    /*std::map<int, std::string> colors;
    for (int i = 1; i <= s; i++) {
        std::cout << "Name color " << i << ":" << std::endl;
        std::string c;
        std::cin >> c;
        colors.insert(std::make_pair(i, c));
    }*/ // TODO: Uncomment when everything is finished
    
    std::clock_t start = std::clock();
    int tries = table.Sort();
    std::cout << table << std::endl;
    std::cout << "Time taken to solve: " << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
}