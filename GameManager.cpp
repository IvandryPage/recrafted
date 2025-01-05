#include "GameManager.h"

GameManager::GameManager()
{
    loadScene();
    loadCharacter();
    current_scene_index = 0;
}

GameManager::~GameManager() = default;

void GameManager::startGame(InputHandler* inputHandler)
{
    running_state = true;
    while(running_state)
    {
        displayScene();
        
        if(std::size(scenes[current_scene_index].getChoices()) != 0)
            getUserInput(inputHandler);
        else
            nextScene();
    }
}

void GameManager::exitGame()
{
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
    system("clear");
    scenes[current_scene_index].display(characters);
}

void GameManager::getUserInput(InputHandler* inputHandler)
{
    inputHandler->getPlayerInput(scenes[current_scene_index].getChoices());
    current_scene_index = (inputHandler->getSanitizedInput() >= 0) ? scenes[current_scene_index].getNextScenes()[inputHandler->getSanitizedInput()] : current_scene_index;
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
        .setNextScene(1) // should be the save file
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
        .addDialogue(Characters::EVA, "Hmm.. kayaknya nggak dulu dehh, aku belajar sendiri aja dulu")
        .addDialogue(Characters::GALANG, "Ohh okayy, kalo kamu suatu saat berubah pikiran, bilang aja ya")
        .setNextScene(Scenes::PROLOGUE_4_2)
    );
}

void GameManager::loadCharacter()
{
    characters.push_back(Character(
        "Galang", 
        "Cowok 20 tahun yang suka coding, musik, dan fotografi. Pake kacamata "
        "dan rambutnya model curtain. Orangnya kreatif dan punya rasa ingin tahu tinggi."
    ));

    characters.push_back(Character(
        "Eva", 
        "Cewek 19 tahun yang suka scroll TikTok dan tidur, tipe orang yang independen. "
        "Lebih sering introvert, tapi kadang juga ekstrovert. Lagi ngerasa kecewa karena jurusannya nggak sesuai."
    ));

    characters.push_back(Character(
        "Yola", 
        "Cewek ekstrovert yang suka cari peluang, jago akuntansi, dan penasaran banget sama banyak hal."
    ));

    characters.push_back(Character(
        "Fia", 
        "Cewek ekstrovert yang super berisik, gampang terdistraksi, tapi juga ambisius banget."
    ));

    characters.push_back(Character(
        "Maulana", 
        "Cowok yang suka banget main Genshin Impact dan Valorant, content creator kecil, "
        "santai aja tapi punya sisi bad boy."
    ));

    characters.push_back(Character(
        "Awan", 
        "Cowok 20 tahun yang punya catatan kriminal, tenang aja tapi punya aura bad boy "
        "yang mirip mafia, meskipun bukan mafia."
    ));

    characters.push_back(Character(
        "Intan", 
        "Cewek introvert yang tenang, suka petualangan, dan penuh energi. Sering kasih vibe 'buru-buru'-nya"
    ));

    characters.push_back(Character(
        "Kris", 
        "Cowok kaya yang lembut dan agak naif. Cenderung menghindari masalah, dan lagi tertarik sama Eva."
    ));

    characters.push_back(Character(
        "Yuli", 
        "Cewek yang ceria dan penuh semangat, suka ngobrol dan memiliki banyak teman. "
        "Terkadang sedikit kekanak-kanakan, tapi selalu membawa keceriaan di sekitarnya."
    ));

    characters.push_back(Character(
        "Ana", 
        "Cewek yang suka seni dan sangat kreatif, introvert tapi memiliki bakat dalam menggambar dan melukis. "
        "Terkadang sulit bergaul, tapi sangat peduli dengan teman-temannya."
    ));

    characters.push_back(Character(
        "Dika", 
        "Cowok yang pendiam, suka hal-hal yang berbau teknologi, dan lebih memilih menghabiskan waktu sendirian "
        "daripada bergaul. Namun, ia sangat cerdas dan sering membantu teman-temannya dalam hal teknis."
    ));

    characters.push_back(Character(
        "Rays", 
        "Cewek yang suka anime dan musik, dengan tanda-tanda depresi dan ketertarikan pada budaya Jepang. "
        "Dihadapkan pada orang tua yang sangat ketat."
    ));

    characters.push_back(Character(
        "Habibi", 
        "Cowok ekstrovert yang berani, dari keluarga kaya, suka bercanda, dan jago olahraga."
    ));

    characters.push_back(Character(
        "Sya", 
        "Cewek tomboy yang punya aura bossy, ekstrovert, dan selalu menghadapi deadline. "
        "Meskipun imut, semua cewek pada dengar dia."
    ));

    characters.push_back(Character(
        "Via", 
        "Cewek feminim, ekstrovert, rambut panjang, dan selalu ada deadline. "
        "Suka banget perhatian, aktif banget di sosial."
    ));

    characters.push_back(Character(
        "Sipa", 
        "Cewek introvert yang ambisius, suka baca buku, dan pengen banget kerja di public relation. "
        "Dia lebih suka simpen pengetahuannya sendiri dan cuma bagi kalau perlu."
    ));

    characters.push_back(Character(
        "Azkia", 
        "Cewek yang sangat disiplin dan terorganisir, sangat teliti dalam pekerjaannya. "
        "Meskipun agak tertutup, dia selalu siap membantu teman-temannya jika dibutuhkan."
    ));

    characters.push_back(Character(
        "Ayu", 
        "Cewek dengan kepribadian yang ramah dan empati tinggi. Meskipun terlihat lembut, "
        "dia memiliki ketegasan yang luar biasa ketika menghadapi masalah."
    ));

    characters.push_back(Character(
        "Ros", 
        "Cewek yang menyukai musik dan sering bermain gitar. Memiliki sifat yang easy-going "
        "dan suka menghibur teman-temannya dengan lagu-lagu yang dia mainkan."
    ));
}