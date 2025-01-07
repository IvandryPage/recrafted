#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Character.h"
#include "Dialogue.h"

class Scene
{
    
    private:
        std::string title{};
        std::string description{};
        std::string prompt{};
        int next_scene {};
        std::vector<int> next_scenes {};
        std::vector<std::string> choices {};
        std::vector<Dialogue> dialogues {};
        bool is_ending{};
        bool pause_at_end{};

    public:
        Scene();
        Scene(std::string title_param, std::string description_param, bool is_ending_param = false);
        ~Scene();
        bool getIsEnding();
        int getNextScene();
        bool getPauseAtEnd();
        void display(std::vector<Character> characters);
        void displayDialogues(std::vector<Character> characters);
        Scene& addChoice(const std::string &choice, int next_scene_index);
        Scene& addDialogue(int character_index, const std::string line);
        Scene& setPrompt(std::string prompt_param);
        Scene& setNextScene(int next_scene_param);
        std::vector<std::string>& getChoices();
        std::vector<int> getNextScenes();
};

#endif