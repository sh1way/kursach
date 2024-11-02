#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <string>
#include <vector>

using namespace sf;

void clearField();
bool checkCollision();
void spawnFigure();
bool checkTopRows();
void handleEvents(RenderWindow& window, bool& gameOver, int& dx, bool& rotate, bool& gameStarted, const FloatRect& startButtonBounds, const FloatRect& topScoreButtonBounds, int& score, Sprite& button3Sprite);
void updateGameState(bool& gameOver, int& dx, bool& rotate, float& timer, float delay, int& colorNum, int& score, bool gameStarted);
void drawScene(RenderWindow& window, Sprite& tiles, Sprite& backgroundSprite, Text& scoreText, Text& gameOverText, Text& button1, Sprite& imageRectangle, bool startVisible, int score, bool gameOver, int colorNum, bool gameStarted, Sprite& button3Sprite, Font& font);
bool isButtonPressed(RenderWindow& window, Text& button);
void restartGame(bool& gameStarted, bool& gameOver, int& score);
std::set<int> readHighScores(const std::string& filePath);
void writeHighScores(const std::string& filePath, const std::set<int>& scores);
void updateHighScores(const std::string& filePath, int newScore);