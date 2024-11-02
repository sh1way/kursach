#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include "functions.h"
#include "constant.h"


void clearField() {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            field[i][j] = 0;
        }
    }
}

void drawImageRectangle(RenderWindow& window, Sprite& sprite, const Vector2f& position, const Vector2f& size) {
    sprite.setPosition(position);
    sprite.setScale(size.x / sprite.getLocalBounds().width, size.y / sprite.getLocalBounds().height);
    window.draw(sprite);
}

bool checkCollision() {
    for (int i = 0; i < 4; i++) {
        if (a[i].x < 0 || a[i].x >= N || a[i].y >= M || field[a[i].y][a[i].x]) {
            return false;
        }
    }
    return true;
}

void spawnFigure() {
    int n = rand() % 7;
    for (int i = 0; i < 4; i++) {
        a[i].x = figures[n][i] % 2 + N / 2 - 1;
        a[i].y = figures[n][i] / 2 + 1;
    }
}

bool checkTopRows() {
    for (int j = 0; j < N; j++) {
        if (field[4][j] != 0) {
            return true;
        }
    }
    return false;
}

void handleEvents(RenderWindow& window, bool& gameOver, int& dx, bool& rotate, bool& gameStarted, const FloatRect& startButtonBounds, const FloatRect& topScoreButtonBounds, int& score, Sprite& button3Sprite) {
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed)
            window.close();
        if (event.type == Event::Closed) {
            window.close();
        }
        if (event.type == Event::KeyPressed && !gameOver) {
            if (event.key.code == Keyboard::Up) {
                rotate = true;
            }
            else if (event.key.code == Keyboard::Right) {
                dx = 1;
            }
            else if (event.key.code == Keyboard::Left) {
                dx = -1;
            }
        }
        if (!gameStarted && event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            if (startButtonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                gameStarted = true;
                spawnFigure();
            }
            if (topScoreButtonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                gameStarted = false;
                gameOver = true;
            }
        }
        if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
            Vector2i mousePos = Mouse::getPosition(window);
            if (button3Sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
                gameStarted = false;
                gameOver = false;
                score = 0;
                clearField();
                spawnFigure();
            }
        }
    }
}

void updateGameState(bool& gameOver, int& dx, bool& rotate, float& timer, float delay, int& colorNum, int& score, bool gameStarted) {
    if (!gameStarted) {
        return;
    }

    if (Keyboard::isKeyPressed(Keyboard::Down)) {
        delay = 0.05;
    }

    for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].x += dx;
    }

    if (!checkCollision()) {
        for (int i = 0; i < 4; i++) {
            a[i] = b[i];
        }
    }

    if (rotate) {
        Point p = a[1];
        for (int i = 0; i < 4; i++) {
            int x = a[i].y - p.y;
            int y = a[i].x - p.x;
            a[i].x = p.x - x;
            a[i].y = p.y + y;
        }
        if (!checkCollision()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
        }
    }

    if (timer > delay) {
        __asm {
            push eax
            push ecx
            push edi
            push esi


            mov ecx, 0

            lea esi, a
            lea edi, b

            loop_start :
            mov eax, [esi + ecx * 8]
                mov[edi + ecx * 8], eax

                mov eax, [esi + ecx * 8 + 4]
                mov[edi + ecx * 8 + 4], eax

                add eax, 1
                mov[esi + ecx * 8 + 4], eax

                inc ecx
                cmp ecx, 4
                jl loop_start

                pop esi
                pop edi
                pop ecx
                pop eax
        }

        if (!checkCollision()) {
            for (int i = 0; i < 4; i++) {
                field[b[i].y][b[i].x] = colorNum;
            }

            colorNum = 1 + rand() % 7;
            spawnFigure();

            if (!checkCollision() || checkTopRows()) {
                gameOver = true;
                updateHighScores("scores.txt", score);
            }
        }

        timer = 0;
    }

    int k = M - 1;
    for (int i = M - 1; i > 0; i--) {
        int count = 0;
        for (int j = 0; j < N; j++) {
            if (field[i][j]) {
                count++;
            }
            field[k][j] = field[i][j];
        }
        if (count < N) {
            k--;
        }
        if (count == N) {
            score += 1;
        }
    }

    dx = 0;
    rotate = false;
}

void drawScene(RenderWindow& window, Sprite& tiles, Sprite& backgroundSprite, Text& scoreText, Text& gameOverText, Text& button1, Sprite& imageRectangle, bool startVisible, int score, bool gameOver, int colorNum, bool gameStarted, Sprite& button3Sprite, Font& font) {
    window.clear(Color::Black);

    window.draw(backgroundSprite);

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (field[i][j] == 0) {
                continue;
            }
            tiles.setTextureRect(IntRect(field[i][j] * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            tiles.setPosition(j * TILE_SIZE, i * TILE_SIZE);
            window.draw(tiles);
        }
    }

    for (int i = 0; i < 4; i++) {
        if (!gameOver && gameStarted) {
            tiles.setTextureRect(IntRect(colorNum * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            tiles.setPosition(a[i].x * TILE_SIZE, a[i].y * TILE_SIZE);
            window.draw(tiles);
        }
    }

    drawImageRectangle(window, imageRectangle, Vector2f(0, 0), Vector2f(N * TILE_SIZE - 1, 4 * TILE_SIZE));

    if (gameOver) {
        window.draw(gameOverText);

        Text yourScoreText("Your score", font, 40);
        yourScoreText.setFillColor(Color::Black);
        yourScoreText.setPosition((N * TILE_SIZE - yourScoreText.getLocalBounds().width) / 2 - 5, M * TILE_SIZE / 4 + 45);

        window.draw(yourScoreText);

        Text scoreNumberText(std::to_string(score), font, 60);
        scoreNumberText.setFillColor(Color::Black);
        scoreNumberText.setPosition((N * TILE_SIZE - scoreNumberText.getLocalBounds().width) / 2 - 5, M * TILE_SIZE / 4 + 70);

        window.draw(scoreNumberText);

        std::set<int> highScores = readHighScores("scores.txt");
        Text highScoresText("High Scores:", font, 30);
        highScoresText.setFillColor(Color::Black);
        highScoresText.setPosition(10, M * TILE_SIZE / 2);
        window.draw(highScoresText);

        int y_offset = 30;
        for (auto it = highScores.rbegin(); it != highScores.rend(); ++it) {
            Text scoreText(std::to_string(*it), font, 30);
            scoreText.setFillColor(Color::Black);
            scoreText.setPosition(10, M * TILE_SIZE / 2 + y_offset);
            window.draw(scoreText);
            y_offset += 30;
        }
    }

    scoreText.setString("Score: " + std::to_string(score));
    window.draw(scoreText);

    Text ByMeText("Tetris! by sh1way", font, 28);
    ByMeText.setFillColor(Color::Red);
    FloatRect rect = imageRectangle.getGlobalBounds();
    ByMeText.setPosition(rect.left + (rect.width - ByMeText.getLocalBounds().width) / 2, rect.top + (rect.height - ByMeText.getLocalBounds().height) / 2 + 15);
    window.draw(ByMeText);

    if (startVisible) {
        window.draw(button1);
    }


    window.draw(button3Sprite);

    window.display();
}

bool isButtonPressed(RenderWindow& window, Text& button) {
    Vector2i mousePos = Mouse::getPosition(window);
    if (Mouse::isButtonPressed(Mouse::Left)) {
        FloatRect bounds = button.getGlobalBounds();
        if (bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            return true;
        }
    }

    return false;
}

void restartGame(bool& gameStarted, bool& gameOver, int& score) {
    gameStarted = false;
    gameOver = false;
    score = 0;
    clearField();
    spawnFigure();
}

std::set<int> readHighScores(const std::string& filePath) {
    std::set<int> scores;
    std::ifstream file(filePath);
    if (file.is_open()) {
        int score;
        while (file >> score) {
            scores.insert(score);
        }
        file.close();
    }
    return scores;
}

void writeHighScores(const std::string& filePath, const std::set<int>& scores) {
    std::ofstream file(filePath);
    if (file.is_open()) {
        for (auto it = scores.rbegin(); it != scores.rend(); ++it) {
            file << *it << std::endl;
        }
        file.close();
    }
}

void updateHighScores(const std::string& filePath, int newScore) {
    std::set<int> scores = readHighScores(filePath);
    scores.insert(newScore);
    writeHighScores(filePath, scores);
}