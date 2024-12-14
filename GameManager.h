#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>
#include <vector>
#include "Scene.h"
#include "InputHandler.h"
#include "Character.h"
#include "Animation.h"
#include "Frames.h"

class GameManager
{

    private:
        int current_scene_index {};
        bool running_state {};
        std::vector<Scene> scenes {};
        std::vector<Character> characters {};

    public:
        GameManager();
        ~GameManager();

        void startGame(InputHandler* inputHandler);
        void displayScene();
        void nextScene(int choice_index);

        void pauseGame();
        void exitGame();
        void loadScene();
        void loadCharacter();

};

#endif