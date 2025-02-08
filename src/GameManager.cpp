#include "../include/GameManager.h"

GameManager::GameManager() {
    loadScene();
    loadCharacter();
    current_scene_index = 0;
}

void GameManager::startGame(InputHandler* inputHandler) {
    running_state = true;
    while (running_state) {
        timer.startTimer();
        displayScene();

        if (std::filesystem::exists(kSaveFileName) && current_scene_index == 0) {
            startMenu(inputHandler);
        }

        if (std::size(scenes[current_scene_index].getChoices()) != 0) {
            getUserInput(inputHandler);
        } else {
            nextScene();
        }

        if (scenes[current_scene_index].getIsEnding()) {
            running_state = false;
        }

        if (!scenes[current_scene_index].getPauseAtEnd()) {
            std::cout << "\n Tekan tombol apapun untuk melanjutkan ... ";

#if defined(__linux__) || defined(__APPLE__)
                inputHandler->getKey();
#else
                _getch();
#endif
        }

        timer.stopTimer();
        saveGame();
        Animation loading (Frames::Loading, 4, 2, true, 2, 0);
    }

    exitGame();
}

void GameManager::exitGame() {
    timer.displayTime();
    exit(0);
}

void GameManager::startMenu(InputHandler* inputHandler) {
    std::cout << "Ketik angka atau kata untuk memilih. CTRL + C untuk keluar\n";
    std::vector<std::string> choices{"Lanjut", "Mulai Baru", "Keluar"};

    for (int i{}; i < std::size(choices); i++) {
        std::cout << '(' << i+1 << ')' << " ";
        std::cout << choices[i] << std::endl;
    }

    inputHandler->getPlayerInput(choices);
    int menu_input = inputHandler->getSanitizedInput();
    switch (menu_input) {
        case 0:
            loadGame();
            break;
        case 2:
            exitGame();
            break;
    }
}

void GameManager::displayScene() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

    scenes[current_scene_index].display(&characters);
}

void GameManager::getUserInput(InputHandler* inputHandler) {
    inputHandler->getPlayerInput(scenes[current_scene_index].getChoices());
    current_scene_index = (inputHandler->getSanitizedInput() >= 0) ? scenes[current_scene_index].getNextScenes()[inputHandler->getSanitizedInput()] : current_scene_index;
    user_choices.push_back(inputHandler->getSanitizedInput());
}

void GameManager::nextScene() {
    if (scenes[current_scene_index].getNextScene() == -1) {
        current_scene_index++;
    } else {
        current_scene_index = scenes[current_scene_index].getNextScene();
    }
}

void GameManager::saveGame() {
    if (progress_index != std::size(scenes)) {
        nlohmann::json save_data {
            {"progress_index", current_scene_index},
            {"time_played", timer.getTotalElapsedTime()},
            {"user_choice_history", user_choices}
        };

        std::ofstream save_file(kSaveFileName); 
        save_file << save_data.dump(4);
        save_file.close();
    }
}

void GameManager::loadGame() {
    std::ifstream file(kSaveFileName);
    if (file.is_open()) {
        nlohmann::json save_data;
        file >> save_data;
        progress_index = save_data.value("progress_index", 1);
        timer.setTotalElapsedTime(std::chrono::seconds(save_data.value("time_played", 0)));
        user_choices = save_data.value("choices", std::vector<int>{});
    } else {
        progress_index = 1;
        user_choices = {};
    }

    scenes[0].setNextScene(progress_index);
}

void GameManager::loadScene() {
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
    );

    scenes.push_back(
      Scene(
        "Prolog_1",
        "Sebuah kehormatan bagi aku bisa berkuliah setelah lulus dari SMK.\n"
        "Namun, aku merasa asing di tempat ini. Semua orang tampaknya tahu apa yang mereka inginkan,\n"
        "tahu apa yang harus mereka kejar, kecuali aku. Aku bahkan tidak yakin dengan keputusanku masuk\n"
        "ke jurusan Sistem Informasi ini."
      )
      .addDialogue(Characters::EVA, "(Semua orang bilang coding adalah masa depan)", "Aku sangat meragukan itu")
      .addDialogue(Characters::EVA, "(Apa yang akan terjadi jika aku tidak melangkah ke dunia coding?)")
      .addDialogue(Characters::EVA, "(Apa impianku masih bisa tercapai?)")
      .setPrompt("Apa yang akan aku lakukan?")
      .addChoice("Meyakinkan diri sendiri", Scenes::PROLOG_2A)
      .addChoice("Membiarkan ketidakyakinan itu", Scenes::PROLOG_2B)
      .addChoice("Mencari distraksi", Scenes::PROLOG_2C)
    );

    scenes.push_back(
        Scene(
            "Prolog_2A",
            "Aku yakin aku bisa lewati semua!\n"
            "Walaupun mungkin berat, aku harus bisa! Demi mama papa!\n"
            "Minggu ini harus aku lalui semaksimalku!"
        )
        .setNextScene(Scenes::PROLOG_3)
    );

    scenes.push_back(
        Scene(
            "Prolog_2B",
            "Aku membiarkan ketidakyakinan itu menghantuiku. Di minggu ini, aku berusaha belajar.\n"
            "Namun, rasanya aku tidak ikhlas, aku belajar dengan setengah hati, tidak ada materi yang masuk"
            "ke dalam otakku. Berat sekali rasanyaa minggu ini."
        )
        .addDialogue(Characters::EVA, "(Gimana aku bisa survive kalo kayak gini)")
        .setNextScene(Scenes::PROLOG_3)
    );

    scenes.push_back(
        Scene(
            "Prolog_2C",
            "Aku berusaha mencari kesibukan agar aku tidak terpikirkan oleh hal itu\n"
            "Menonton tiktok, marathon drama korea, pergi jalan-jalan, jajan, ngobrol sama temen-temenku\n"
            "... semuanya aku lakukan agar tidak terpikirkan hal itu\n"
            "Aku pun tidak mempelajar materi yang disampaikan dosen. Malas sekali rasanya..."
        )
        .setNextScene(Scenes::PROLOG_3)
    );

    scenes.push_back(
        Scene(
            "Prolog_3",
            "Satu minggu kemudian,                \n"
            "siang itu setelah matkul programming, aku semakin merasa terjebak di tempat ini, rasanya..\n"
            "hanya aku yang tidak bisa mengikuti materinya. Seperti pemula yang berada di tengah kumpulan ahli.\n" 
            "Saat aku sedang termenung itu, tiba-tiba ada yang menghampiriku ...\n"
        )
        .addDialogue(Characters::GALANG, "Permisi maaf, aku Galang. Aku tadi liatin kamu selama di kelas, ...")
        .addDialogue(Characters::GALANG, "kamu kayaknya kesulitan ngikutin ya?")
        .addDialogue(Characters::EVA, "Hehe, Iya aku kesulitan, Galang. Btw, nama aku Eva.", "Kok dia perhatiin aku sihh sial!")
        .addDialogue(Characters::GALANG, "Kalo gitu, kamu mau nggak belajar bareng aku? Aku udah bisa dikit sih")
        .addDialogue(Characters::EVA, "...", "Gimana ini yaa? Terima enggak")
        .setPrompt("Apa yang akan Eva lakukan")
        .addChoice("Terima dengan antusias", Scenes::PROLOG_4A)
        .addChoice("Terima dengan ragu-ragu", Scenes::PROLOG_4B)
        .addChoice("Tolak dengan sopan", Scenes::PROLOG_4C)
    );
    
    scenes.push_back(
        Scene(
            "Prolog_4A",
            "Begitu senangnya diriku ketika ada yang menawarkan diri untuk mengajariku.\n"
            "Kesempatan yang tidak mungkin aku buang begitu saja. Segera aku bertukar nomor dan mendiskusikan\nrencana sesi belajar dengan Galang"
        )
        .addDialogue(Characters::EVA, "Jadi, kita mau gimana belajarnya?")
        .addDialogue(Characters::GALANG, "Tergantung kamu soalnya, kalo aku free terus sih cuma fokus kuliah soalnya")
        .addDialogue(Characters::EVA, "Aku juga kalii", "Ga ada inisiatifnya apa ya ini orang?")
        .addDialogue(Characters::GALANG, "Gimana kalo seminggu 3x? Hari Senin, Rabu, Sabtu")
        .addDialogue(Characters::EVA, "Jangan sabtu deh, aku mau rebahann kalo sabtu, kamis aja gimana?")
        .addDialogue(Characters::GALANG, "Ok baik, aku tunggu di next sesi ya berarti")
        .setNextScene(Scenes::PROLOG_9)
    );

    scenes.push_back(
        Scene(
            "Prolog_4B",
            "Aku ragu aku bisa mengikuti cara belajarnya, apa yang akan terjadi kalo aku gak bisa?\nDia pasti bakal ngolok-olok aku. "
            "Salah gak ya aku nerima tawaran dia. Aduh, gimana ini"
        )
        .addDialogue(Characters::GALANG, "Gimana kamu mau sesi belajarnya seperti?")
        .addDialogue(Characters::EVA, "Maksudnya?")
        .addDialogue(Characters::GALANG, "Seberapa sering kita bakal belajar bareng.")
        .addDialogue(Characters::EVA, "...", "Hah masa langsung atur jadwal gini sih")
        .addDialogue(Characters::EVA, "Kalo lagi free aja, bisa enggak?")
        .addDialogue(Characters::GALANG, "Ya gapapa sih, aku ngikut jadwal kamu aja kalo gitu")
        .addDialogue(Characters::GALANG, "Ini nomorku 62######, Aku duluan yaa, kabarin kalo kamu free.")
        .addDialogue(Characters::EVA, "Ok")
        .setNextScene(Scenes::PROLOG_9)
    );

    scenes.push_back(
        Scene(
            "Prolog_4C",
            "Aku menolak ajakan tersebut. Aku tidak ingin merepotkan orang lain dalam tanggung jawabku."
        )
        .addDialogue(Characters::GALANG, "Ohh okayy, jadi gak mau yaa")
        .addDialogue(Characters::EVA, "Maaf yaa, aku lagi gak bisa soalnyaa, banyak urusaan")
        .addDialogue(Characters::GALANG, "Baiklahh, kalo misal suatu saat kamu berubah pikiran, bilang aja yaa. Aku duluan!")
        .setNextScene(Scenes::PROLOG_5)
    );

    scenes.push_back(
        Scene(
            "Prolog_5",
            "Aku belajar mandiri selama beberapa minggu. Mencoba memahami coding itu seperti apa\n"
            "Namun, sudah beberapa minggu sejak saat itu, susah sekali materi ini masuk ke dalam otakku.\n"
            "Hal ini membuatku kembali merenungkan keputusanku"
        )
        .addDialogue(Characters::EVA, "(Salah deh aku ngambil keputusan ini. Berat sekali belajarnya)")
        .addDialogue(Characters::EVA, "(Gimana inii... nasibkuu)")
        .addDialogue(Characters::EVA, "(Omong-omong si Galang Galang itu masih mau nggak ya kalo aku minta ngajarin)")
        .addDialogue(Characters::EVA, "(Keliatannya di kelas juga dia jago)")
        .setPrompt("Apa aku minta bantuan dia?")
        .addChoice("Hubungi Galang", Scenes::PROLOG_6A)
        .addChoice("Tetap belajar mandiri", Scenes::PROLOG_6B)
    );
    
    scenes.push_back(
        Scene(
            "Prolog_6A",
            "Aku memutuskan untuk meminta bantuannya lagi. Berharap dia masih mau mengajariku.\n"
            "Siapa ya yang punya nomornya? atau aku tunggu besok aja bilang langsung? Enaknya gimana ya"
        )
        .setPrompt("Apa yang akan aku lakukan?")
        .addChoice("Chat Galang", Scenes::PROLOG_7A)
        .addChoice("Langsung saat ketemu", Scenes::PROLOG_7B)
    );

    scenes.push_back(
        Scene(
            "Prolog_6B",
            "Melalui pertimbangan yang aku lakukan, aku semakin yakin dengan keinginanku untuk pindah jurusan saja.\n"
            "Aku akan membicarakannya dengan kedua orang tuaku. Semoga mereka mau mempertimbangkan hal ini.\n"
            "Karena aku benar-benar kesulitan di sini, aku harap mereka bisa memberiku pertimbangan kembali."
        )
        .addDialogue(Characters::EVA, "Pa, Ma.. aku mau ngomong sesuatu", "dengan grogi aku berbicara hal ini")
        .addDialogue(Characters::MM, "Iya sayangg, ada apaa?")
        .addDialogue(Characters::PP, "...")
        .addDialogue(Characters::EVA, "Sebenernya selama aku kuliah di sini, aku merasa gak nyaman Pa, Ma")
        .addDialogue(Characters::PP, "Ada apa sini cerita sama mama papa")
        .addDialogue(Characters::EVA, "Aku kesulitan ngejar materinya, udah belajar berjam-jam berhari-hari, ga ada yang masuk")
        .addDialogue(Characters::EVA, "Di sini juga banyak yang udah jago, aku merasa ketinggalan")
        .addDialogue(Characters::MM, "Kamu merasa salah jurusan, Sayaang?")
        .addDialogue(Characters::PP, "Papa rasa itu wajar, namanya baru belajar")
        .addDialogue(Characters::EVA, "Tapi Ma Pa, aku juga takut aku gak bisa lulus. Nanti malah merepotkan Mama Papa")
        .addDialogue(Characters::PP, "Yaudah coba pikir-pikir dulu, mau lanjutin apa pindah langsung?")
        .setPrompt("Keputusan apa yang harus aku ambil?")
        .addChoice("Lanjut dengan berat hati", Scenes::ENDING_1)
        .addChoice("Pindah jurusan", Scenes::ENDING_2A)
    );

    scenes.push_back(
        Scene(
            "Prolog_7A",
            "Untuk menghubungi dia, aku harus tau nomornya, tapi gimana aku nyarinyaa,\n"
            "aku coba tanya yang udah aku kenal aja deeh"
        )
        .addChat(Characters::EVA, "beb, kamu kenal galang enggak?")
        .addChat(Characters::ALIFIA, "Nggak e Va. Coba tanya Yunita")
        .addChat(Characters::EVA, "ok deh")
        .addChat(Characters::EVA, "yunitaa, kamu tau galang enggakk?")
        .addChat(Characters::YUNITA, "Galang kelas kita? yang bisa ngoding itu")
        .addChat(Characters::EVA, "iya ituu, kamu tauu kahh, aku boleh minta nomornya gakk")
        .addChat(Characters::YUNITA, "Tauu, ciee ada yang cinlok nih kayaknyaa")
        .addChat(Characters::EVA, "enggak yaa, cuma mau tauu aja, buruan dehh")
        .addChat(Characters::YUNITA, "Iya sabar deh si sensi")
        .addChat(Characters::YUNITA, "Galang(asciiart_phone_share)")
        .addChat(Characters::EVA, "makasihh beb")
        .setNextScene(Scenes::PROLOG_8)
    );

    scenes.push_back(
        Scene(
            "Prolog_7B",
            "Keesokan harinya, aku segera buru-buru mencari orang itu -- Galang di kampus.\n"
            "Sempat putus asa mencari karena dia tidak masuk kelas hari itu.\n"
            "Entah lah apa yang ada dipikirannya. Dua jam aku berkeliling, mencarinya.\n"
            "Akhirnya aku menemukannya, di taman fakultas sebelah, sendirian bergumam dengan laptopnya.\n"
            "Segera aku temui dia\n"
        )
        .addDialogue(Characters::EVA, "Galangg!")
        .addDialogue(Characters::GALANG, "Eh Eva, haloo, kok sampe sini")
        .addDialogue(Characters::EVA, "Harusnya aku gak sih yang nanya kamu gitu.")
        .addDialogue(Characters::EVA, "Aku nyariin kamu daritadi. ehm, aku mau ngomong sesuatuu")
        .addDialogue(Characters::GALANG, "eh iya? mau ngomong apa? sampe nyariin gitu")
        .addDialogue(Characters::EVA, "Tawaran kamu masih bisa berlaku lagi gak yaa? Aku berubah pikiran")
        .addDialogue(Characters::GALANG, "Oohh, masihh kokk, kamu mau aku ajarin nih sekarang ceritanyaa?")
        .addDialogue(Characters::EVA, "Iya hehe, bisa gak?", "aku malu karena pernah nolak tawaran dia")
        .addDialogue(Characters::GALANG, "Okee, boleh ajaa, kita atur jadwalnya ya")
        .addDialogue(Characters::EVA, "Oke sip, makasihh yaaa")
        .setNextScene(Scenes::PROLOG_12B)
    );

    scenes.push_back(
        Scene(
            "Prolog_8",
            "Tanpa menunggu lama lagi, aku segera menghubungi dia setelah mendapatkan nomornya"
        )
        .addChat(Characters::EVA, "halo, galang, aku eva, save nomorku yaa")
        .addChat(Characters::GALANG, "Ok Eva. Ada perlu apa ya?")
        .addChat(Characters::EVA, "ituuu soal tawaran kamu kemarinn,")
        .addChat(Characters::EVA, "masih berlaku gak yaa?")
        .addChat(Characters::GALANG, "kamu berubah pikiran po?\n")
        .addChat(Characters::EVA, "iyaa, coding susah bangett butuh orang lain ngajarin")
        .addChat(Characters::GALANG, "Hahaha, oke")
        .addChat(Characters::EVA, "jadi kamu mau nggak?")
        .addChat(Characters::GALANG, "Bolehh, kita atur jadwalnya")
        .addChat(Characters::EVA, "sip makasihh")
        .setNextScene(Scenes::PROLOG_10B)
    );

    scenes.push_back(
        Scene(
            "Prolog_9",
            "Aku bingung, kenapa Galang mau membantuku padahal belum pernah kenal.\n"
            "Sebenarnya, apa yang dia mau dari aku. Apa tujuan dia ya?"
        )
        .setPrompt("Menurutku Galang itu: ")
        .addChoice("Mentor yang kompeten", Scenes::PROLOG_10A)
        .addChoice("Temen yang potensial", Scenes::PROLOG_10B)
        .addChoice("Seseorang yang ngebantu karena kasihan", Scenes::PROLOG_10B)
    );

    scenes.push_back(
        Scene(
            "Prolog_10A",
            "Aku dan Galang merencanakan untuk segera memulai sesi belajar kita karena kita merasa\n"
            "semakin cepat memulai maka akan semakin baik. Rencananya kita akan mulai hari rabu besok ini.\n"
            "Aku cukup berharap Galang bisa membantuku meningkatkan pemahaman coding"
        )
        .setNextScene(Scenes::PROLOG_11A)
    );

    scenes.push_back(
        Scene(
            "Prolog_10B",
            "Aku masih ragu-ragu untuk segera memulai sesi belajar dengannya.\n"
            "Aku takut mengecewakan dan takut terlihat bodoh di depannya\n"
            "Hingga akhirnya aku memutuskan untuk memulai sesi pertama kita di minggu depan"
        )
        .setNextScene(Scenes::PROLOG_11B)
    );

    scenes.push_back(
        Scene(
            "Prolog_11A",
            "Hari Rabu, pagi hari dengan cuaca yang cerah dan tidak panas itu. Aku janjian untuk menemuinya di kampus,\n"
            "sekitar taman fakultas sebelah. "
            "Dari kejauhan terlihat dia sedang memandangi laptopnya dengan serius\n"
            "Dia seperti telah menyiapkan apa yang mau dia sampaikan dengan matang.\n"
            "Kami memulai sesi belajar pertama kami hingga siang hari.\n"
        )
        .addDialogue(Characters::GALANG, "Gimana hari ini? masih banyak pasti kan yang dibingungin")
        .addDialogue(Characters::EVA, "Hahaha tau aja kamu")
        .addDialogue(Characters::GALANG, "Kan proses belajar, jadi wajar aja kalo masih banyak yang dibingungin")
        .addDialogue(Characters::GALANG, "Kita akhiri dulu ya hari ini, nanti kamu bisa review kalo mau tanya, chat aja")
        .addDialogue(Characters::EVA, "Okay okay makasih yaa")
        .setPrompt("Setelah sesi belajar pertama ini, apa yang akan aku lakukan?")
        .addChoice("Review materi yang diberikan Galang", Scenes::PROLOG_12A)
        .addChoice("Merasa coding semakin susah", Scenes::PROLOG_12B)
    );

    scenes.push_back(
        Scene(
            "Prolog_11B",
            "Seminggu kemudian, siang harinya. Aku bertemu dengannya di kampus, sekitar taman fakultas sebelah.\n"
            "Aku menghampiri dia yang sedang fokus dengan laptopnya, tak menyangka dia menyiapkan materi untukku\n"
        )
        .addDialogue(Characters::EVA, "Kamu beneran mau ngajarin aku ta? sampe nyiapin materi gituu")
        .addDialogue(Characters::GALANG, "Lah kamu kira aku bercanda.. aku serius kokk")
        .addDialogue(Characters::EVA, "Hehe, okehh deh, boleh kita mulai aja?")
        .addDialogue(Characters::GALANG, "Okay")
        .addDialogue(Characters::NARRATOR, "Aku dan dia belajar dari jam 1 siang hingga jam 3 sore")
        .addDialogue(Characters::GALANG, "Gimana hari ini? masih banyak pasti kan yang dibingungin")
        .addDialogue(Characters::EVA, "Hahaha tau aja kamu")
        .addDialogue(Characters::GALANG, "Kan proses belajar, jadi wajar aja kalo masih banyak yang dibingungin")
        .addDialogue(Characters::GALANG, "Kita akhiri dulu ya hari ini, nanti kamu bisa review kalo mau tanya, chat aja")
        .addDialogue(Characters::EVA, "Okay okya makasih yaa")
        .setPrompt("Setelah sesi belajar pertama ini, apa yang akan aku lakukan?")
        .addChoice("Merasa coding semakin susah", Scenes::PROLOG_12B)
        .addChoice("Mempertimbangkan pindah jurusan", Scenes::PROLOG_6B)
    );

    scenes.push_back(
        Scene(
            "Prolog_12A",
            "Aku akan mereview materi yang disampaikan Galang nanti. Sekarang aku mau jalan-jalan terlebih dahulu.\n"
            "Ketika aku sedang di parkiran, tiba-tiba ada yang memanggilku"
        )
        .addDialogue(Characters::ALIFIA, "Eva")
        .addDialogue(Characters::YUNITA, "Cie cie yang habis ketemu cowook..")
        .addDialogue(Characters::EVA, "Apa dehh kaliann, gajelass")
        .addDialogue(Characters::ALIFIA, "Alahh gak usah ngelak dehh, aku sama Yuni tadi ngefoto kalian lagi berduaan lohh")
        .addDialogue(Characters::EVA, "Eh kokk kaliann, woilahh, hapus gakk hapuss")
        .addDialogue(Characters::ALIFIA, "Gak mau, gak mauu")
        .addDialogue(Characters::YUNITA, "aahahahha")
        .addDialogue(Characters::EVA, "Ah kalian mahh, serah dahh, aku mau pulang dulu")
        .addDialogue(Characters::EVA, "Jangan disebar loh yaa, awas aja kalian")
        .addDialogue(Characters::YUNITA, "buru-buru banget, mau teleponan yaa sama Galang")
        .addDialogue(Characters::EVA, "Sssstttt diem ah")
        .setNextScene(Scenes::PROLOG_13)
    );

    scenes.push_back(
        Scene(
            "Prolog_12B",
            "Ternyata malah makin banyak tahu, coding makin susahh. Aku merasa aku tidak layak di sini..\n"
            "Padahal ini masih sesi pertama, tapi aku sudah kesusahan seperti ini.\n"
        )
        .addDialogue(Characters::EVA, "(Bisa nggak ya aku lanjutin ini)")
        .addDialogue(Characters::EVA, "(Baru pertama aja udah bikin overwhelming)")
        .setNextScene(Scenes::PROLOG_13)
    );

    scenes.push_back(
        Scene(
            "Prolog_13",
            "Sesampainya di rumah, aku melihat ponselku dan mendapati Galang mengirimiku pesan.\n"
            "Entah bagaimana ada perasaan yang mulai tumbuh dari dalam diriku.\n"
            "Perasaan berbunga-bunga ketika dia menghubungiku.\n"
            "Ketika dia mengajariku..., ketika dia tersenyum..."
        )
        .addChat(Characters::GALANG, "Aku tahu kamu masih banyak belum pahamnya, jangan patah semangat yaa\nKalo ada yang mau ditanyain, tanyain aja, jangan sungkan")
        .setPrompt("Setelah mendengar dukungan dan belajar bersama dengan Galang, perasaanku:")
        .addChoice("Percaya diri", Scenes::PROLOG_14)
        .addChoice("Tetap ragu-ragu", Scenes::PROLOG_14)
        .addChoice("Mempertimbangkan pindah jurusan", Scenes::PROLOG_6B)
    );

    scenes.push_back(
        Scene(
            "Prolog_14",
            "Aku dan Galang semakin rutin belajar bersama.\n"
            "Dia berusaha keras untuk membuatku mengerti, tetapi aku merasa banyak belum pahamnya.\n"
            "Namun dia tetap berusaha menjelaskan pelan-pelan materinya.\n"
            "Dia tidak memaksaku untuk menjadi orang lain, tidak memaksaku untuk segera mengambil pilihan.\n"
            "Perasaanku semakin tumbuh dan kuat sejak pertama belajar dengannya hingga saat ini.\n\n"
            "Tak terasa minggu depan, sudah ujian pertama programming\n"
            "Saat ini aku sedang bersama Galang untuk belajar terakhir kalinya sebelum ujian minggu depan"
        )
        .addDialogue(Characters::EVA, "Gak kerasaa ya minggu depan udah ujian aja", "Sebenernya kerasa banget woilah!")
        .addDialogue(Characters::GALANG, "Real, cepet banget, kita udah beberapa kali belajar bareng")
        .addDialogue(Characters::GALANG, "Gimana kamu udah merasa lebih banyak tau nggak?")
        .addDialogue(Characters::EVA, "Iyaa lumayan lahh, makasih yaa")
        .addDialogue(Characters::GALANG, "my pleasure, semangatt yaa persiapan ujiannya")
        .setNextScene(Scenes::PERASAAN_1)
    );

    scenes.push_back(
        Scene(
            "Perasaan_1",
            "Seminggu kemudian, hari pertama ujian pun dimulai\n"
            "Kala itu aku terbangun dengan perasaan yang campur aduk. Aku masih merasa tidak yakin dengan pilihanku.\n"
            "Panik, takut, cemas, khawatir, semua bercampur aduk mengganggu pikiranku.\n"
            "Aku mengambil ponselku dan melihat pesan dari seseorang yang tidak aku duga"
        )
        .setPrompt("Apa yang akan aku lakukan?")
        .addChoice("Membalas dengan semangat tinggi dan merasa termotivasi", Scenes::PERASAAN_2A)
        .addChoice("Membalas dengan cemas dan masih meragukan diri sendiri", Scenes::PERASAAN_2B)
        .addChoice("Tidak membuka pesan", Scenes::PERASAAN_2C)
    );

    scenes.push_back(
        Scene(
            "Perasaan_2A",
            ""
        )
        .addChat(Characters::GALANG, "Halo, pagi, Eva, gimana perasaan kamu hari ini? Jangan overthinking yaa,\naku yakin kamu bisa. Kalo butuh bantuan, bilang aja ya")
        .addChat(Characters::EVA, "Percaya diri banget inii, semoga gampang lah nanti soalnya hahahha")
        .setNextScene(Scenes::PERASAAN_3)
    );

    scenes.push_back(
        Scene(
            "Perasaan_2B",
            ""
        )
        .addChat(Characters::GALANG, "Halo, pagi, Eva, gimana perasaan kamu hari ini? Jangan overthinking yaa,\naku yakin kamu bisa. Kalo butuh bantuan, bilang aja ya")
        .addChat(Characters::EVA, "Takut banget aku, aku takut gak bisa ngerjain soal yang dikasih")
        .setNextScene(Scenes::PERASAAN_3)
    );

    scenes.push_back(
        Scene(
            "Perasaan_2C",
            "Aku tidak membuka pesannya dan segera bersiap untuk berangkat ke kampus.\n"
            "Selama perjalanan ke kampus, aku sangat mencemaskan ujiannya. Khawatir akan mendapatkan nilai yang buruk.\n"
            "Aku terus memikirkan bagaimana aku bisa mengerjakan ujiannya, menakutkan sekali ujian programming ini.\n"
        )
        .addDialogue(Characters::EVA, "(Aku merasa ragu dengan diriku sendiri)")
        .addDialogue(Characters::EVA, "(Bener gak yaa aku milih untuk stay disini)")
        .addDialogue(Characters::EVA, "(Aku takut mengecewakan kedua orang tuaku)")
        .setNextScene(Scenes::PERASAAN_4B)
    );

    scenes.push_back(
        Scene(
            "Perasaan_3",
            "Setelah membalas pesan dari Galang, aku segera bergegas berangkat ke kampus.\n"
            "Selama perjalanan ke kampus, aku merasa semangat dan tenang apalagi setelah dia memberikanku dukungan.\n"
            "Pesan sederhana dari Galang, namun mampu membuatku percaya diri dengan kemampuanku\n"
            "Aku merasa aku bisa mengerjakan ujian itu, melewati tantangan yang mungkin dulunya berat bagiku"
        )
        .setNextScene(Scenes::PERASAAN_4A)
    );

    scenes.push_back(
        Scene(
            "Perasaaan_4A",
            "Sesampainya di kampus, aku melihat Galang sudah berada di sana. Dengan tenang melihat layar komputernya,\n"
            "matanya terfokus pada satu titik.\n"
            "Namun begitu dia melihatku, dia tersenyum, senyuman itu membuat hatiku berdebar."
        )
        .setPrompt("Apa yang akan aku lakukan?")
        .addChoice("Menyapanya dengan senyuman hangat", Scenes::PERASAAN_5A)
        .addChoice("Membahas materi ujian untuk meminta bantuan", Scenes::PERASAAN_5B)
    );

    scenes.push_back(
        Scene(
            "Perasaaan_4B",
            "Sesampainya di kampus, aku melihat Galang sudah berada di sana. Dengan tenang melihat layar komputernya,\n"
            "matanya terfokus pada satu titik.\n"
            "Namun begitu dia melihatku, dia tersenyum, senyuman itu membuat hatiku berdebar.\n"
            "Namun, karena perasaanku begitu campur aduk, aku menghindari\n"
            "kontak dengan Galang. Bahkan aku tidak membalas senyumannya, memalingkan pandanganku,\nke arah tempat duduk yang telah diatur.\n\n"
            "Menunggu waktu ujian dimulai, aku merasa semakin cemas, ketakutan menguasai diriku saat itu.\nBingung akan apa yang sedang terjadi.\n"
        )
        .addDialogue(Characters::EVA, "(Waduh gimana ini yaaa)")
        .addDialogue(Characters::EVA, "(Yang lainnya belajarnya gimana yaa)")
        .addDialogue(Characters::EVA, "(Kok mereka kayak tenang-tenang aja sihh)")
        .setPrompt("Apa yang akan aku lakukan?")
        .addChoice("Berbicara dengan Galang", Scenes::PERASAAN_5B)
        .addChoice("Diam saja", Scenes::PERASAAN_5C)
    );

    scenes.push_back(
        Scene(
            "Perasaan_5A",
            "Aku membalas senyumannya dengan hangat dan menghampirinya sebelum ujian dimulai.\nSedikit mengobrol dan berdiskusi materi ujian"
        )
        .addDialogue(Characters::EVA, "Tenang banget nih keliatannya")
        .addDialogue(Characters::GALANG, "Harus dong")
        .addDialogue(Characters::EVA, "Bisa jelasin ulang beberapa materi sebentar enggak?")
        .addDialogue(Characters::GALANG, "Tanyain ajaa")
        .setNextScene(Scenes::PERASAAN_7)
    );

    scenes.push_back(
        Scene(
            "Perasaan_5B",
            "Aku segera menghampirinya sebelum ujian dimulai. Aku menanyakan beberapa materi ujian yang belum aku pahami"
        )
        .addDialogue(Characters::EVA, "Bisa jelasin ulang beberapa materi sebentar enggak, Lang?")
        .addDialogue(Characters::GALANG, "Tanyain ajaa")
        .setNextScene(Scenes::PERASAAN_7)
    );

    scenes.push_back(
        Scene(
            "Perasaan_5C",
            "Aku berusaha meyakinkan diriku agar aku bisa tenang dan tidak terbawa kepanikan sehingga\naku bisa mengerjakan ujianku dengan baik"
        )
        .addDialogue(Characters::EVA,"(Aku tau aku bisa)")
        .addDialogue(Characters::EVA,"(Aku udah belajar keras semalam)")
        .addDialogue(Characters::EVA,"(Aku udah belajar sama Galang)")
        .addDialogue(Characters::EVA,"(Itu semua pasti ada positifnya)")
        .addDialogue(Characters::EVA,"(Aku pasti bisa)")
        .setNextScene(Scenes::PERASAAN_6)
    );

    scenes.push_back(
        Scene(
            "Perasaan_6",
            "Ujianpun dimulai, aku mengerjakan semua soalnya semampuku...\n"
            "Dua jam berlalu, setelah ujian selesai, aku termenung, meratapi ketidakmampuanku dalam mengerjakan soal\n"
            "Tidak seperti teman-teman yang lain, aku tidak bisa berbahagia setelah ujian selesai.\n"
            "Pikiranku penuh dengan pertimbangan untuk pindah jurusan."
        )
        .setNextScene(Scenes::PROLOG_6B)
    );

    scenes.push_back(
        Scene(
            "Perasaan_7",
            "Setelah berdiskusi dan mengobrol panjang lebar dengan Galang, ujianpun akan segera dimulai.\n" 
            "Sehingga aku harus segera kembali ke posisi dudukku dan bersiap mengerjakan ujian.\n"
            "Aku merasa tenang, merasa yakin dengan diriku sendiri"
        )
        .setPrompt("Bagaimana ppenilaianku terhadap Galang?")
        .addChoice("Dia benar-benar orang yang peduli", Scenes::PERASAAN_8A)
        .addChoice("Dia punya perasaan yang sama denganku", Scenes::PERASAAN_8A)
        .addChoice("Dia baik hanya karena kasihan", Scenes::PERASAAN_8B)
    );

    scenes.push_back(
        Scene(
            "Perasaan_8A",
            "Dua jam berlalu, ujian pun sudah selesai, aku merasa cukup lega dengan ujian kali ini.\n"
            "Walaupun masih belum maksimal, setidaknya aku sudah tidak sekosong itu.\n"
            "Aku mengobrol sebentar dengan Alifia dan Yunita yang kala itu juga sedang membahas tentang\nbagaimana ujian mereka.\n"
            "Galang sudah keluar ruangan sejak setengah jam yang lalu, sepertinya ujian ini mudah baginya.\n"
            "Namun, saat aku keluar ruangan, aku melihatnya duduk di kursi depan ruangan seperti sedang menunggu seseorang."
        )
        .setPrompt("Apa yang akan aku lakukan?")
        .addChoice("Menghampirinya dan mengajak diskusi tentang ujian", Scenes::PERASAAN_9A)
        .addChoice("Memberitahunya bahwa aku akan belajar sendiri saja mulai saat ini", Scenes::PERASAAN_9B)
    );

    scenes.push_back(
        Scene(
            "Perasaan_8B",
            "Dua jam berlalu, ujian pun sudah selesai, aku merasa cukup lega dengan ujian kali ini.\n"
            "Walaupun masih belum maksimal, setidaknya aku sudah tidak sekosong itu.\n"
            "Aku mengobrol sebentar dengan Alifia dan Yunita yang kala itu juga sedang membahas tentang\nbagaimana ujian mereka.\n"
            "Galang sudah keluar ruangan sejak setengah jam yang lalu, sepertinya ujian ini mudah baginya.\n"
            "Namun, saat aku keluar ruangan, aku melihatnya duduk di kursi depan ruangan\nseperti sedang menunggu seseorang."
        )
        .setNextScene(Scenes::PERASAAN_9B)
    );

    scenes.push_back(
        Scene(
            "Perasaan_9A",
            "Aku menghampirinya dan mengajaknya berbincang tentang ujian.\nAku berharap jawabanku sama dengan jawabannya."
        )
        .addDialogue(Characters::EVA, "Eh, Lang. Masih disini ajaa")
        .addDialogue(Characters::GALANG, "akhirnya yang ditunggu keluar juga. Gimana ujian kamu?")
        .addDialogue(Characters::EVA, "hah? kamu nungguin aku? ngapainn ih...")
        .addDialogue(Characters::EVA, "Ujiannya sih masih lumayan susah, tapi oke lahh gak kosong banget lembar jawabanku hahahah")
        .addDialogue(Characters::GALANG, "Baguslahh kalo begitu, berarti sesi belajar ada progressnya juga")
        .addDialogue(Characters::NARRATOR, "Aku dan Galang pun membahas soal demi soal mencari tahu jawaban yang lebih tepat.\nSetelah semuanya selesai, dia bertanya kepadaku,")
        .addDialogue(Characters::GALANG, "Jadi, gimana masih ragu nerusin kuliah di sini?")
        .addDialogue(Characters::EVA, "...", "tiba-tiba sekali dia bertanya seperti itu")
        .setPrompt("Bagaimana aku menjawabnya?")
        .addChoice("Jujur dengan bilang aku merasa terjebak", Scenes::PERASAAN_10)
        .addChoice("Tersenyum saja", Scenes::PERASAAN_9B)
    );

    scenes.push_back(
        Scene(
            "Perasaan_9B",
            "Aku hanya diam dan berusaha mengalihkan pembicaraan. Galang pun sepertinya menyadari\nkalau aku terbebani dengan apa yang dia katakan.\n"
            "Percakapan selanjutnya, terasa menjadi canggung. Sepertinya dia merasa gagal mengajariku\natau mungkin dia tidak ingin membebaniku lagi.\n"
            "Entah lah apa yang ada dipikirannya. Namun, aku segera pulang setelah merasa canggung dengannya."
        )
        .setNextScene(Scenes::PERASAAN_11)
    );

    scenes.push_back(
        Scene(
            "Perasaan_10",
            "aku jujur akan perasaanku kepada Galang. Aku jujur bahwa aku masih merasa terjebak.\nAku merasa tidak cocok dengan jurusan ini\n"
            "Coding sangat membuatku merasa asing di dunia ini. Semuanya ku ungkapkan kepada Galang,\nkecuali perasaanku pada dirinya"
        )
        .addDialogue(Characters::GALANG, "Begitu yaa, merasa tidak layak berada di sini, semuanya berjalan tidak sesuai keinginanmu")
        .addDialogue(Characters::GALANG, "Boleh aku ngasih bahan pertimbangan ke kamu")
        .addDialogue(Characters::EVA, "Boleh..")
        .addDialogue(Characters::GALANG, "Aku yakin itu wajar, banyak yang merasa begitu di jurusan manapun itu")
        .addDialogue(Characters::GALANG, "You've tried to give the best of you. Itu udah cukup, gak harus ngejar standar lain")
        .addDialogue(Characters::GALANG, "Kamu cuma butuh waktu lebih..")
        .addDialogue(Characters::GALANG, "Cobalah sekali lagi, aku yakin kamu bisa lebih baik dari yang sekarang")
        .addDialogue(Characters::GALANG, "Sekarang kita pulang aja, istirahat dulu")
        .setPrompt("Mendengar nasihat dari Galang, aku :")
        .addChoice("Merenungkan kata-katanya", Scenes::KERAGUAN_1)
        .addChoice("Merasa terbebani", Scenes::PERASAAN_9B)
    );

    scenes.push_back(
        Scene(
            "Perasaan_11",
            "Sejak saat itu, aku jauh darinya, tak kubalas pesan-pesannya lagi, hingga akhirnya kami seperti orang asing.\n"
            "Aku dan dia sudah tidak saling menyapa lagi, tidak melakukan sesi belajar lagi,\nkini aku dan dia sudah berbeda jalan. "
            "Aku kembali sendiri dengan perasaan ini. Tak ada lagi yang memberiku dukungan"
        )
        .setNextScene(Scenes::ENDING_1)
    );

    scenes.push_back(
        Scene(
            "Keraguan_1",
            "Aku merenungkan kata-katanya selama perjalanan pulang, sesampainya di rumah, dalam segala aktivitasku.\n"
            "Aku mencoba mencari jawaban atas keraguanku sendiri, aku bingung harus berbuat apa..."
        )
        .addDialogue(Characters::EVA, "(Sudah seminggu ya, aku belum bisa memutuskan apakah aku mau melanjutkan kuliahku)")
        .addDialogue(Characters::EVA, "(Aku juga bingung, apakah aku harus mengungkapkan perasaanku kepada Galang)")
        .addDialogue(Characters::EVA, "(Perasaan ini sudah sangat menggelora, aku sangat ingin menyampaikannya)")
        .setPrompt("Langkah apa yang akan aku ambil?")
        .addChoice("Mencari nasihat dari orang lain selain Galang", Scenes::KERAGUAN_2A)
        .addChoice("Introspeksi diri dahulu", Scenes::KERAGUAN_2B)
    );

    scenes.push_back(
        Scene(
            "Keraguan_2A",
            "Sesi belajar dengan Galang selalu menjadi salah satu waktu favoritku, karena\naku bisa merasakan ketenangan ketika di dekatnya.\n"
            "Dia selalu memberiku dukungan dan nasihat terkait keraguanku ini. Membantuku keluar dari permasalahan ini.\n"
            "Namun, aku rasa aku tidak boleh hanya mendengarkan atau mendapatkan nasihat darinya,\naku juga harus mencari "
            "pendapat-pendapat lain dari temen-temenku."
        )
        .addChat(Characters::EVA, "Alifia, kamu lagi sibuk nggak, aku mau cerita\n")
        .addChat(Characters::EVA, "Yunita, aku mau ceritaaa")
        .addChat(Characters::ALIFIA, "kamu cerita dulu aja, aku lagi sibuk, nanti aku balas")
        .addChat(Characters::YUNITA, "kenapa Eva?")
        .addChat(Characters::EVA, "Aku selama ini merasa salah jurusan. Aku kayak gak cocok sama jurusan ini.\nTapi aku juga bimbang mau ninggalin jurusan ini, sayang waktuku yang udah kuhabisin disini\n")
        .addChat(Characters::EVA, "Aku udah belajar bareng sama Galang, udah belajar mandiri, tapi susah banget materinya masuk ke aku")
        .addChat(Characters::YUNITA, "Kamu merasa ketinggalan sama temen-temen yang lain ya? Hmmz, tapi emang jurusan ini itu secepet itu sihh gerakannya")
        .addChat(Characters::YUNITA, "Kamu selama ini gimana waktu belajar sama Galang?")
        .addChat(Characters::EVA, "Iya aku merasa ketinggalan banget. Selama ini belajar ya dia jelasinnya pelan sih, cuman akunya aja yang gak paham-paham")
        .addChat(Characters::YUNITA, "Kita samaan sihh, tapi nggak sampe ngerasa kayak kamu.\nSemangat Sayang! aku ada disini kalo mau butuh tempat cerita\n")
        .addChat(Characters::ALIFIA, "Hahahaha kita sama banget tauu, aku juga ngerasa gituu, kamu gak sendirian\n")
        .addChat(Characters::ALIFIA, "Mau aku kenalin nggak sama temen codingku?")
        .addChat(Characters::EVA, "Siapa?")
        .addChat(Characters::ALIFIA, "Musa, dia kalo dimintai jawaban langsung ngasih gitu sukanya,\nnilaiku jadi bagus-bagus di tugas.")
        .addChat(Characters::ALIFIA, "Nih kontaknya (ascii art phone card)")
        .setNextScene(Scenes::KERAGUAN_3A)
    );

    scenes.push_back(
        Scene(
            "Keraguan_2B",
            "Aku memutuskan untuk memberi ruang kepada diriku sendiri, menjauh dari Galang untuk sementara waktu\n"
            "Berusaha memikirkan semua solusinya untuk sementara waktu.\n"
            "Tak terasa sudah hampir seminggu, aku menjauh dari Galang dan memikirkan solusinya.\nAku menyimpulkan beberapa langkah yang bisa aku ambil.."
        )
        .setPrompt("Langkah mana yang akan aku ambil?")
        .addChoice("Berusaha sendirian seterusnya", Scenes::PERASAAN_10)
        .addChoice("Tetap meminta bantuan Galang ketika aku kesulitan", Scenes::KERAGUAN_3B)
        .addChoice("Mencari sudut pandang dari yang lain", Scenes::KERAGUAN_2A)
    );

    scenes.push_back(
        Scene(
            "Keraguan_3A",
            "Malam harinya aku pun menghubungi temannya Alifia, yang katanya membantu Alifia dapat nilai bagus\ndi matkul programming"
        )
        .addChat(Characters::EVA, "Malam, Musa. aku Eva temennya Alifia. Aku boleh nanya gak?")
        .addChat(Characters::MUSA, "Malam Eva, bolehh")
        .addChat(Characters::EVA, "Kata Alifia, kamu jago ngoding. kamu belajarnya gimana ee?")
        .addChat(Characters::MUSA, "Bisa aja Alifiaa bilang gitu. Hmm, aku sih belajarnya make AI, codeAI gitu")
        .addChat(Characters::EVA, "Gimana ituu cara makenya?")
        .addChat(Characters::NARRATOR, "Dia pun mengajariku hingga aku bisa menggunakan codeAI")
        .addChat(Characters::MUSA, "Tapi kalo kamu butuh bantuan, tanya langsung ke aku juga gapapaa Eva")
        .addChat(Characters::EVA, "Makasihh banyak yaa Musaa")
        .setNextScene(Scenes::KERAGUAN_4)
    );
    
    scenes.push_back(
        Scene(
            "Keraguan_3B",
            "Aku menjadi lebih sering melakukan sesi belajar dengan Galang. Perasaanku semakin kuat hari demi harinya\n"
            "Ingin sekali kuungkapkan perasaanku kepada Galang. Namun, aku takut dia pergi dan tidak ada yang membantuku lagi\n"
        )
        .setNextScene(Scenes::KECEMBURUAN_1)
    );

    scenes.push_back(
        Scene(
            "Keraguan_4",
            "Hari demi hari, perasaanku kepada Galang semakin kuat. Dia berada di tengah-tengah kebingunganku.\n"
            "Membantuku melewati jalan yang tidak mulus ini.\n"
            "Entahlah aku ingin mengungkapkan perasaan ini. Namun, rasa terjebakku ini membuatku bingung\n"
        )
        .addDialogue(Characters::EVA, "(Haruskah aku ungkapkan\?)")
        .addDialogue(Characters::EVA, "(Namun bagaimana jika dia pergi)")
        .addDialogue(Characters::EVA, "(Perlukah aku menyingkirkan perasaan ini)")
        .setPrompt("Seperti apa penilaianku kepada Galang saat ini: ")
        .addChoice("Inspirasiku untuk terus maju", Scenes::KECEMBURUAN_1)
        .addChoice("Orang yang semakin berharga bagiku", Scenes::KECEMBURUAN_1)
        .addChoice("Pengingat betapa buruknya diriku dan menjauh darinya", Scenes::PERASAAN_10)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_1",
            "Kini, perasaan cemburu mulai tumbuh ketika aku melihat Galang berbicara dengan gadis lain.\n"
            "Perasaan tidak nyaman yang sulit aku jelaskan. Sedangkan kala itu, sering sekali aku melihatnya\nberbicara dengan gadis lain.\n"
            "Sore itu, aku melihatnya sedang bersama gadis lain, mereka terlihat sangat dekat,\nGalang bisa tertawa lepas ketika bersama dia."
        )
        .addDialogue(Characters::EVA, "(Apa aku takut kehilangan dia? Apa aku mulai peduli dengannya\?)")
        .addDialogue(Characters::EVA, "(Siapa gadis yang terlihat dekat itu ya)")
        .addDialogue(Characters::EVA, "(Kenapa aku memikirkannya sih)")
        .addDialogue(Characters::EVA, "(Kenapa aku berharap ya)")
        .setPrompt("Apa yang akan aku lakukan?")
        .addChoice("Bertanya langsung", Scenes::KECEMBURUAN_2A)
        .addChoice("Diam tapi terganggu", Scenes::KECEMBURUAN_2B)
        .addChoice("Menghindarinya", Scenes::KECEMBURUAN_2B)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_2A",
            "Aku segera pergi dari sana agar tidak dinotice oleh Galang.\n"
            "Segera setelah menemukan tempat untuk duduk, aku mengiriminya pesan dan langsung menanyakan\ntentang siapa gadis tadi.\n"
            "Lama sekali menunggu jawaban darinya, hampir 3 jam dia tidak membalasku..."
        )
        .addChat(Characters::EVA, "Lang, tadi aku liat kamu sama cewek, kalian keliatan akrab gitu, jadi gak enak mau ganggu")
        .addChat(Characters::EVA, "(Lama banget ni anak belum balas padahal udah 3 jam yang lalu)")
        .addChat(Characters::GALANG, "eh tiba-tiba banget nanyain temenku")
        .addChat(Characters::GALANG, "itu namanya Lea, dia temen waktu ospek dulu")
        .addChat(Characters::GALANG, "Emang kamu tadi liatin aku darimana dehh, kok aku gak liat ada kamu")
        .addChat(Characters::EVA, "(Ohh temen ospek, aku tidak mau langsung membalas pesannya)")
        .setNextScene(Scenes::KECEMBURUAN_3)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_2B",
            "Sejak hari itu, aku bersikap aneh kepadanya. Aku tidak bisa bersikap seperti biasanya. Hingga akhirnya\n"
            "dia menyadari hal ini dan menanyaiku tentang hal ini. Namun, aku tidak mau bercerita kepadanya,\n"
            "aku selalu diam atau mengalihkan pembicaraan agar dia tidak membahas sikapku ini."
        )
        .setNextScene(Scenes::KECEMBURUAN_3)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_3",
            "Rasa cemburu itu tidak bisa aku hapus begitu saja. Itu sungguh menggangguku. Aku harus berbuat sesuatu\n"
        )
        .setPrompt("Apa yang akan aku lakukan untu mengatasi rasa ini?")
        .addChoice("Fokus dengan studi", Scenes::KECEMBURUAN_4A)
        .addChoice("Motivasi untuk mendekati Galang", Scenes::KECEMBURUAN_5)
        .addChoice("Cerita ke temanku", Scenes::KECEMBURUAN_4B)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_4A",
            "Untuk mendistraksi diri dari rasa cemburu itu, aku fokus belajar coding.\n"
            "Siang malam kugunakan untuk belajar. Kuhabiskan waktuku untuk berkegiatan agar aku tidak memikirkan hal itu.\n"
            "Walau sesekali masih terpikirkan, aku memilih untuk segera melakukan kegiatan apapun itu sehingga\ntidak ada waktu bagiku untuk cemburu. "
            "Hingga aku bosan belajar sendiri"
        )
        .setNextScene(Scenes::KECEMBURUAN_5)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_4B",
            "Sejak aku kenal Musa, aku sering chat dengannya, bercerita tentang hidup maupun membahas coding.\n"
            "Akupun memutuskan untuk cerita kepada dia, tentang aku yang memiliki rasa cemburu\ntetapi aku tidak menyebutkan siapa cowok itu.\n"
            "Musa pun menanggapi ceritaku, memberiku perhatian hingga aku tidak memikirkan hal itu lagi"
        )
        .setNextScene(Scenes::KECEMBURUAN_6)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_5",
            "Karena bosan belajar secara mandiri, akupun mengajak Galang belajar bersama untuk persiapan ujian berikutnya\n"
            "Galang pun menyanggupinya dan akhirnya keesokan harinya aku bertemu kembali dengannya.\n"
            "Keesokan harinya kami pun belajar bersama. Aku tidak bisa fokus ketika belajar saat itu.\n"
            "Rasa suka, cemburu, dan keinginan untuk belajar saling tumpang tindih sehingga membuyarkan fokusku"
        )
        .setNextScene(Scenes::KECEMBURUAN_6)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_6",
            "Galang menyadari hal ini sehingga dia sekali lagi menanyakan kenapa sikapku seperti tidak fokus\ndan bersikap tidak seperti biasanya\n"
        )
        .addDialogue(Characters::GALANG, "Kamu kenapa Eva?")
        .addDialogue(Characters::GALANG, "Daritadi aku perhatiin, kamu kayak gak fokus")
        .addDialogue(Characters::GALANG, "Lagi ada masalah kah? atau lagi kenapa?")
        .setPrompt("Apa yang akan aku lakukan")
        .addChoice("Jujur kepada Galang", Scenes::KECEMBURUAN_7A)
        .addChoice("Berpura-pura tidak ada yang salah", Scenes::KECEMBURUAN_7B)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_7A",
            ""
        )
        .addDialogue(Characters::EVA, "Hmm gimana ya ngomongnyaa")
        .addDialogue(Characters::EVA, "Aku emang lagi kepikiran sesuatu, dibilang masalah, mungkin iya, mungkin bukan.")
        .addDialogue(Characters::EVA, "dibilang gak masalah, tapi kepikiran. Kayaknya nanti dulu dehh ceritanyaa")
        .addDialogue(Characters::GALANG, "Kenapa enggak mau cerita?")
        .addDialogue(Characters::EVA, "Gak mau aja")
        .addDialogue(Characters::EVA, "Nanti yaa, ada waktunya, aku kasih tau, sekarang kita fokus dulu belajar")
        .setNextScene(Scenes::PERJALANAN_1)
    );

    scenes.push_back(
        Scene(
            "Kecemburuan_7B",
            ""
        )
        .addDialogue(Characters::EVA, "Orang biasa aja")
        .addDialogue(Characters::EVA, "Lagi fokus belajar mungkin jadi keliatan mikir", "fokus aja lah jangan bahas itu please Galang, kupukul juga nih")
        .addDialogue(Characters::GALANG, "Enggak sih, kamu kayak lagi mikirin sesuatu yang bikin kamu gak fokus justru")
        .addDialogue(Characters::GALANG, "Belum mau cerita ya? Yaudah lanjutin belajarnya aja ya")
        .setNextScene(Scenes::PERJALANAN_1)
    );

    scenes.push_back(
        Scene(
            "Perjalanan_1",
            "Setelah seharian belajar sama Galang, aku pun pulang.\n"
            "Se-sampainya di rumah, aku kembali merenungkan tentang keraguanku terhadap jurusan ini.\n"
            "Memang aku sudah berkembang dari segi pemahaman. Namun, perasaan tertinggal oleh yang lainnya itu menghantuiku\n"
            "Aku mempertimbangkan kembali apa pilihanku, kali ini aku harus benar-benar komitmen dengan pilihanku."
        )
        .setPrompt("Masa depan kuliahku, apa yang akan aku pilih?")
        .addChoice("Lanjut dengan percaya diri", Scenes::PERJALANAN_2A)
        .addChoice("Mengambil cuti kuliah", Scenes::ENDING_3A)
        .addChoice("Mencoba hal baru", Scenes::PERJALANAN_2B)
    );

    scenes.push_back(
        Scene(
            "Perjalanan_2A",
            "Aku memantapkan diri untuk melanjutkan kuliahku sebaik-baiknya. Aku harus sampai lulus\n"
            "Aku tidak ingin menyia-yiakan orang yang telah membantu dan mendukungku selama ini.\n"
            "Aku tidak mau mengecewakan mereka. Namun, aku bingung dengan perasaanku kepada Galang"
        )
        .addDialogue(Characters::EVA, "(Haruskah aku ungkapkan perasaanku kepada dia)")
        .addDialogue(Characters::EVA, "(Aku harus membuat sebuah keputusan)")
        .addDialogue(Characters::EVA, "(Bagaimana caraku mengungkapkannya)")
        .addDialogue(Characters::EVA, "(Ini akan menjadi awal dari perjalananku)")
        .setPrompt("Bagaimana caraku mengungkapkannya?")
        .addChoice("Dengan cara halus sampai dia notice", Scenes::ENDING_3A)
        .addChoice("Secara langsung", Scenes::ENDING_5)
    );

    scenes.push_back(
        Scene(
            "Perjalanan_2B",
            "Walaupun coding tidak sesulit dulu, pada akhirnya aku memutuskan untuk keluar dari jurusan Sistem Informasi\n"
            "Akhirnya aku tunduk pada rasa keraguan itu. Mencoba hal baru di luar sana.\nTapi apakah aku harus mengungkapkan perasaanku kepada Galang?\n"
        )
        .setPrompt("Haruskah aku ungkapkan?")
        .addChoice("Dengan cara halus sampai dia notice", Scenes::ENDING_4)
        .addChoice("Untuk apa?", Scenes::ENDING_2B)
    );

    scenes.push_back(
        Scene(
            "Ending_1",
            "Aku memutuskan untuk melanjutkan kuliah, belajar semampuku, sebisaku, sendirian.\n"
            "Perjalanan kuliah yang sangat tidak menyenangkan. Setiap langkah ditempatkan dalam ruang kebingungan.\n"
            "Sepi sekali rasanya perjalanan kuliahku. Akhirnya setelah 5 tahun penuh perjuangan,\n\n"
            "AKU LULUS\n\n"
            "Ya walaupun, pada akhirnya nilaiku sangat tidak memuaskan.\n"
            "Namun yang terpenting, aku bisa keluar dari penjara ini!"
        )
        .setNextScene(Scenes::END_TITLE)
    );
    
    scenes.push_back(
        Scene(
            "Ending_2A",
            "Pada akhirnya rasa ketidakyakinan itu berubah menjadi sebuah keyakinan.\n"
            "Aku semakin yakin untuk pindah jurusan. Memilih perjalanan hidup lain.\n"
            "Aku mengejar impianku dulu, mendaftar kedinasan, STIS -- seperti apa yang orang tuaku juga inginkan dahulu\n"
            "SEMOGA INI MENJADI PERJALANAN YANG LEBIH BAIK"
        )
        .setNextScene(Scenes::END_TITLE)
    );

    scenes.push_back(
        Scene(
            "Ending_2B",
            "Perasaanku mungkin memiliki makna, tapi jika jauh darinya. Aku rasa aku tak akan bisa menjalaninya\n"
            "Pada akhirnya rasa ketidakyakinan itu berubah menjadi sebuah keyakinan.\n"
            "Aku semakin yakin untuk pindah jurusan. Memilih perjalanan hidup lain.\n"
            "Keputusanku sudah bulat, tak akan aku ungkapkan perasaan ini.\n"
            "Aku akan mengejar impianku dulu, mendaftar kedinasan, STIS -- seperti apa yang orang tuaku juga inginkan dahulu\n"
            "SEMOGA INI MENJADI PERJALANAN YANG LEBIH BAIK"
        )
        .setNextScene(Scenes::END_TITLE)
    );

    scenes.push_back(
        Scene(
            "Ending_3A",
            "Dengan keputusanku ini, aku pun mengungkapkan perasaanku kepada Galang dengan cara halus. Berharap dia akan notice\n"
            "Hari demi hari, aku lalui bersama dirinya. Menahan perasaan yang semakin menggelora di dalam jiwa\n"
            "Aku selalu berusaha memberinya kode-kode kecil setiap kali bertemu dengannya. Chatku dengannya juga semakin intense\n\n"
            "Aku semakin dekat dengannya, tak terasa sudah lebih dari satu semester aku bersama dia.\n"
            "Aku juga menjadi lebih siap untuk kuliah di sini, jauh lebih baik dibandingkan sebelumnya..\n"
            "Kini coding terasa lebih mudah.\n\n"
            "Namun, dia tak kunjung peka dengan perasaanku. Tak ada kejelasan tentang hubunganku dengan dia\n"
            "Walaupun menyakitkan, aku tetap memilih untuk melanjutkan kuliah tanpa mengungkapkan perasaanku"
        )
        .setNextScene(Scenes::END_TITLE)
    );

    scenes.push_back(
        Scene(
            "Ending_3B",
            "Dengan keputusanku ini, aku pun mengungkapkan perasaanku kepada Galang dengan cara halus. Berharap dia akan notice\n"
            "Hari demi hari di perkuliahan, aku lalui bersama dirinya. Menahan perasaan yang semakin menggelora di dalam jiwa\n"
            "Aku selalu berusaha memberinya kode-kode kecil setiap kali bertemu dengannya. Chatku dengannya juga semakin intense\n\n"
            "Aku semakin dekat dengannya, tak terasa sudah satu semester aku bersama dia.\n"
            "Kuliahku juga semakin tertata, jauh lebih baik dibandingkan sebelumnya..\n"
            "Kini coding terasa lebih mudah.\n\n"
            "Namun, dia tak kunjung peka dengan perasaanku. Tak ada kejelasan tentang hubunganku dengan dia\n"
            "Walaupun menyakitkan, aku tetap memilih untuk melanjutkan kuliah tanpa mengungkapkan perasaanku"
        )
        .setNextScene(Scenes::END_TITLE)
    );

    scenes.push_back(
        Scene(
            "Ending_4",
            "Keputusanku untuk pindah jurusan sudah bulat.\n"
            "Namun, aku tidak bisa melepaskan perasaan ini begitu saja. Aku harus tau perasaanya juga.\n"
            "Sore itu, hari terakhirku di kuliah. Aku menemuinya di taman, tempatku dan dia sering bertemu.\n"
            "Aku mengungkapkan keputusanku yang akan pindah jurusan sekaligus mengungkapkan perasaanku kepadanya.\n"
            "Terlihat jelas dia sedih karena aku pindah jurusan.\n"
            "Namun, dia juga senang karena ternyata kita memiliki perasaan yang sama."
        )
        .setNextScene(Scenes::END_TITLE)
    );

    scenes.push_back(
        Scene(
            "Ending_5",
            "Sore itu, setelah kelas, aku mengajaknya ke taman--tempat biasa yang sering kita kunjungi.\n"
            "Di sana aku bercerita dengan gembira kepadanya, bahwa aku akan tetap melanjutkan kuliah.\n"
            "Dia tersenyum kegirangan melihatku yakin dengan keputusan ini, meneruskan kuliahku di sini.\n"
            "Tidak lupa, aku juga menungkapkan perasaanku kepadanya dan ternyata ...\n"
            "Dia juga memiliki perasaan yang sama. Hari itu menjadi hari terbaik bagi kita berdua.\n\n"
            "Awal dari perjalanan indahku berkuliah baru saja dimulai!"
        )
        .setNextScene(Scenes::END_TITLE)
    );

    scenes.push_back(
        Scene(
            "end_title",
            R"ascii(
                 ______   __ __   ____        ____   _  __   ___ 
                /_  __/  / // /  / __/       / __/  / |/ /  / _ \
                 / /    / _  /  / _/        / _/   /    /  / // /
                /_/    /_//_/  /___/       /___/  /_/|_/  /____/ 
            )ascii"
        )
        .setIsTitle(true)
    );
}

void GameManager::loadCharacter() {
    characters.push_back(Character(
        "Eva", 
        "Cewek 19 tahun yang suka scroll TikTok dan tidur, tipe orang yang independen. "
        "Lebih sering introvert, tapi kadang juga ekstrovert. Lagi ngerasa kecewa karena jurusannya nggak sesuai.",
        Color::SKY_MAGENTA,
        false
    ));
    
    characters.push_back(Character(
        "Galang", 
        "Cowok 20 tahun yang suka coding, musik, dan fotografi. Pake kacamata "
        "dan rambutnya model curtain. Orangnya kreatif dan punya rasa ingin tahu tinggi.",
        Color::SAPPHIRE
    ));

    characters.push_back(Character(
        "Mama",
        "Ibu Eva, orang yang paling Eva cintai",
        Color::TURKEY_RED,
        false
    ));

    characters.push_back(Character(
        "Papa",
        "Papa Eva, sang donatur utama dalam hidup Eva",
        Color::WOOD,
        false
    ));

    characters.push_back(Character(
        "Alifia",
        "Cewek 19 tahun lulusan SMK Akuntansi yang super ekstrovert gak kenal malu."
        "Tubuhnya agak berisi tapi masih ideal",
        Color::ORANGE,
        false
    ));

    characters.push_back(Character(
        "Yunita",
        "Cewek botol yakult yang teliti dan introvert, tapi kenalannya banyak",
        Color::VANILLA,
        false
    ));
    
    characters.push_back(Character(
        "Musa",
        "Cowok soft spoken yang katanya jago ngoding",
        Color::VERDIGRIS
    ));

    characters.push_back(Character(
        "Narrator",
        "Narrator",
        Color::DEFAULT
    ));
}