#ifndef MAPLAYERS_H
#define MAPLAYERS_H

#include <QVector>

#include "layer.h"

class MapLayers {
    QVector<Layer*> layers;
public:
    MapLayers();

    int size();
    void add(Layer *layer);

    Layer* get(int index);
    Layer* get(QString name);
    int getIndex(QString name);
    int getIndex(Layer *layer);

    void remove(int index);
    void remove(Layer *layer);

    void turnRight();
    void turnLeft();
    void flipX();
    void flipY();
};

#endif // MAPLAYERS_H
