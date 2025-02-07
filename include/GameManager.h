#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include "../include/library/json.hpp"
#include "Scene.h"
#include "InputHandler.h"
#include "Timer.h"
#include "Character.h"
#include "Animation.h"
#include "Frames.h"
#include "CharactersIndex.h"
#include "ScenesIndex.h"

#define SAVE_FILE_NAME "recrafted_memory.json"

class GameManager
{

    private:
        int current_scene_index {};
        int progress_index {};
        std::vector<int> user_choices {};
        bool running_state {};
        std::vector<Scene> scenes {};
        std::vector<Character> characters {};
        Timer timer;

    public:
        GameManager();
        ~GameManager();

        void startGame(InputHandler* inputHandler);
        bool getState();
        void displayScene();
        void nextScene();
        void getUserInput(InputHandler* inputHandler);

        void startMenu(InputHandler* inputHandler);
        void exitGame();
        void loadScene();
        void loadCharacter();

        void saveGame();
        void loadGame();

};

#endif