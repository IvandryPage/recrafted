#include "GameManager.h"

GameManager::GameManager() = default;
GameManager::~GameManager() = default;

void GameManager::startGame()
{
    running_state = true;
    std::cout << "Recrafted - The Love Rewritten" << std::endl;
    std::cout << "Loading.. animation" << std::endl;
}

void GameManager::exitGame()
{
    std::cout << "Exit Game!" << std::endl;
    exit(0);
}

void GameManager::pauseGame()
{
    running_state = false;

    std::cout << "Pause!" << std::endl;
}

void GameManager::loadScene()
{
    std::cout << "Load scene!" << std::endl;
}