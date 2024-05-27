#include <iostream>
#include <vector>
#include <conio.h>
#include <ctime>
#include <cstdlib>

using namespace std;

class Rectangle {
public:
    int top;
    int left;
    int bottom;
    int right;

    int GetWidth() const { return right - left; }
    int GetHeight() const { return bottom - top; }
};

class Apple {
public:
    int x;
    int y;

    void GenerateNewPosition(int mapWidth, int mapHeight, const vector<pair<int, int>>& occupiedPositions) {
        bool positionOccupied = true;
        while (positionOccupied) {
            x = rand() % (mapWidth - 2) + 1; // Avoid placing apple on the borders
            y = rand() % (mapHeight - 2) + 1;

            positionOccupied = false;
            for (const auto& pos : occupiedPositions) {
                if (pos.first == x && pos.second == y) {
                    positionOccupied = true;
                    break;
                }
            }
        }
    }
};

class Snake {
public:
    vector<pair<int, int>> positions;
    int length;
    int speed;
    char direction; // 'U' (Up), 'D' (Down), 'L' (Left), 'R' (Right)

    void Move() {
        int dx = 0, dy = 0;
        switch (direction) {
        case 'U': dy = -1; break;
        case 'D': dy = 1; break;
        case 'L': dx = -1; break;
        case 'R': dx = 1; break;
        }

        int newX = positions[0].first + dx;
        int newY = positions[0].second + dy;

        positions.insert(positions.begin(), make_pair(newX, newY));
        positions.resize(length);
    }

    bool CheckCollision(int mapWidth, int mapHeight) const {
        int headX = positions[0].first;
        int headY = positions[0].second;

        if (headX <= 0 || headX >= mapWidth - 1 || headY <= 0 || headY >= mapHeight - 1)
            return true; // Collision with the wall

        for (size_t i = 1; i < positions.size(); ++i) {
            if (positions[i].first == headX && positions[i].second == headY)
                return true; // Collision with itself
        }

        return false;
    }
};

class Player {
public:
    int score;
};

class Game {
public:
    int mapWidth;
    int mapHeight;
    Snake snake;
    Apple apple;
    Player player;

    void Setup() {
        mapWidth = 30;
        mapHeight = 20;
        snake.length = 10;
        snake.speed = 1;
        snake.direction = 'R';

        // Initial snake position in the middle of the map
        int startX = mapWidth / 2;
        int startY = mapHeight / 2;
        for (int i = 0; i < snake.length; ++i) {
            snake.positions.push_back(make_pair(startX - i, startY));
        }

        player.score = 0;
        GenerateApplePosition();
    }

    void Draw() {
        system("cls"); // Clear the console

        for (int i = 0; i < mapHeight; ++i) {
            for (int j = 0; j < mapWidth; ++j) {
                if (i == 0 || i == mapHeight - 1 || j == 0 || j == mapWidth - 1)
                    cout << "#"; // Draw the map borders
                else if (snake.positions[0].first == j && snake.positions[0].second == i)
                    cout << "O"; // Draw the snake's head
                else if (IsSnakeBodySegment(j, i))
                    cout << "o"; // Draw the snake's body
                else if (apple.x == j && apple.y == i)
                    cout << "A"; // Draw the apple
                else
                    cout << " "; // Empty space
            }
            cout << endl;
        }

        cout << "Score: " << player.score << "   Length: " << snake.length << endl;
    }

    void Input() {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'w' || key == 'W') snake.direction = 'U';
            else if (key == 's' || key == 'S') snake.direction = 'D';
            else if (key == 'a' || key == 'A') snake.direction = 'L';
            else if (key == 'd' || key == 'D') snake.direction = 'R';
        }
    }

    void Update() {
        snake.Move();

        if (snake.positions[0].first == apple.x && snake.positions[0].second == apple.y) {
            snake.length++;
            player.score++;
            GenerateApplePosition();
        }

        if (snake.CheckCollision(mapWidth, mapHeight)) {
            Draw();
            cout << "Game Over! Score: " << player.score << " Length: " << snake.length << endl;
            cout << "Press 'Y' to play again or any other key to exit." << endl;
            char choice = _getch();
            if (choice == 'y' || choice == 'Y') {
                snake.positions.clear();
                snake.length = 10;
                snake.direction = 'R';
                GenerateApplePosition();
                Setup();
            }
            else {
                exit(0);
            }
        }
    }

private:
    bool IsSnakeBodySegment(int x, int y) const {
        for (size_t i = 1; i < snake.positions.size(); ++i) {
            if (snake.positions[i].first == x && snake.positions[i].second == y)
                return true;
        }
        return false;
    }

    void GenerateApplePosition() {
        vector<pair<int, int>> occupiedPositions = snake.positions;
        apple.GenerateNewPosition(mapWidth, mapHeight, occupiedPositions);
    }
};

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    Game game;
    game.Setup();

    while (true) {
        game.Draw();
        game.Input();
        game.Update();
    }

    return 0;
}
