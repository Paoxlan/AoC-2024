#include <regex>
#include <fstream>
#include <iostream>

int main() {
    std::ifstream input("input.txt");
    std::string str;

    size_t mulTotal = 0;
    bool canMultiply = true;
    while (std::getline(input, str)) {
        std::regex rgx(R"(do\(\)|don't\(\)|mul\(([0-9]+),([0-9]+)\))");
        std::smatch match;

        auto searchStart( str.cbegin() );
        while (std::regex_search(searchStart, str.cend(), match, rgx)) {
            if (match[0] == "do()") {
                canMultiply = true;
            } else if (match[0] == "don't()") {
                canMultiply = false;
            } else if (canMultiply) {
                mulTotal += std::stoi(match[1]) * std::stoi(match[2]);
            }
            searchStart = match.suffix().first;
        }
    }

    std::cout << "Result: " << mulTotal << std::endl;
}