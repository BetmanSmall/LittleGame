#include "src/head/mapEditor/animatedtile.h"

#include <QDebug>
#include <QDateTime>

AnimatedTile::AnimatedTile() {
    lastTiledMapRenderTime = 0;
    frameCount = 0;
}

//int AnimatedTile::getId() {
//    return id;
//}

//void AnimatedTile::setId(int id) {
//    this->id = id;
//}

//Tile::BlendMode AnimatedTile::getBlendMode() {
//    return blendMode;
//}

//void AnimatedTile::setBlendMode(BlendMode blendMode) {
//    this->blendMode = blendMode;
//}

int AnimatedTile::getCurrentFrameIndex() {
    int currentTime = (int)(lastTiledMapRenderTime % loopDuration);

    for (int i = 0; i < animationIntervals.length(); ++i) {
        int animationInterval = animationIntervals.value(i);
        if (currentTime <= animationInterval) return i;
        currentTime -= animationInterval;
    }
    qCritical() << "Could not determine current animation frame in AnimatedTiledMapTile.  This should never happen.";
//    throw new GdxRuntimeException(
//        "Could not determine current animation frame in AnimatedTiledMapTile.  This should never happen.");
}

StaticTile *AnimatedTile::getCurrentFrame() {
    return frameTiles[getCurrentFrameIndex()];
}

QPixmap AnimatedTile::getPixmap() {
    return getCurrentFrame()->getPixmap();
}

void AnimatedTile::setPixmap(QPixmap pixmap) {
    qCritical() << "Cannot set the pixmap of AnimatedTile.";
//    throw new GdxRuntimeException("Cannot set the texture region of AnimatedTiledMapTile.");
}

float AnimatedTile::getOffsetX() {
    return getCurrentFrame()->getOffsetX();
}

void AnimatedTile::setOffsetX(float offsetX) {
    qCritical() << "Cannot set offset of AnimatedTile.";
//    throw new GdxRuntimeException("Cannot set offset of AnimatedTiledMapTile.");
}

float AnimatedTile::getOffsetY() {
    return getCurrentFrame()->getOffsetY();
}

void AnimatedTile::setOffsetY(float offsetY) {
    qCritical() << "Cannot set offset of AnimatedTile.";
//    throw new GdxRuntimeException("Cannot set offset of AnimatedTiledMapTile.");
}

QVector<int> AnimatedTile::getAnimationIntervals() {
    return animationIntervals;
}

void AnimatedTile::setAnimationIntervals(QVector<int> intervals) {
    if (intervals.length() == animationIntervals.length()) {
        this->animationIntervals = intervals;

        loopDuration = 0;
        for (int i = 0; i < intervals.length(); i++) {
            loopDuration += intervals[i];
        }

    } else {
        qCritical() << "Cannot set " << intervals.length() << " frame intervals. The given int[] must have a size of " << animationIntervals.length() << ".";
//        throw new GdxRuntimeException("Cannot set " + intervals.length() + " frame intervals. The given int[] must have a size of " + animationIntervals.length() + ".");
    }
}

//QMap<QString, QString> *AnimatedTile::getProperties() {
////    if (properties == null) {
////        properties = new MapProperties();
////    }
//    return &properties;
//}

/** Function is called by BatchTiledMapRenderer render(), lastTiledMapRenderTime is used to keep all of the tiles in lock-step
 * animation and avoids having to call TimeUtils.millis() in getTextureRegion() */
/*static */void AnimatedTile::updateAnimationBaseTime() {
    lastTiledMapRenderTime = QDateTime::currentMSecsSinceEpoch() - initialTimeOffset;
}

/** Creates an animated tile with the given animation interval and frame tiles.
 *
 * @param interval The interval between each individual frame tile.
 * @param frameTiles An array of {@link StaticTiledMapTile}s that make up the animation. */
AnimatedTile::AnimatedTile(float interval, QVector<StaticTile*> frameTiles) {
//    this->frameTiles = new StaticTile[frameTiles.length()];
    this->frameCount = frameTiles.length();

    this->loopDuration = frameTiles.length() * (int)(interval * 1000);
    this->animationIntervals.resize(frameTiles.length());
    for (int i = 0; i < frameTiles.length(); ++i) {
        this->frameTiles[i] = frameTiles[i];
        this->animationIntervals[i] = (int)(interval * 1000);
    }
}

/** Creates an animated tile with the given animation intervals and frame tiles.
 *
 * @param intervals The intervals between each individual frame tile in milliseconds.
 * @param frameTiles An array of {@link StaticTiledMapTile}s that make up the animation. */
AnimatedTile::AnimatedTile(QVector<int> intervals, QVector<StaticTile*> frameTiles) {
//    this->frameTiles = new StaticTile[frameTiles.length()];
    this->frameCount = frameTiles.length();

    this->animationIntervals = intervals;
    this->loopDuration = 0;

    for (int i = 0; i < intervals.length(); ++i) {
        this->frameTiles.insert(i, frameTiles.value(i));
        this->loopDuration += intervals.value(i);
    }
}

QVector<StaticTile*> AnimatedTile::getFrameTiles() {
    return frameTiles;
}
