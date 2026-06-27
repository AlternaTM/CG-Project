#include "highscoreManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<ScoreEntry> HighscoreManager::scores;
std::string HighscoreManager::filePath;

void HighscoreManager::load(const std::string& path) {
    filePath = path;
    scores.clear();

    std::ifstream file(filePath);
    if (!file.is_open()) {
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string scoreStr, killsStr;

        if (!std::getline(ss, scoreStr, ';')) continue;
        if (!std::getline(ss, killsStr)) continue;

        ScoreEntry entry;
        entry.score = static_cast<uint32_t>(std::stoul(scoreStr));
        entry.enemiesKilled = static_cast<uint32_t>(std::stoul(killsStr));

        scores.push_back(entry);
    }

    file.close();
}

void HighscoreManager::trySaveScore(uint16_t score, uint16_t enemiesKilled) {
    ScoreEntry entry{ score, enemiesKilled };

    auto it = std::find_if(scores.begin(), scores.end(), [&](const ScoreEntry& e) {
        return e.score < entry.score;
    });

    scores.insert(it, entry);

    if (scores.size() > 5) {
        scores.pop_back();
    }

    save();
}

void HighscoreManager::save() {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "ERRORE: impossibile salvare highscores in " << filePath << std::endl;
        return;
    }

    for (const auto& entry : scores) {
        file << entry.score << ";" << entry.enemiesKilled << "\n";
    }

    file.close();
}

const std::vector<ScoreEntry>& HighscoreManager::get_high_scores() {
    return scores;
}