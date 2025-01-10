#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include "Scene.h"
#include "InputHandler.h"
#include "Timer.h"
#include "Character.h"
#include "Animation.h"
#include "Frames.h"
#include "CharactersIndex.h"
#include "ScenesIndex.h"

class GameManager
{

    private:
        int current_scene_index {};
        std::vector<int> user_choices {};
        bool running_state {};
        std::vector<Scene> scenes {};
        std::vector<Character> characters {};

    public:
        GameManager();
        ~GameManager();

        void startGame(InputHandler* inputHandler);
        bool getState();
        void displayScene();
        void nextScene();
        void getUserInput(InputHandler* inputHandler);

        void pauseGame();
        void exitGame();
        void loadScene();
        void loadCharacter();

};

#endif