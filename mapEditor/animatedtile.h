#ifndef ANIMATEDTILE_H
#define ANIMATEDTILE_H

#include "statictile.h"

class AnimatedTile : public Tile {
//    int id;
//    BlendMode blendMode;// = BlendMode.ALPHA;
//    QMap<QString, QString> properties;

    long lastTiledMapRenderTime;
    QVector<StaticTile*> frameTiles;
    QVector<int> animationIntervals;

    int frameCount;
    int loopDuration;
    long initialTimeOffset;// = TimeUtils.millis();
public:
    AnimatedTile();

//    int getId();
//    void setId(int id);

//    BlendMode getBlendMode();
//    void setBlendMode(BlendMode blendMode);

    int getCurrentFrameIndex();

    StaticTile *getCurrentFrame();

    QPixmap getPixmap();
    void setPixmap(QPixmap pixmap);

    float getOffsetX();
    void setOffsetX(float offsetX);

    float getOffsetY();
    void setOffsetY(float offsetY);

    QVector<int> getAnimationIntervals();
    void setAnimationIntervals(QVector<int> intervals);

//    QMap<QString, QString> *getProperties();

    /** Function is called by BatchTiledMapRenderer render(), lastTiledMapRenderTime is used to keep all of the tiles in lock-step
     * animation and avoids having to call TimeUtils.millis() in getTextureRegion() */
    void updateAnimationBaseTime();

    /** Creates an animated tile with the given animation interval and frame tiles.
     *
     * @param interval The interval between each individual frame tile.
     * @param frameTiles An array of {@link StaticTiledMapTile}s that make up the animation. */
    AnimatedTile(float interval, QVector<StaticTile*> frameTiles);

    /** Creates an animated tile with the given animation intervals and frame tiles.
     *
     * @param intervals The intervals between each individual frame tile in milliseconds.
     * @param frameTiles An array of {@link StaticTiledMapTile}s that make up the animation. */
    AnimatedTile(QVector<int> intervals, QVector<StaticTile*> frameTiles);

    QVector<StaticTile*> getFrameTiles();
};

#endif // ANIMATEDTILE_H
