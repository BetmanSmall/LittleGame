#ifndef CREEPS_H
#define CREEPS_H

#include <stdlib.h>

#include <QPixmap>
#include <qdebug.h>

#include "unit.h"

/**
 * @brief The UnitsManager class
 */
class UnitsManager {
public: // we are friendly to all!
    Unit* hero;
    Unit* units;

    QPixmap defaultPixmapForUnit;

    /**
     * @brief size - Количество units, которое может быть создано;
     * максимально возможное и ограниченное данным параметром.
     */
    int size;
    /**
     * @brief amount - Количество созданных крипов
     */
    int amount;

public:
    /**
     * @brief Units.  Конструктор. Массив крипов инициилизируется NULL, чтобы определить, что массив нужно создать.
     * Нужно определять создан ли массив крипов или нет, чтобы не возникло проблем при игровой инициализации массива
     */
    UnitsManager();

    /**
     * @brief  Деструктор. Вызывает функцию deleteMass(). Она в свою очередь очищает память из под массива крипов.
     */
    ~UnitsManager();

    /**
     * @brief createMass - иноциализирует массив крипов.
     * @param newSize - максимально возможное количество крипов.
     */
    void createMass(int newSize);
    /**
     * @brief deleteMass - удаляет массив крипов.
     */
    void deleteMass();

    /**
     * @brief getSize - возвращает максимальное количество крипов, которое может быть.
     * @return - количество крипов.
     */
    int getSize();
    /**
     * @brief getAmount - возвращает количество созданных крипов.
     * @return - количество крипов.
     */
    int getAmount();
    /**
     * @brief getHP - возвращает количество здоровья крипа, находящегося по заданным координатам.
     * @param x - координаты по оси x.
     * @param y - координаты по оси y.
     * @return
     */
    int getHP(int x, int y);

    /**
     * @brief attackUnit - наносит определенному (по переданным координатам) крипу урон, равный переменной @hp.
     * Если этот урон превышает текущее количество здоровья крипа, то убивает его. Также, в зависимости от направления ходьбы
     * крипа устанавливает определнённые иллюстрации смерти.
     * @param x - координаты по оси x.
     * @param y - координаты по оси y.
     * @param damage - урон, наносимый крипу.
     * @param unit - указатель на крипа.
     * @return - true, если нужный крип найден и урон нанесён; false - крип не найден, урон не нанесён.
     */
    bool attackUnit(int x, int y, int damage); //, Unit* unit);

    /**
     * @brief getUnit - возвращает крипа по его номеру в массиве
     * @param number - номер крипа в массиве
     * @return - возвращает указатель на нужного крипа.
     */
    Unit* getUnit(int number);
    /**
     * @brief getUnit - ищет и возвращает крипа по координатам. Если x = -1; y = -1,
     * то параметры в функцию не передались, будет возвращена последний созданный крип по ID.
     * @param x - координаты по оси x.
     * @param y - координаты по оси y.
     * @return - возвращает указатель на крипа или NULL, если переданные координаты не совпали
     * с координатами ни одного крипа в массиве.
     */
    Unit* getUnit(int x, int y);

    Unit* createHero(int coorByCellX, int coorByCellY, int coorByMapX, int coorByMapY, DefaultUnit* unit);
    /**
     * @brief createUnit - Если переменная size позволяет создать ещё крипа, то инициализирует нового крипа.
     * @param coorByCellX - координаты, где нужно создать крипа по оси x << Trouble
     * @param coorByCellY - координаты, где нужно создать крипа по оси y << Trouble
     * @param coorByMapX - координаты, где нужно создать крипа по оси x << Trouble
     * @param coorByMapX - координаты, где нужно создать крипа по оси y << Trouble
     * @param unit - реализация крипа по-умолчанию (уровень модификации, тип, и т. д.)
     * @return - возвращает указатель на созданного крипа [и увеличивает количесвто созданных крипов на 1].
     */
    Unit* createUnit(int coorByCellX, int coorByCellY, int coorByMapX, int coorByMapY, DefaultUnit* unit, int type);

    /**
     * @brief setDefaulPixmapForUnit - устанавливает картинку крипа по-умолчанию.
     * @param pixmap - картинка.
     */
    void setDefaulPixmapForUnit(QPixmap pixmap);
    /**
     * @brief getUnitPixmap - возвращает картинку крипа с переданными координатами. Идёт поиск по всем
     * созданным крипам.
     * @param x - координаты по оси x.
     * @param y - координаты по оси y.
     * @return - возвращат картинку крипа, находящегося по переданным координатам.
     */
    QPixmap getUnitPixmap(int x, int y);
};

#endif // CREEPS_H
