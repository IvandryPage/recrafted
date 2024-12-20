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
        
        if(std::size(scenes[current_scene_index].getChoices()) != 0)
        {
            inputHandler->getPlayerInput(scenes[current_scene_index].getChoices());
            current_scene_index = (inputHandler->getSanitizedInput() >= 0) ? scenes[current_scene_index].getNextScenes()[inputHandler->getSanitizedInput()] : current_scene_index;
        }
        else
        {
            if(current_scene_index < std::size(scenes))
                current_scene_index++;
        }
        
        if(!scenes[current_scene_index].getIsEnding())
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
}

void GameManager::loadCharacter()
{
    std::cout << "Load all characters!" << std::endl;
    characters.push_back(Character("Character 1", "Description of character 1"));
    characters.push_back(Character("Character 2", "Description of character 2"));
}