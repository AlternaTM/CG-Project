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
        std::string scoreStr, killsStr, nameStr;

        if (!std::getline(ss, scoreStr, ';')) continue;
        if (!std::getline(ss, killsStr, ';')) continue;
        if (!std::getline(ss, nameStr)) continue;

        ScoreEntry entry;
        entry.score = static_cast<uint32_t>(std::stoul(scoreStr));
        entry.enemiesKilled = static_cast<uint32_t>(std::stoul(killsStr));
        entry.name = nameStr;

        scores.push_back(entry);
    }

    file.close();
}

int HighscoreManager::getRank(uint16_t score) {
    for (int i = 0; i < (int)scores.size(); i++) {
        if (score > scores[i].score) return i;
    }
    if ((int)scores.size() < 5) return (int)scores.size();
    return -1;
}

void HighscoreManager::saveScore(uint16_t score, uint16_t enemiesKilled, const std::string& name) {
    ScoreEntry entry{ score, enemiesKilled, name };

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
        file << entry.score << ";" << entry.enemiesKilled << ";" << entry.name << "\n";
    }

    file.close();
}

const std::vector<ScoreEntry>& HighscoreManager::get_high_scores() {
    return scores;
}