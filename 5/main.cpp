#include <algorithm>
#include <fstream>
#include <map>
#include <iostream>
#include <vector>

typedef std::vector<int> Pages;
typedef std::map<int, Pages> Rules;

size_t middlePageNumbers[2] = { 0, 0 };

uint getIncorrectIndex(const Rules *rules, const Pages *pages) {
    for (uint i = 1; i < pages->size(); i++) {
        const int previousPage = pages->at(i - 1);
        if (rules->find(previousPage) == rules->end()) return i;

        const int page = pages->at(i);
        const auto &pageRules = rules->at(previousPage);
        if (std::find(pageRules.begin(), pageRules.end(), page) == pageRules.end()) return i;
    }

    return 0;
}

void outputPages(const Pages *pages) {
    for (size_t i = 0; i < pages->size(); i++) {
        std::cout << pages->at(i) << " ";
    }

    std::cout << std::endl;
}

void addRule(Rules *rules, const std::string *ruleLine) {
    const std::string::size_type index = ruleLine->find('|');
    if (index == std::string::npos) return;

    const int leftRule = std::stoi(ruleLine->substr(0, index));
    const int rightRule = std::stoi(ruleLine->substr(index + 1));

    if (rules->find(leftRule) == rules->end()) {
        (*rules)[leftRule] = std::vector<int>();
    }

    (*rules)[leftRule].push_back(rightRule);
}

void calculateMiddlePageNumbers(const Rules *rules, std::string ruleLine) {
    const std::string::size_type strIndex = ruleLine.find(',');
    if (ruleLine.find(',') == std::string::npos) return;

    Pages pages;
    while (!ruleLine.empty()) {
        pages.push_back(std::stoi(ruleLine.substr(0, strIndex)));
        ruleLine.erase(0, strIndex + 1);
    }

    bool incorrectlyOrdered = false;
    while (true) {
        const uint index = getIncorrectIndex(rules, &pages);
        if (index == 0) break;

        incorrectlyOrdered = true;

        const int previousPage = pages[index - 1];
        const int page = pages[index];
        std::cout << "Swapping " << page << " with " << previousPage << std::endl;
        std::iter_swap(index + pages.begin() - 1, pages.begin() + index);
        if (page == 0) exit(1);
    }

    for (int i = 0; i < pages.size(); i++) {
        std::cout << pages[i] << " ";
    }

    std::cout << std::endl;

    middlePageNumbers[incorrectlyOrdered ? 1 : 0] += pages[pages.size() / 2];;
}

int main() {
    Rules rules;

    std::ifstream input("input.txt");
    std::string line;
    while (std::getline(input, line)) {
        addRule(&rules, &line);
        calculateMiddlePageNumbers(&rules, line);
    }

    std::cout << std::endl;

    for (auto const & [leftRule, rightRules] : rules) {
        std::cout << leftRule << ": ";
        for (auto const & rule : rightRules) {
            std::cout << rule << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    std::cout << "Correctly ordered middle page numbers: " << middlePageNumbers[0] << std::endl;
    std::cout << "Incorrectly ordered middle page numbers: " << middlePageNumbers[1] << std::endl;
}
