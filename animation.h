#ifndef ANIMATION_H
#define ANIMATION_H

#include <QDebug>
#include <QPixmap>
#include <QtGlobal>

class Animation {
public:
    enum PlayMode {
        NORMAL,
        REVERSED2,
        LOOP,
        LOOP_REVERSED2,
        LOOP_PINGPONG,
        LOOP_RANDOM
    };

public:
    std::vector<QPixmap> keyFrames;
    float frameDuration;
    float animationDuration;
    int lastFrameNumber;
    float lastStateTime;
    PlayMode playMode = PlayMode::NORMAL;

//private:
    Animation(float frameDuration, std::vector<QPixmap> keyFrames);
//    Animation(float frameDuration, Array<? extends QPixmap> keyFrames, PlayMode playMode);
//    Animation(float frameDuration, QPixmap... keyFrames);
    QPixmap getKeyFrame(float stateTime, bool looping);
    QPixmap getKeyFrame(float stateTime);
    int getKeyFrameIndex(float stateTime);
    std::vector<QPixmap> getKeyFrames();
    PlayMode getPlayMode();
    void setPlayMode(PlayMode playMode);
    bool isAnimationFinished(float stateTime);
    void setFrameDuration(float frameDuration);
    float getFrameDuration();
    float getAnimationDuration();
};

#endif // ANIMATION_H
