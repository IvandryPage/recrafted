#include "GameManager.h"

GameManager::GameManager()
{
    loadGame();
    loadScene();
    loadCharacter();
    current_scene_index = 0;
}

GameManager::~GameManager() = default;

void GameManager::startGame(InputHandler* inputHandler)
{
    running_state = true;
    timer.startTimer();

    while(running_state)
    {
        displayScene();

        if (scenes[current_scene_index].getPauseAtEnd())
        {
            Animation loading (Frames::Loading, 4, 2, true, 2, 0);
        }

        if(current_scene_index < std::size(scenes))
        {
            if(std::size(scenes[current_scene_index].getChoices()) != 0)
                getUserInput(inputHandler);
            else
                nextScene();
        }
        else
            running_state = false;
    }

    timer.stopTimer();
    exitGame();
}

void GameManager::exitGame()
{
    saveGame();
    std::cout << "The End!" << std::endl;
    exit(0);
}

void GameManager::pauseGame()
{
    running_state = false;

    std::cout << "Pause!" << std::endl;
}

void GameManager::displayScene()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    scenes[current_scene_index].display(characters);
}

void GameManager::getUserInput(InputHandler* inputHandler)
{
    inputHandler->getPlayerInput(scenes[current_scene_index].getChoices());
    current_scene_index = (inputHandler->getSanitizedInput() >= 0) ? scenes[current_scene_index].getNextScenes()[inputHandler->getSanitizedInput()] : current_scene_index;
    user_choices.push_back(inputHandler->getSanitizedInput());
}

void GameManager::nextScene()
{
    if(scenes[current_scene_index].getNextScene() == -1)
        current_scene_index++;
    else
        current_scene_index = scenes[current_scene_index].getNextScene();

    if(scenes[current_scene_index].getPauseAtEnd())
        std::this_thread::sleep_for(std::chrono::seconds(2));
}

bool GameManager::getState() { return running_state; }

void GameManager::saveGame()
{
    nlohmann::json save_data {
        {"progress_index", current_scene_index},
        {"time_played", timer.getTotalElapsedTime()},
        {"user_choice_history", user_choices}
    };

    std::ofstream save_file("recrafted.json");
    save_file << save_data.dump(4);
    save_file.close();
}

void GameManager::loadGame()
{
    std::ifstream file("recrafted.json");
    if (file.is_open()) {
        nlohmann::json save_data;
        file >> save_data;
        progress_index = save_data.value("progress_index", 0);
        timer.setTotalElapsedTime(std::chrono::seconds(save_data.value("time_played", 0)));
        user_choices = save_data.value("choices", std::vector<int>{});
    } else {
        progress_index = 1;
        user_choices = {};
    }
}

void GameManager::loadScene()
{
    scenes.push_back(
        Scene(
            "title_screen", 
            R"ascii(
               ______                                ___                   _ 
              (_____ \                              / __) _               | |
               _____) )  ____   ____   ____   ____ | |__ | |_    ____   _ | |
              (_____ (  / _  ) / ___) / ___) / _  ||  __)|  _)  / _  ) / || |
                    | |( (/ / ( (___ | |    ( ( | || |   | |__ ( (/ / ( (_| |
                    |_| \____) \____)|_|     \_||_||_|    \___) \____) \____|
              
                          ___      ___       __      ___   __  ___     __  _  ___  ___  ___   
                           |  |__|[__   |   |  |\  /[__   [__)[__ |  |[__) |   |    |  [__ |\ |
                           |  |  |[___  |___|__| \/ [___  |  \[___|/\||  \_|_  |    |  [___| \|
                                                                                                                                                                                                                                               
            )ascii"
        )
        .setIsTitle(true)
        .setNextScene(progress_index)
    );

    scenes.push_back(
        Scene(
            "prologue_1",
            "Aku selalu merasa asing di dunia ini. Semua orang tampak tau apa yang mereka inginkan, kecuali aku.\n"
            "Di jurusan ini, Sistem Informasi, aku bahkan tidak yakin apakah aku benar-benar ingin di sini.\n"
            "Semua orang mengatakan bahwa ini adalah pilihan yang baik, bahwa coding adalah masa depan.\n"
        )
        .addDialogue(Characters::EVA, "Aku merasa terjebak, bener gak yaa aku ngambil jurusan ini?")
        .addDialogue(Characters::EVA, "Ini keinginanku atau ini yang diinginkan orang lain dariku?")
        .addDialogue(Characters::EVA, "Apa yang akan terjadi jika aku tidak melangkah ke dunia coding?")
        .addDialogue(Characters::EVA, "Apa tujuanku masih bisa tercapai?")
    );

    scenes.push_back(
        Scene(
            "prologue_2",
            "Semuanya berubah ketika aku bertemu dengannya -- "
            "Galang, \n"
            "yang membuka mataku terhadap dunia yang tidak pernah aku pikirkan sebelumnya.\n"
            "Di ruang kelas setelah matkul coding"
        )
        .addDialogue(Characters::GALANG, "Halo Eva, aku Galang. Tadi aku ga sengaja liatin kamu, kamu kayak kesulitan ngikuti matkul coding. Kamu gapapa?")
        .addDialogue(Characters::EVA, "Ohh halo, iya aku belum paham apa-apa. Pusinggg...")
        .addDialogue(Characters::GALANG, "Kamu mau aku ajarin?")

        .setPrompt("Apa yang akan aku lakukan?")
        .addChoice("Terima", Scenes::PROLOGUE_3_1)
        .addChoice("Tolak", Scenes::PROLOGUE_3_2)
    );

    scenes.push_back(
        Scene(
            "prologue_3.1",
            "Aku merasa ragu ketika dia menawarkan diri untuk mengajariku.\n"
            "Apa dia benar-benar ingin membantuku apa hanya merasa kasihan kepadaku?\n"
        )
        .addDialogue(Characters::EVA, "Kamu mau ngajarin aku?")
        .addDialogue(Characters::GALANG, "Yaa kalo kamu mau sihh")
        .addDialogue(Characters::EVA, "Hmm... tapi aku bener-bener dari nol yaa")
        .addDialogue(Characters::GALANG, "Amann ajaa. Kita atur jadwalnyaa")
        .setNextScene(Scenes::PROLOGUE_4_1)
    );

    scenes.push_back(
        Scene(
            "prologue_3.2",
            "Aku merasa ragu ketika dia menawarkan diri untuk mengajariku.\n"
            "Aku takut aku mengecewakan karena aku nggak bisa-bisa."
        )
        .addDialogue(Characters::EVA, "Kayaknya nggak dulu dehh, aku belajar sendiri aja dulu")
        .addDialogue(Characters::GALANG, "Ohh okayy, kalo kamu suatu saat berubah pikiran, bilang aja ya")
        .setNextScene(Scenes::PROLOGUE_4_2)
    );

    scenes.push_back(
        Scene(
            "prologue_4.1",
            "Di perjalanan pulang, aku takut mengecewakan Galang.\n"
            "Aku takut aku nggak bisa belajar coding. Dia gimana ya nantinya.\n"
            "Sesampainya di rumah, aku melihat ponselku. Galang mengirim pesan 30 menit yang lalu."
        )
        .addDialogue(Characters::GALANG, "Eva, ini Galang, save ya. Besok kamu free nggak jam 9 pagi?")
        .addDialogue(Characters::EVA, "iy langg, besokk yaa... hmmzz, aku free sih")
        .addDialogue(Characters::GALANG, "Okee sip, temuin aku di kampus yaa. Kita belajar coding")
        .addDialogue(Characters::EVA, "ok")
        .setNextScene(Scenes::PROLOGUE_8)
    );

    scenes.push_back(
        Scene(
            "prologue_4.2",
            "Di perjalanan pulang, aku memikirkan kembali penawaran itu.\n"
            "Menurutku sebenarnya itu bukan penawaran yang buruk.\n"
            "Walaupun memang menakutkan, itu layak untuk dicoba"
        )
        .addDialogue(Characters::EVA, "Aku  coba deh belajar sama dia. Enaknya aku chat atau bilang langsung ya")
        .setPrompt("Aku hubungi lewat apa ya?")
        .addChoice("Chat : cari nomor dulu", Scenes::PROLOGUE_5)
        .addChoice("Langsung : nunggu besok", Scenes::PROLOGUE_7_2)
    );

    scenes.push_back(
        Scene(
            "prologue_5",
            "Sesampainya di rumah, aku memutuskan untuk chat Galang.\n" 
            "Aku harus mencari nomornya lebih dulu"
        )
        .setPrompt("Tanya siapa ya?")
        .addChoice("Yola : kenal Galang duluan", Scenes::PROLOGUE_6_1)
        .addChoice("Nindya : keliatan deket sama Galang", Scenes::PROLOGUE_6_2)
    );

    scenes.push_back(
        Scene(
            "prologue_6.1",
            "Aku memutuskan untuk tanya Yola"
        )
        .addDialogue(Characters::EVA, "eh beb, km tau nomornya galang itu gak?")
        .addDialogue(Characters::YOLA, "Ciee ada yang cinlok nihh... Bentar ya")
        .addDialogue(Characters::YOLA, "Ada nihh, aku share yaa. (ascii kontak share WA)")
        .addDialogue(Characters::EVA, "okay makasihh beb")
        .setNextScene(Scenes::PROLOGUE_7_1)
    );

    scenes.push_back(
        Scene(
            "prologue_6.2",
            "Aku memutuskan untuk tanya Fia"
        )
        .addDialogue(Characters::EVA, "eh beb, km tau nomornya galang itu gak?")
        .addDialogue(Characters::FIA, "Ciee ada yang cinlok nihh... Bentar ya")
        .addDialogue(Characters::FIA, "Ada nihh, aku share yaa. (ascii kontak share WA)")
        .addDialogue(Characters::EVA, "okay makasihh beb")
        .setNextScene(Scenes::PROLOGUE_7_1)
    );

    scenes.push_back(
        Scene(
            "prologue_7_1",
            "Setelah dapat nomornya, aku langsung hubungi dia"
        )
        .addDialogue(Characters::EVA, "halo, galang. ini eva, dapet nomor kamu dari temen aku")
        .addDialogue(Characters::GALANG, "Hai, Eva, kenapa Eva? Ada perlu apa?")
        .addDialogue(Characters::EVA, "Itu, aku berubahh pikirann. Kamu masih mau ngajarin aku nggakke?")
        .addDialogue(Characters::EVA, "kalo gak mau gapapa")
        .addDialogue(Characters::GALANG, "Massihhh dongg, besok jam 9 kamu bisa nggak?")
        .addDialogue(Characters::EVA, "bisa kok, makasih ya")
        .addDialogue(Characters::GALANG, "Okee aku tunggu ya besok")
        .setNextScene(Scenes::PROLOGUE_8)
    );

    scenes.push_back(
        Scene(
            "prologue_7_2",
            "Keesokan harinya, sesampainya di kampus pukul 7 pagi, aku segera menemui Galang dan segera ngomong kalo aku mau belajar coding sama dia"
        )
        .addDialogue(Characters::EVA, "Galang!")
        .addDialogue(Characters::GALANG, "Oit, gimana Eva?")
        .addDialogue(Characters::EVA, "Ini aku mau ngomong soal ajakan kamu kemarin, ...")
        .addDialogue(Characters::EVA, "Aku mau belajar coding kalo kamu masih mau")
        .addDialogue(Characters::GALANG, "Weh, tiba-tiba banget, nanti jam 9 aku free, kamu bisa nggak")
        .addDialogue(Characters::EVA, "Nanti banget??")
        .addDialogue(Characters::GALANG, "Iyaa enggak juga sih, tergantung kamu free atau enggaknyaa")
        .addDialogue(Characters::GALANG, "Aku tungguin deh nanti di kampus, gazebo yak")
        .addDialogue(Characters::EVA, "Oke deh, nanti yaa kalo aku free")
        .setNextScene(Scenes::PROLOGUE_8)
    );

    scenes.push_back(
        Scene(
            "prologue_8",
            "Jam 9 pagi, setelah selesai matkul olahraga. Aku menemui Galang di gazebo kampus. Dia terlihat sudah menungguku daritadi.\n"
            "Walaupun aku masih meragukan apakah aku memilih yang benar, aku bakal mencoba dulu memilih jalan ini."
        )
        .addDialogue(Characters::GALANG, "Ehh dateng juga yang ditunggu-tunggu")
        .addDialogue(Characters::EVA, "Udah lama Lang?")
        .addDialogue(Characters::GALANG, "enggak, belumm, aku juga baru kok")
        .addDialogue(Characters::GALANG, "Baru sejaaam hahaha")
        .addDialogue(Characters::EVA, "Ehh maaf yaa, aku abis kelas tadi")
        .addDialogue(Characters::GALANG, "Gapapaa santaii ajaa, lagian aku kosong kookk, mau mulai langsung aja kahh?")
        .addDialogue(Characters::EVA, "Bolehh")
    );
}

void GameManager::loadCharacter()
{

    characters.push_back(Character(
        "Eva", 
        "Cewek 19 tahun yang suka scroll TikTok dan tidur, tipe orang yang independen. "
        "Lebih sering introvert, tapi kadang juga ekstrovert. Lagi ngerasa kecewa karena jurusannya nggak sesuai.",
        Color::SKY_MAGENTA
    ));
    
    characters.push_back(Character(
        "Galang", 
        "Cowok 20 tahun yang suka coding, musik, dan fotografi. Pake kacamata "
        "dan rambutnya model curtain. Orangnya kreatif dan punya rasa ingin tahu tinggi.",
        Color::SAPPHIRE
    ));

    characters.push_back(Character(
        "Yola",
        "Cewek akuntansi yang ambis dan deket sama Eva",
        Color::ORANGE
    ));

    characters.push_back(Character(
        "Fia",
        "Cewek ekstrovert",
        Color::ORANGE
    ));
}