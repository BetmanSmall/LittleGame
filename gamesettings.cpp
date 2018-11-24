#include "gamesettings.h"

GameSettings::GameSettings(QString mapPath) {
    if (mapPath.contains("randomMap")) {
        gameType = GameType::type::LittleGame;
        difficultyLevel = 0.0;
        enemyCount = 10;
        towersCount = 0;
    } else if (mapPath.contains("island")) {
        gameType = GameType::type::LittleGame;
        difficultyLevel = 1.0;
        enemyCount = 10;
        towersCount = 0;
    } else {
        gameType = GameType::type::TowerDefence;
        difficultyLevel = 1.0;
        enemyCount = 0;
        towersCount = 0;
    }
}
