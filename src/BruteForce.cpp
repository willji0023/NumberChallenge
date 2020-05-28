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
    }

    bool CellIsInDiagonal(int x, int y) {
        int counter = 0;
        int max = data.size() - 1;
        while(counter <= max) {
            if (x == counter && y == counter) {
                return true;
            } else if (x == max - counter && y == counter) {
                return true;
            } else if (x == counter && y == max - counter) {
                return true;
            }
            counter++;
        }
        return false;
    }

    bool IsSafe(std::pair<int, int> val, int x, int y) {
        // TODO: implement
        return true;
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
    int iterations = 0;

    void Recycle() {
        for (int i = 1; i < data.size(); i++) {
            for (int j = 0; j < data[i].size(); j++) {
                values.push_back(data[i][j]);
                data[i][j] = std::make_pair(0, 0);
            }
        }
        std::random_shuffle(values.begin(), values.end());
    }

    int Sort(int x, int y) {
        iterations++;
        if (values.empty()) {
            return iterations;
        } else {
            int counter = 0;
            while (!IsSafe(values.front(), x, y) || counter >= values.size()) {
                counter++;
                auto temp = values.front();
                values.pop_front();
                values.push_back(temp);
            }
            // Restart the sorting process if no working options are avaliable from the list 
            if (counter > values.size()) {
                std::cout << "Failed. Trying again." << std::endl; // TODO: remove after debugging
                Recycle();
                Sort();
            } else {
                data[x][y] = values.front();
                values.pop_front();
                if (y == data[x].size() - 1) {
                    return Sort(x + 1, 0);
                } else {
                    return Sort(x, y + 1);
                }
            }
        }
        return -1;
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
    int s = 7; // TODO: change to be any number
    Table table{s};

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