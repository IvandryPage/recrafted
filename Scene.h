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
        std::vector<Dialogue> chats {};
        bool is_ending{};
        bool pause_at_end{};
        bool is_title{};

    public:
        Scene();
        Scene(std::string title_param, std::string description_param, bool is_ending_param = false);
        ~Scene();
        bool getIsEnding();
        int getNextScene();
        bool getPauseAtEnd();
        void display(std::vector<Character>* characters);
        void displayDialogues(std::vector<Character>* characters);
        void displayChat(std::vector<Character>* characters);
        Scene& addChoice(const std::string &choice, int next_scene_index);
        Scene& addDialogue(int character_index, const std::string line);
        Scene& addDialogue(int character_index, const std::string line, const std::string monologue);
        Scene& addChat(int character_index, const std::string line);
        Scene& setPrompt(std::string prompt_param);
        Scene& setNextScene(int next_scene_param);
        Scene& setIsTitle(bool is_title_param);
        std::vector<std::string>& getChoices();
        std::vector<int> getNextScenes();
        void displaySeparator(char separator_character, std::string separator_title);
};

#endif