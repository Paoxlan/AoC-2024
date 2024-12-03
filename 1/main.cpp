#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

std::vector<int> left;
std::vector<int> right;
std::map<int, int> similarityMap;

bool addToList(const bool isLeft, const std::string *str) {
    const int value = std::stoi(*str);
    if (isLeft) {
        left.push_back(value);
        return false;
    }

    right.push_back(value);
    if (similarityMap.find(value) != similarityMap.end()) {
        similarityMap[value]++;
    } else {
        similarityMap[std::stoi(*str)] = 1;
    }

    return true;
}

int main() {
    std::ifstream file("input.txt");

    bool isLeft = true;
    std::string str;
    while (std::getline(file, str)) {
        std::stringstream ss(str);

        while (getline(ss, str, ' ')) {
            if (str.find_first_not_of(' ') == std::string::npos) continue;
            isLeft = addToList(isLeft, &str);
        }
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    size_t totalDistance = 0;
    size_t totalSimilarity = 0;
    for (int i = 0; i < left.size(); i++) {
        int leftItem = left[i];

        totalDistance += abs(leftItem - right[i]);
        if (similarityMap.find(leftItem) == similarityMap.end()) continue;

        totalSimilarity += leftItem * similarityMap[leftItem];
    }

    std::cout << "Total Distance: " << totalDistance << std::endl;
    std::cout << "Total Similarity: " << totalSimilarity << std::endl;
}
