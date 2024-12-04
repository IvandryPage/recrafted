#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>

class Scene
{
    
    private:
        std::string title{};
        std::string description{};
        std::vector<int> next_scenes {};
        std::vector<std::string> choices {};
        bool is_ending{};

    public:
        Scene();
        Scene(std::string title_param, std::string description_param);
        ~Scene();
        bool getIsEnding();
        void display();
        void addChoice(const std::string &choice, int next_scene_index);
        std::vector<std::string>& getChoices();
        std::vector<int> getNextScenes();
};

#endif