#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include "Character.h"
#include "Dialogue.h"

class Scene
{
    
    private:
        std::string title{};
        std::string description{};
        std::vector<int> next_scenes {};
        std::vector<std::string> choices {};
        std::vector<Dialogue> dialogues {};
        bool is_ending{};

    public:
        Scene();
        Scene(std::string title_param, std::string description_param);
        ~Scene();
        bool getIsEnding();
        void display(std::vector<Character> characters);
        void displayDialogues(std::vector<Character> characters);
        void addChoice(const std::string &choice, int next_scene_index);
        void addDialogues(int character_index, const std::string line);
        std::vector<std::string>& getChoices();
        std::vector<int> getNextScenes();
};

#endif