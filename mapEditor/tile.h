#ifndef TILE_H
#define TILE_H

#include <QDebug>
#include <QPixmap>
#include <QMap>

class Tile {
    enum BlendMode {
        NONE, ALPHA
    };
protected:
    int id;
    QPixmap pixmap;
    QMap<QString, QString> properties;
    BlendMode blendMode;
    float offsetX;
    float offsetY;
public:
    Tile();
    virtual ~Tile();
    void setId(int id);
    int getId();

    QMap<QString, QString> *addProperties(QMap<QString, QString> properties);
    QMap<QString, QString> *getProperties();
    void setPixmap(QPixmap pixmap);
    QPixmap getPixmap();
    QPixmap *getPixmapRef();

    void setBlendMode(BlendMode blendMode);
    BlendMode getBlendMode();

    void setOffsetX(float offsetX);
    float getOffsetX();

    void setOffsetY(float offsetY);
    float getOffsetY();
};

#endif // TILE_H
