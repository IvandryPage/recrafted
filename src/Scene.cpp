#include "../include/Scene.h"

Scene::Scene(std::string title, std::string description, bool is_ending)
    : title_(title), description_(description), is_ending_(is_ending)
{
    next_scene_ = -1;
    pause_at_end_ = true;
    is_title_ = false;
}

void Scene::display(std::vector<Character>* characters) {
    if (!is_title_) {
        displaySeparator('=', "Narrator");
        Animation::type(description_);
    } else {
        std::cout << description_ << '\n';
    }

    if (std::size(dialogues_) != 0) {
        displaySeparator('-', "--------");
        displayDialogues(characters);
    } else if (std::size(chats_) != 0) {
        displaySeparator('-', "--------");
        displayChat(characters);
    }
    
    if(std::size(choices_) != 0) {
        displaySeparator('-', "Pilihan");

        std::cout << prompt_ << '\n';
        for(int i = 0; i < std::size(choices_); i++) {
            std::cout << '(' << i+1 << ')' << " ";
            std::cout << choices_[i] << '\n';
        }
    }
}

void Scene::displaySeparator(char separator_character, const std::string& separator_title) {
    Animation::withColor(Color::RAISIN_BLACK, [&]() {
        std::cout << "\n" 
                  << std::setw(45) << std::setfill(separator_character) << ""
                  << std::left << std::setw(45 + separator_title.length()) << separator_title 
                  << "\n\n";
    });
}

void Scene::displayDialogues(std::vector<Character>* characters) {
    for (auto& dialogue : dialogues_) {
        characters->at(dialogue.character_index_).speak(dialogue.line_);

        if(!dialogue.monologue_.empty()) {
            std::cout << std::setw(13) << " " << "(\"" << dialogue.monologue_ << "\")" << std::endl;
        }
    }
}

void Scene::displayChat(std::vector<Character>* characters) {
    for (const auto& chat : chats_) {
        characters->at(chat.character_index_).chat(chat.line_);
    }
}

Scene& Scene::addChoice(const std::string &choice, int next_scene_index) {
    choices_.push_back(choice);
    next_scenes_.push_back(next_scene_index);
    return *this;
}

Scene& Scene::addDialogue(int character_index, const std::string& line) {
    dialogues_.push_back(Dialogue(character_index, line));
    return *this;
}

Scene& Scene::addDialogue(int character_index, const std::string& line, const std::string& monologue) {
    dialogues_.push_back(Dialogue(character_index, line, monologue));
    return *this;
}

Scene& Scene::addChat(int character_index, const std::string& line) {
    chats_.push_back(Dialogue(character_index, line));
    return *this;
}

Scene& Scene::setPrompt(const std::string& prompt) {
    prompt_ = prompt;
    pause_at_end_ = false;
    return *this;
}

Scene& Scene::setNextScene(int next_scene) {
    next_scene_ = next_scene;
    return *this;
}

Scene& Scene::setIsTitle(bool is_title) {
    is_title_ = is_title;
    return *this;
}