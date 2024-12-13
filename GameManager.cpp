#include "GameManager.h"

GameManager::GameManager()
{
    loadScene();
    loadCharacter();
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

        current_scene_index = (inputHandler->getSanitizedInput() >= 0) ? scenes[current_scene_index].getNextScenes()[inputHandler->getSanitizedInput()] : current_scene_index;
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
    system("clear");
    scenes[current_scene_index].display(characters);
}

void GameManager::nextScene(int scene_index)
{
    std::cout << "Change scene!" << std::endl;
}

void GameManager::loadScene()
{
    std::cout << "Load all scene data!" << std::endl;
    scenes.push_back(Scene("start_scene", "Home - Recrafted!"));
    scenes[0].addChoice("Go", 1);
    scenes[0].addChoice("Stay", 0);

    scenes.push_back(Scene("second_scene", "Jungle - Recrafted!"));
    scenes[1].addDialogues(0, "EMERGENCY!");
    scenes[1].addDialogues(1, "Status?");
    scenes[1].addDialogues(0, "God save me!");

    scenes[1].addChoice("Home", 0);
    scenes[1].addChoice("Castle", 2);

    scenes.push_back(Scene("third_scene", "Castle - Recrafted!"));
    scenes[2].addChoice("Jungle", 1);
    scenes[2].addChoice("Home", 0);
}

void GameManager::loadCharacter()
{
    std::cout << "Load all characters!" << std::endl;
    characters.push_back(Character("Ivandry", "20 years old boy who loves programming", Color::BRIGHT_BLUE));
    characters.push_back(Character("Eva", "19 years old girl who loves sleeping", Color::BRIGHT_RED));
}