#ifndef FIELD_H
#define FIELD_H

#define CIRCLET8
#include <iostream>
#include <QPixmap>

//#include "src/head/libs/astar.h"
#include "src/head/unitsmanager.h"
#include "src/head/towersmanager.h"
#include "src/head/factionsmanager.h"
#include "src/head/bullet.h"
#include "src/head/cell.h"

/**
 * @brief Поле из ячеек
 * @details Класс так же отвечает за Башни, Юнитов. И пока что одну фракцию
 */
class Field
{
public: // we are friendly!
    AStar::PathFinder pathFinder;

    Map* map;
    Cell* field;
    FactionsManager* factionsManager;
    TowersManager towersManager;
    UnitsManager unitsManager;
//    Faction* faction;

    int gameOverLimitUnits;
    int currentFinishedUnits;

    int sizeX, sizeY;

    int mainCoorMapX, mainCoorMapY;
    int spaceWidget; // fix this. 16 and launch
    int sizeCell;

    bool isometric;
    int tileMapWidth, tileMapHeight;

    int mouseX, mouseY;
    int spawnPointX, spawnPointY;
    int exitPointX, exitPointY;

public:
    Field();
    ~Field();
    Cell* getCell(int x, int y);

    /**
     * @brief Создает поле, размерностью (newSizeX, newSizeY)
     * @param newSizeX
     * @param newSizeY
     */
    void createField(int newSizeX, int newSizeY);
    /**
     * @brief Очищает память из под поля
     */
    void deleteField();

//    /**
//     * @brief Временная функция, устанавливает Фракцию
//     * @param faction - Указатель на фракцию
//     */
//    void setFaction(Faction *faction);

    /**
     * @brief Устанавливает точку спавна на поле
     * @param num - Колличество крипов, которые будут созданы из этой точки
     * @param x - Координаты по X
     * @param y - Координаты по Y
     * @return True - Успешно; False - Не получилось (True/False)
     */
    bool createSpawnPoint(int num, int x = -1, int y = -1);
    /**
     * @brief Устанавливает точку куда криппы должны идти
     * @param x - Координаты по X
     * @param y - Координаты по Y
     */
    void createExitPoint(int x, int y);

    void updateHeroDestinationPoint();
    void updateHeroDestinationPoint(int x, int y);
    void updatePathFinderWalls();

    /**
     * @brief Возвращает размер поля по X
     * @return Число клеток по X
     */
    int getSizeX();
    /**
     * @brief Возвращает размер поля по Y
     * @return Число клеток по Y
     */
    int getSizeY();

    /**
     * @brief setMainCoorMap -- ЗАДОКУМЕНТИТЬ --
     * @param mainCoorMapX_FromGameWidget
     * @param mainCoorMapY_FromGameWidget
     */
    void setMainCoorMap(int mainCoorMapX, int mainCoorMapY);
    /**
     * @brief setSizeCell
     * @param sizeCell_FromGameWidget
     */
    void setSizeCell(int sizeCell);
    /**
     * @brief getMainCoorMapX
     * @return
     */
    int getMainCoorMapX();
    /**
     * @brief getMainCoorMapY
     * @return
     */
    int getMainCoorMapY();

    /**
     * @brief getSpaceWidget
     * @return
     */
    int getSpaceWidget();
    /**
     * @brief getSizeCell
     * @return
     */
    int getSizeCell();
    /**
     * @brief Функция которая заставляет атаковать все башни, когда в радиусе обзора есть крипп
     * @return Пока что всегда тру
     */

    /**
     * @brief Устанавливает изометрическая ли карта
     * @param isometric
     */
    void setIsometric(bool isometric);
    /**
     * @brief setTileMapSize
     * @param tileMapWidth
     * @param tileMapHeight
     */
    void setTileMapSize(int tileMapWidth, int tileMapHeight);

    /**
     * @brief getIsometric
     * @return
     */
    bool getIsometric();
    /**
     * @brief getTileMapWidth
     * @return
     */
    int getTileMapWidth();
    /**
     * @brief getTileMapHeight
     * @return
     */
    int getTileMapHeight();

    bool towersAttack(int deltaTime);
    /**
     * @brief Применяет Волновой Алгоритм на поле из ячееки по данным кординатам
     * @param x - Координаты по X
     * @param y - Координаты по Y
     */
    void waveAlgorithm(int x = -1, int y = -1);
    /**
     * @brief Рекурсивная функция для волнового алгоритма
     * @param x - Координаты по X
     * @param y - Координаты по Y
     * @param Число для данной ячейки
     */
    void waveStep(int x, int y, int step);

    /**
     * @brief setMousePress
     * @param x
     * @param y
     */
    void setMousePress(int x, int y);
    /**
     * @brief getMousePress
     * @param x
     * @param y
     * @return True - Установленно; False - Нет (True/False)
     */
    bool getMousePress(int x = -1, int y = -1);
    /**
     * @brief Проверяет установленная ли Спавн точка
     * @param x
     * @param y
     * @return True - Созданная; False - Не создана (True/False)
     */
    bool isSetSpawnPoint(int x = -1, int y = -1);
    /**
     * @brief Проверяет установленная ли точка выхода
     * @param x
     * @param y
     * @return True - Созданная; False - Не создана (True/False)
     */
    bool isSetExitPoint(int x = -1, int y = -1);

    /**
     * @brief Говорит всем криппам ходить
     * @return 4 - Герой contact с врагом
     * @return 3 - Герой в точке @exitPoint
     * @return 2 - Все криппы мертвы
     * @return 1 - Eсли колличество криппов в точке @exitPoint превышено $gameOverLimitUnits
     * @return 0 - Все криппы сходили успешно
     * @return -1 - Какому-либо криппу перекрыли путь до $exitPoint
     */
    int stepAllUnits();
    /**
     * @brief Должен сходить крипп под номером
     * @param num - номер криппа
     * @return Какие-то ошибки, смотри выше!
     */
    int stepOneUnit(int num);

    /**
     * @brief Возращает номер шага для волнового алгоритма с проверками
     * @param x
     * @param y
     * @return Номер шага
     */
    int getNumStep(int x, int y);
    /**
     * @brief Возращает номер шага для волнового алгоритма без проверок
     * @param x
     * @param y
     * @return Номер шага
     */
    int getStepCell(int x, int y);
    /**
     * @brief Устнавливает номер шага в ячейки из расчета волнового алгоритма
     * @param x
     * @param y
     * @param step - Номер шага волнового алгоритма
     * @return True/False установли/не установил по определеным причинам
     */
    bool setNumOfCell(int x, int y, int step);
    /**
     * @brief Непосредственно устанавливает номер шага в ячейку
     * @param x
     * @param y
     * @param Номер шага волнового алгоритма
     */
    void setStepCell(int x, int y, int step);
    /**
     * @brief Сброс номер шага в ячейке
     * @param x
     * @param y
     */
    void clearStepCell(int x, int y);

    /**
     * @brief Возвращает указатель на объект класса "крипп" по полученным координатам
     * @param x
     * @param y
     * @return Указатель на объект класса "крипп"
     */
    Unit* getUnit(int x, int y);
    /**
     * @brief Возвращает вектор криппов, находящихся в этой ячейке
     * @param x
     * @param y
     * @return Вектор указателей на обекты криппов
     */
    std::vector<Unit *> getUnits(int x, int y);
    /**
     * @brief Устарело
     * @param x
     * @param y
     * @return
     */
    int getUnitHpInCell(int x, int y);
    /**
     * @brief Возвращает указатель на криппа, с наименьшим хп в данной ячейке
     * @param x
     * @param y
     * @return Указатель на криппа
     */
    Unit* getUnitWithLowHP(int x, int y);

    /**
     * @brief Возвращает вектор указателей всех башен созданных в объекте класса $Towers
     * @return Вектор указателей на объекты башен
     */
    std::vector<Tower*> getAllTowers();

    /**
     * @brief Проверяет пустая ли ячейка
     * @param x
     * @param y
     * @return True - Свободна; False - Занята (True/False)
     * @return True/False свободна/занята
     */
    bool containEmpty(int x, int y);
    /**
     * @brief Проверяет занята ли клетка рельефом
     * @param x
     * @param y
     * @return True - Занята; False - Свободна (True/False)
     * @return True/False занята/свободна
     */
    bool isTerrain(int x, int y);
    /**
     * @brief Проверяет занята ли клетка башней
     * @param x
     * @param y
     * @return True - Занята; False - Свободна (True/False)
     * @return True/False занята/свободна
     */
    bool containTower(int x, int y);
    /**
     * @brief Возвращает индекс криппа в данной клетке (с учетом того что в данной клектке могут находится несколько криппов)
     * @param x
     * @param y
     * @param unit - Указатель на криппа
     * @return 0 - Нет крипов, либо нет данного криппа.
     * @return {int} - Индекс криппа, либо количество криппов
     */
    int containUnit(int x, int y, Unit* unit = NULL);

    /**
     * @brief Устанавливает занятость клетки и соответсвующую картинку
     * @param x
     * @param y
     * @param pixmap - Картинка рельефа
     * @return True - Установил; False - Не установил (True/False)
     * @return True/False установил/не установил
     */
    bool setBusy(int x, int y, QPixmap pixmap);

    /**
     * @brief Устанавливает башню на основе Стандартной Башни из фракции
     * @param x
     * @param y
     * @param defTower - Указатель на стандартную(дефолтную) башню
     * @return True - Установил; False - Не установил (True/False)
     * @return True/False установил/не установил
     */
    bool setTower(int x, int y, TemplateForTower* defTower);
    /**
     * @brief Создать криппа в spawnPoint'e
     * @return True - Создал; False - Не создал (True/False)
     * @return True/False создал/не создал
     */
    bool spawnHeroInSpawnPoint();//Unit* unit = NULL);//, int type = 0);
    /**
     * @brief Создаёт криппа в данную клетку
     * @param x
     * @param y
     * @param type - тип юнита 0 = hero, 1 = guardian
     * @return Unit* - указатель на созданого юнита
     */
    Unit* createUnit(int x, int y, int type = -1);


    /**
     * @brief Очищяет 'занятость' / Убирает рельеф
     * @param x
     * @param y
     * @return True - Очистил; False - Не очистил (True/False)
     */
    bool clearBusy(int x, int y);
    /**
     * @brief Удаляет с !клетки! башню
     * @param x
     * @param y
     * @return True - Удалил; False - Не удалил (True/False)
     */
    bool clearTower(int x, int y);
    /**
     * @brief Удаляет с клетки конкретного криппа (если он там есть есть) / если криппа не передали - удаляем последнего пришедшего?
     * @param x
     * @param y
     * @param unit - Указатель на криппа
     * @return True - Удалил; False - Не удалил (True/False)
     */
    bool clearUnit(int x, int y, Unit* unit = NULL);

    /**
     * @brief Удаляет из $towerS по координатам
     * @param x
     * @param y
     * @return True - Удалил; False - Не удалил (True/False)
     */
    bool deleteTower(int x = -1, int y = -1);
};

#endif // FIELD_H
