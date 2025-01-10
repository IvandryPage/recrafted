#include <iostream>
#include "GameManager.h"
#include "InputHandler.h"

GameManager gameManager;
InputHandler inputHandler;

int main()
{
    gameManager.startGame(&inputHandler);
    return 0;
}