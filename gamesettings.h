#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include "gametype.h"
#include "cell.h"

class GameSettings
{
public:
    GameType::type gameType;
    float difficultyLevel;
    int enemyCount;
    int towersCount;

    bool isometric = false;
    Cell* cellSpawnHero = NULL;
    Cell* cellExitHero = NULL;

    int maxOfMissedUnitsForComputer0;
    int missedUnitsForComputer0;
    int maxOfMissedUnitsForPlayer1;
    int missedUnitsForPlayer1;

public:
    GameSettings(QString mapPath);
};

#endif // GAMESETTINGS_H
