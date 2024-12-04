#include "GameManager.h"

GameManager::GameManager()
{
    loadScene();
    current_scene_index = 0;
}

GameManager::~GameManager() = default;

void GameManager::startGame(InputHandler* inputHandler)
{
    running_state = true;
    std::cout << "Recrafted - The Love Rewritten" << std::endl;
    std::cout << "Loading.. animation" << std::endl;
    while(running_state)
    {
        displayScene();
        inputHandler->getPlayerInput(scenes[current_scene_index].getChoices());

        current_scene_index = scenes[current_scene_index].getNextScenes()[inputHandler->getSanitizedInput()];
        nextScene(current_scene_index);
    }
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


void GameManager::displayScene()
{
    // system("clear");

    std::cout << "\n\n";
    scenes[current_scene_index].display();
}

void GameManager::nextScene(int scene_index)
{
    std::cout << "Change scene!" << std::endl;
}

void GameManager::loadScene()
{
    std::cout << "Load all scene data!" << std::endl;
    scenes.push_back(Scene("start_scene", "Introduction - Recrafted!"));
    scenes.push_back(Scene("second_scene", "Introduction 2 - Recrafted!"));

    scenes[0].addChoice("Go", 1);
    scenes[0].addChoice("Stay", 0);
    scenes[1].addChoice("Home", 0);
    scenes[1].addChoice("Castle", 1);
}