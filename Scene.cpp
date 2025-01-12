#include "Scene.h"

Scene::Scene() = default;
Scene::Scene(std::string title_param, std::string description_param, bool is_ending_param)
{
    title = title_param;
    description = description_param;
    is_ending = is_ending_param;
    next_scene = -1;
    pause_at_end = true;
    is_title = false;
}

Scene::~Scene() = default;

bool Scene::getIsEnding() { return is_ending; }
int Scene::getNextScene() { return next_scene; }
bool Scene::getPauseAtEnd() { return pause_at_end; }
std::vector<std::string>& Scene::getChoices() { return choices; }
std::vector<int> Scene::getNextScenes() { return next_scenes; }

void Scene::display(std::vector<Character> characters)
{
    if(!is_title)
    {
        // displaySeparator('=', "Narrator");
        displaySeparator('=', title);
        Animation::type(description);
    }
    else
        std::cout << description << std::endl;


    if(std::size(dialogues) != 0)
    {
        displaySeparator('-', "--------");
        displayDialogues(characters);
    }
    
    if(std::size(choices) != 0)
    {
        displaySeparator('-', "Pilihan");

        std::cout << prompt << std::endl;
        for(int i{}; i < std::size(choices); i++)
        {
            std::cout << '(' << i+1 << ')';
            std::cout << choices[i] << std::endl;
        }
    }
}

void Scene::displaySeparator(char separator_character, std::string separator_title)
{
    Animation::changeColor(Color::RAISIN_BLACK);
    std::cout << "\n" << std::setw(45) << std::setfill(separator_character) << "";
    std::cout << std::left << std::setw(45 + separator_title.length()) << separator_title << "\n\n";
    Animation::resetColor();
}

void Scene::displayDialogues(std::vector<Character> characters)
{
    for(auto& dialogue : dialogues)
    {
        characters[dialogue.character_index].speak(dialogue.line);
    }
}

Scene& Scene::addChoice(const std::string &choice, int next_scene_index)
{
    choices.push_back(choice);
    next_scenes.push_back(next_scene_index);
    return *this;
}

Scene& Scene::addDialogue(int character_index, const std::string line)
{
    dialogues.push_back(Dialogue(character_index, line));
    return *this;
}

Scene& Scene::setPrompt(std::string prompt_param)
{
    prompt = prompt_param;
    pause_at_end = false;
    return *this;
}

Scene& Scene::setNextScene(int next_scene_param)
{
    next_scene = next_scene_param;
    return *this;
}

Scene& Scene::setIsTitle(bool is_title_param)
{
    is_title = is_title_param;
    return *this;
}