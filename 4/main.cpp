#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

const std::basic_string WORD = "XMAS";
std::vector<std::string> words;

enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

std::string scanDirection(int x, int y, const direction d) {
    if (y < 1 || y > words.size() - 2) return {};
    if (x < 1 || x > words[0].size() - 2) return {};

    std::stringstream ss;
    if (d == UP) {
        y += 1;
        ss << words[y][x + 1] << words[y][x - 1];
    } else if (d == DOWN) {
        y -= 1;
        ss << words[y][x + 1] << words[y][x - 1];
    } else if (d == LEFT) {
        x -= 1;
        ss << words[y + 1][x] << words[y - 1][x];
    } else {
        x += 1;
        ss << words[y + 1][x] << words[y - 1][x];
    }

    return ss.str();
}

// For Part 1
int scanRow(int x, int y, const int dx, const int dy) {
    std::string word {};

    while (x >= 0 && x < words[0].length() && y >= 0 && y < words.size()) {
        if (word.length() >= 4) break;

        word += words[y][x];
        if (word == "XMAS") return 1;

        x += dx;
        y += dy;
    }

    return 0;
}

int main() {
    std::ifstream input("input.txt");

    std::string str;
    while (std::getline(input, str))
        words.push_back(str);

    size_t results = 0;
    for (int y = 0; y < words.size(); y++) {
        for (int x = 0; x < words[0].length(); x++) {
            if (words[y][x] != 'A') continue;

            std::string up = scanDirection(x, y, UP);
            std::string down = scanDirection(x, y, DOWN);
            std::string left = scanDirection(x, y, LEFT);
            std::string right = scanDirection(x, y, RIGHT);

            if (up == "MM" && down == "SS" ||
                up == "SS" && down == "MM" ||
                left == "MM" && right == "SS" ||
                left == "SS" && right == "MM") {

                results++;
            }
        }
    }

    std::cout << "Total X-MAS: " << results << std::endl;
}