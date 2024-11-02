#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#include <set>
#include "functions.h"
#include "constant.h"

Texture loadTexture(const std::string& filePath) {
    Texture texture;
    if (!texture.loadFromFile(filePath)) {
        exit(1);
    }
    return texture;
}

int main() {
    std::set<int> readHighScores(const std::string & filePath);
    void updateHighScores(const std::string & filePath, int newScore);

    srand(static_cast<unsigned>(time(0)));
    RenderWindow window(VideoMode(N * TILE_SIZE, M * TILE_SIZE), "Tetris!");

    RectangleShape whiteSquare(Vector2f(N * TILE_SIZE, 4 * TILE_SIZE));
    whiteSquare.setFillColor(Color(60, 60, 60));
    whiteSquare.setPosition(0, 0);

    Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../Paint/321.png")) {
        return -1;
    }
    Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(N * TILE_SIZE / static_cast<float>(backgroundTexture.getSize().x), M * TILE_SIZE / static_cast<float>(backgroundTexture.getSize().y));

    Texture tileTexture;
    if (!tileTexture.loadFromFile("../Paint/tiles.png")) {
        return -1;
    }
    Sprite tiles(tileTexture);

    Texture imageTexture;
    if (!imageTexture.loadFromFile("../Paint/321kub.png")) {
        return -1;
    }
    Sprite imageRectangle(imageTexture);

    Font font;
    if (!font.loadFromFile("../Paint/demo2.ttf")) {
        return -1;
    }

    Texture button3Texture;
    if (!button3Texture.loadFromFile("../Paint/menu3.png")) {
        return -1;
    }

    Text scoreText("", font, 45);
    scoreText.setFillColor(Color::Black);
    scoreText.setPosition(10, 0);

    Text gameOverText("Game Over", font, 50);
    gameOverText.setFillColor(Color::Red);
    gameOverText.setStyle(Text::Bold);
    gameOverText.setPosition((N * TILE_SIZE - gameOverText.getLocalBounds().width) / 2 - 5, M * TILE_SIZE / 4);

    Text button1("Start", font, 40);
    button1.setFillColor(Color::Black);
    button1.setStyle(Text::Bold);
    button1.setPosition((N * TILE_SIZE - button1.getLocalBounds().width) / 2, M * TILE_SIZE / 2 - 100);

    Sprite button3Sprite(button3Texture);
    button3Sprite.setScale(51.0f / button3Texture.getSize().x, 51.0f / button3Texture.getSize().y);
    button3Sprite.setPosition(N * TILE_SIZE - 51, 0);

    FloatRect topScoreButtonBounds;
    FloatRect startButtonBounds = button1.getGlobalBounds();

    int score = 0;
    int dx = 0;
    int colorNum = 1;
    bool rotate = false;
    float timer = 0, delay = 0.3;
    Clock clock;

    bool gameStarted = false;
    bool gameOver = false;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        handleEvents(window, gameOver, dx, rotate, gameStarted, startButtonBounds, topScoreButtonBounds, score, button3Sprite);

        if (!gameOver && gameStarted) {
            updateGameState(gameOver, dx, rotate, timer, delay, colorNum, score, gameStarted);
        }

        drawScene(window, tiles, backgroundSprite, scoreText, gameOverText, button1, imageRectangle, !gameStarted, score, gameOver, colorNum, gameStarted, button3Sprite, font);
    }

    return 0;
}