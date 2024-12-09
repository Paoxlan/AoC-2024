#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

constexpr ushort MAX_LOOP = 5;

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position position;
    size_t loopTotal;
} LoopPosition;

struct findPosition {
    int x;
    int y;
    findPosition(const int _x, const int _y) : x(_x), y(_y) {}
    bool operator()(const Position &p) const {
        return p.x == x && p.y == y;
    }
};

struct findLoopPosition {
    int x;
    int y;
    findLoopPosition(const int _x, const int _y) : x(_x), y(_y) {}
    bool operator()(const LoopPosition &lp) const {
        return lp.position.x == x && lp.position.y == y;
    }
};

std::vector<std::string> map;
std::vector<Position> distinctPositions;
size_t totalLoops = 0;

Position initMap() {
    Position startPosition = {};

    std::ifstream input("input.txt");
    std::string line;
    while (std::getline(input, line)) {
        const size_t startIndex = line.find('^');
        map.push_back(line);

        if (startIndex == std::string::npos) continue;

        startPosition.x = static_cast<int>(startIndex);
        startPosition.y = static_cast<int>(map.size() - 1);
    }

    return startPosition;
}

bool isInMap(const Position p) {
    return p.x >= 0 && p.x < map.size() && p.y >= 0 && p.y < map.size();
}

void outputPosition(const Position p) {
    std::cout << '(' << p.x << ", " << p.y << ')' << std::endl;
}

Position rotateDirection(Position direction) {
    if (direction.y == -1) {
        direction.x = 1;
        direction.y = 0;
    } else if (direction.x == 1) {
        direction.x = 0;
        direction.y = 1;
    } else if (direction.y == 1) {
        direction.x = -1;
        direction.y = 0;
    } else {
        direction.x = 0;
        direction.y = -1;
    }

    return direction;
}

void updateDirection(const Position p, Position *direction) {
    while (true) {
        auto [x, y] = p;
        x += direction->x;
        y += direction->y;

        if (!isInMap({x, y})) break;

        const char front = map[y][x];
        if (front != '#') break;

        *direction = rotateDirection(*direction);
    }
}

void updatePosition(Position *position, Position *direction) {
    updateDirection(*position, direction);
    position->x += direction->x;
    position->y += direction->y;
}

bool hasWallInDirection(Position p, const Position d) {
    while (true) {
        p.x += d.x;
        p.y += d.y;

        if (!isInMap(p)) return false;

        const char front = map[p.y][p.x];
        if (front == '#') return true;
    }
}

bool positionContainsLoop(Position position, Position direction) {
    if (!hasWallInDirection(position, rotateDirection(direction))) return false;

    const Position obstruction = { position.x + direction.x, position.y + direction.y };
    if (!isInMap(obstruction)) return false;
    if (map[obstruction.y][obstruction.x] == '#') return false;

    map[obstruction.y][obstruction.x] = '#';

    std::vector<LoopPosition> loopPositions;
    bool looped = false;

    while (!looped && isInMap(position)) {
        updatePosition(&position, &direction);

        auto result = std::find_if(loopPositions.begin(), loopPositions.end(), findLoopPosition(position.x, position.y));
        if (result == loopPositions.end()) {
            loopPositions.push_back({position, 0});
            continue;
        }

        result->loopTotal++;
        if (result->loopTotal >= MAX_LOOP)
            looped = true;
    }

    map[obstruction.y][obstruction.x] = '.';

    return looped;
}

int main() {
    std::vector<Position> distinctPositions;
    Position currentDirection = {0, -1};
    auto currentPosition = initMap();

    while (isInMap(currentPosition)) {
        auto [x, y] = currentPosition;
        if (positionContainsLoop(currentPosition, currentDirection)) {
            totalLoops++;
            // std::cout << totalLoops << std::endl;
        }

        auto result = std::find_if(distinctPositions.begin(),distinctPositions.end(), findPosition(x, y));
        if (result == distinctPositions.end()) {
            distinctPositions.push_back(currentPosition);
        }

        updatePosition(&currentPosition, &currentDirection);
    }

    std::cout << "Total found loops: " << totalLoops << std::endl;
    std::cout << "Distinct Positions: " << distinctPositions.size() << '\n';
}