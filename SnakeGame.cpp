#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace sf;
using namespace std;

int width = 600, height = 600, size = 20;
int dir = 0, delay = 150, score = 0;
bool gameOver = false;

Vector2i food;
vector<Vector2i> snake = { Vector2i(10, 10) };

void spawnFood() {
    food.x = rand() % (width / size);
    food.y = rand() % (height / size);
}

void moveSnake() {
    for (int i = snake.size() - 1; i > 0; i--)
        snake[i] = snake[i - 1];

    if (dir == 0) snake[0].y -= 1;
    else if (dir == 1) snake[0].x += 1;
    else if (dir == 2) snake[0].y += 1;
    else if (dir == 3) snake[0].x -= 1;
}

bool checkCollision() {
    Vector2i& head = snake[0];
    if (head.x < 0 || head.x >= width / size || head.y < 0 || head.y >= height / size)
        return true;
    for (int i = 1; i < snake.size(); i++)
        if (snake[i] == head)
            return true;
    return false;
}

int main() {
    srand(time(0));

    RenderWindow window(VideoMode(width, height), "Snake Game");
    window.setFramerateLimit(60);

    RectangleShape cell(Vector2f(size - 1, size - 1));
    Font font;
    font.loadFromFile("arial.ttf");

    SoundBuffer eatBuffer, gameOverBuffer;
    eatBuffer.loadFromFile("eat.wav");
    gameOverBuffer.loadFromFile("gameover.wav");
    Sound eatSound(eatBuffer), overSound(gameOverBuffer);

    spawnFood();
    Clock clock;
    Time timer;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Up && dir != 2) dir = 0;
                else if (e.key.code == Keyboard::Right && dir != 3) dir = 1;
                else if (e.key.code == Keyboard::Down && dir != 0) dir = 2;
                else if (e.key.code == Keyboard::Left && dir != 1) dir = 3;
            }
        }

        if (!gameOver && clock.getElapsedTime().asMilliseconds() > delay) {
            clock.restart();
            moveSnake();

            if (snake[0] == food) {
                eatSound.play();
                snake.push_back(snake.back());
                score++;
                spawnFood();
                if (delay > 50) delay -= 5;
            }

            if (checkCollision()) {
                gameOver = true;
                overSound.play();
            }
        }

        window.clear(Color::Black);

        for (int i = 0; i < snake.size(); i++) {
            cell.setFillColor(i == 0 ? Color::Green : Color(0, 200, 0));
            cell.setPosition(snake[i].x * size, snake[i].y * size);
            window.draw(cell);
        }

        cell.setFillColor(Color::Red);
        cell.setPosition(food.x * size, food.y * size);
        window.draw(cell);

        if (gameOver) {
            Text overText("Game Over", font, 40);
            overText.setFillColor(Color::White);
            overText.setPosition(width / 2 - 100, height / 2 - 50);
            window.draw(overText);
        }

        window.display();
    }

    return 0;
}
