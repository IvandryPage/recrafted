#include <iostream>
#include "GameManager.h"
#include "InputHandler.h"
#include "Timer.h"

GameManager gameManager;
InputHandler inputHandler;
Timer timer;

int main()
{
    timer.startTimer();

    gameManager.startGame(&inputHandler);

    timer.stopTimer();
    gameManager.exitGame();
    return 0;
}