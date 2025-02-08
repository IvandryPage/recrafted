#include <iostream>
#include "../include/GameManager.h"
#include "../include/InputHandler.h"

GameManager gameManager;
InputHandler inputHandler;

int main() {
    gameManager.startGame(&inputHandler);
    return 0;
}