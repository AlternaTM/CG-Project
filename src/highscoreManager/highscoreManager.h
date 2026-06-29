#pragma once

#include <vector>
#include <string>

struct ScoreEntry {
    uint16_t score;
    uint16_t enemiesKilled;
    std::string name;
};

class HighscoreManager {
public:
    static void load(const std::string& path = "./assets/savefiles/highscore.txt");
    static int  getRank(uint16_t score);
    static void saveScore(uint16_t score, uint16_t enemiesKilled, const std::string& name);
    static const std::vector<ScoreEntry>& get_high_scores();

private:
    static std::vector<ScoreEntry> scores;
    static std::string filePath;

    static void save();
};