#ifndef LAYER_H
#define LAYER_H

#include "tilelayercell.h"
//#include "map.h"

class Layer {
    QString name;
    float opacity;
    bool visible;
//    MapObjects objects = new MapObjects();
    QMap<QString, QString> properties;

    int width;
    int height;
    float tileWidth;
    float tileHeight;
    QVector<TileLayerCell> cells;

public:
    Layer(int width, int height, int tileWidth, int tileHeight);

    QString getName();
    void setName(QString name);

    float getOpacity();
    void setOpacity(float opacity);
//    MapObjects getObjects();
    bool isVisible();
    void setVisible(bool visible);
    QMap<QString, QString> *getProperties();

    int getWidth();
    int getHeight();
    float getTileWidth();
    float getTileHeight();

    TileLayerCell *getCell(int x, int y);
    void setCell(int x, int y, TileLayerCell cell);

    bool turnRight();
    bool turnLeft();
    bool flipX();
    bool flipY();
};

#endif // LAYER_H
