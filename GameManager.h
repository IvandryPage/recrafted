#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include <iostream>

class GameManager
{

    private:
        bool running_state {};

    public:
        GameManager();
        ~GameManager();

        void startGame();
        void pauseGame();
        void exitGame();
        void loadScene();

};

#endif