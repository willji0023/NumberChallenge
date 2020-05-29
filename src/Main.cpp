// (Unoptimized) Solver for Dell'armo's Number Color Challenge. 

#include <ctime>
#include <iostream>

#include "Table.hpp"

int main() {
    int s = 0;
    std::cout << "Side length of square table?" << std::endl;
    std::cin >> s;
    std::cout << s << "x" << s << " table" << std::endl;
    Table table{s};
    std::cout << table.ToString() << std::endl;

    for (int i = 1; i <= s; i++) {
        std::cout << "Abbreviation for color " << i << ":" << std::endl;
        char c;
        std::cin >> c;
        table.AddColors(std::make_pair(i , toupper(c)));
    }
    
    std::clock_t start = std::clock();
    table.Sort();
    std::cout << table.ToString() << std::endl;
    std::cout << "Time taken to solve: " << (std::clock() - start) / (double) CLOCKS_PER_SEC << " seconds" << std::endl;
}