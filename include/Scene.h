#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include "Character.h"
#include "Dialogue.h"

class Scene {
public:
    Scene() = default;
    Scene(std::string title, std::string description, bool is_ending = false);
    ~Scene() = default;

    inline const bool getIsEnding() const { 
        return is_ending_; 
    }
    
    inline const bool getIsTitle() const { 
        return is_title_; 
    }

    inline const int getNextScene() const {
        return next_scene_;
    }

    inline const bool getPauseAtEnd() const {
        return pause_at_end_;
    }

    const std::vector<std::string>& getChoices() const {
        return choices_;
    }

    const std::vector<int> getNextScenes() const {
        return next_scenes_;
    }

    void display(std::vector<Character>* characters);
    void displayDialogues(std::vector<Character>* characters);
    void displayChat(std::vector<Character>* characters);
    void displaySeparator(char separator_character, const std::string& separator_title);

    Scene& addChoice(const std::string& choice, int next_scene_index);
    Scene& addDialogue(int character_index, const std::string& line);
    Scene& addDialogue(int character_index, const std::string& line, const std::string& monologue);
    Scene& addChat(int character_index, const std::string& line);
    Scene& setPrompt(const std::string& prompt);
    Scene& setNextScene(int next_scene);
    Scene& setIsTitle(bool is_title);


private:
    std::string title_{};
    std::string description_{};
    std::string prompt_{};
    std::vector<Dialogue> dialogues_{};
    std::vector<Dialogue> chats_{};

    int next_scene_{};
    std::vector<int> next_scenes_{};
    std::vector<std::string> choices_{};

    bool is_ending_{};
    bool pause_at_end_{};
    bool is_title_{};
};

#endif