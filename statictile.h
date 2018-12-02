#ifndef STATICTILE_H
#define STATICTILE_H

#include "tile.h"

class StaticTile : public Tile {
//    int id;
//    BlendMode blendMode;// = BlendMode::ALPHA;
//    QMap<QString, QString> properties;
//    QPixmap pixmap;
//    float offsetX;
//    float offsetY;
public:
//    StaticTile();

//    int getId();
//    void setId(int id);

//    BlendMode getBlendMode();
//    void setBlendMode(BlendMode blendMode);

//    QMap<QString, QString> *getProperties();

    QPixmap getPixmap();
    void setPixmap(QPixmap pixmap);

    float getOffsetX();
    void setOffsetX(float offsetX);

    float getOffsetY();
    void setOffsetY(float offsetY);

    /** Creates a static tile with the given region
     *
     * @param textureRegion the {@link TextureRegion} to use. */
    StaticTile(QPixmap pixmap);

    /** Copy constructor
     *
     * @param copy the StaticTiledMapTile to copy. */
//    StaticTile(StaticTile copy);
};

#endif // STATICTILE_H
