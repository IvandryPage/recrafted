#include "Scene.h"

Scene::Scene() = default;
Scene::Scene(std::string title_param, std::string description_param)
{
    title = title_param;
    description = description_param;
}

Scene::~Scene() = default;

bool Scene::getIsEnding() { return is_ending; }
std::vector<std::string>& Scene::getChoices() { return choices; }
std::vector<int> Scene::getNextScenes() { return next_scenes; }

void Scene::display()
{
    std::cout << title << std::endl;
    std::cout << description << std::endl;
    
    for(int i{}; i < std::size(choices); i++)
    {
        std::cout << '(' << i+1 << ')';
        std::cout << choices[i] << std::endl;
    }
}

void Scene::addChoice(const std::string &choice, int next_scene_index)
{
    choices.push_back(choice);
    next_scenes.push_back(next_scene_index);
}