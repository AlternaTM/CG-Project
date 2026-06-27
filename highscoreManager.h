#pragma once

#include <vector>
#include <string>

struct ScoreEntry {
    uint16_t score;
    uint16_t enemiesKilled;
};

class HighscoreManager {
public:
    static void load(const std::string& path = "./assets/savefiles/highscore.txt");
    static void trySaveScore(uint16_t score, uint16_t enemiesKilled);
    static const std::vector<ScoreEntry>& get_high_scores();

private:
    static std::vector<ScoreEntry> scores;
    static std::string filePath;

    static void save();
};