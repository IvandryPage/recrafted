#include <iostream>
#include "GameManager.h"
#include "InputHandler.h"

GameManager gameManager;
InputHandler inputHandler;

int main()
{
    gameManager.startGame();
    inputHandler.getPlayerInput();
    std::cout << inputHandler.getInput() << std::endl;
    gameManager.exitGame();
    return 0;
}