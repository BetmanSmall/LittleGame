#include "animation.h"

Animation::Animation(float frameDuration, std::vector<QPixmap> keyFrames) {
    this->frameDuration = frameDuration;
    this->animationDuration = keyFrames.size() * frameDuration;
//        this->keyFrames = new QPixmap[keyFrames.size()];
    for (int i = 0, n = keyFrames.size(); i < n; i++) {
        this->keyFrames.push_back(keyFrames.at(i));
    }
    this->playMode = PlayMode::NORMAL;
}

//    Animation::Animation(float frameDuration, Array<? extends QPixmap> keyFrames, PlayMode playMode) {
//        this->frameDuration = frameDuration;
//        this->animationDuration = keyFrames.size * frameDuration;
//        this->keyFrames = new QPixmap[keyFrames.size];
//        for (int i = 0, n = keyFrames.size; i < n; i++) {
//            this->keyFrames[i] = keyFrames.get(i);
//        }
//        this->playMode = playMode;
//    }

//    Animation::Animation(float frameDuration, QPixmap... keyFrames) {
//        this->frameDuration = frameDuration;
//        this->animationDuration = keyFrames.size() * frameDuration;
//        this->keyFrames = keyFrames;
//        this->playMode = PlayMode::NORMAL;
//    }

QPixmap Animation::getKeyFrame(float stateTime, bool looping) {
    // we set the play mode by overriding the previous mode based on looping
    // parameter value
    PlayMode oldPlayMode = playMode;
    if (looping && (playMode == PlayMode::NORMAL || playMode == PlayMode::REVERSED2)) {
        if (playMode == PlayMode::NORMAL)
            playMode = PlayMode::LOOP;
        else
            playMode = PlayMode::LOOP_REVERSED2;
    } else if (!looping && !(playMode == PlayMode::NORMAL || playMode == PlayMode::REVERSED2)) {
        if (playMode == PlayMode::LOOP_REVERSED2)
            playMode = PlayMode::REVERSED2;
        else
            playMode = PlayMode::LOOP;
    }
    QPixmap frame = getKeyFrame(stateTime);
    playMode = oldPlayMode;
    return frame;
}

QPixmap Animation::getKeyFrame(float stateTime) {
    int frameNumber = getKeyFrameIndex(stateTime);
    return keyFrames[frameNumber];
}

int Animation::getKeyFrameIndex(float stateTime) {
    if (keyFrames.size() == 1) return 0;
    int frameNumber = (int)(stateTime / frameDuration);
    switch (playMode) {
    case PlayMode::NORMAL:
        frameNumber = qMin((int)(keyFrames.size() - 1), frameNumber);
        break;
    case PlayMode::LOOP:
        frameNumber = frameNumber % keyFrames.size();
        break;
    case PlayMode::LOOP_PINGPONG:
        frameNumber = frameNumber % ((keyFrames.size() * 2) - 2);
        if (frameNumber >= keyFrames.size()) frameNumber = keyFrames.size() - 2 - (frameNumber - keyFrames.size());
        break;
    case PlayMode::LOOP_RANDOM:
        int lastFrameNumberTmp = (int) ((lastStateTime) / frameDuration);
        if (lastFrameNumberTmp != frameNumber) {
            frameNumber = rand()%(keyFrames.size() - 1);
        } else {
            frameNumber = this->lastFrameNumber;
        }
        break;
//    case PlayMode::REVERSED2:
//        frameNumber = qMax((int)(keyFrames.size() - frameNumber - 1), 0);
//        break;
//    case PlayMode::LOOP_REVERSED2:
//        frameNumber = frameNumber % keyFrames.size();
//        frameNumber = keyFrames.size() - frameNumber - 1;
//        break;
    }
    this->lastFrameNumber = frameNumber;
    this->lastStateTime = stateTime;
    return frameNumber;
}

std::vector<QPixmap> Animation::getKeyFrames() {
    return keyFrames;
}

Animation::PlayMode Animation::getPlayMode() {
    return playMode;
}

void Animation::setPlayMode(Animation::PlayMode playMode) {
    this->playMode = playMode;
}

bool Animation::isAnimationFinished(float stateTime) {
    int frameNumber = (int)(stateTime / frameDuration);
    return keyFrames.size() - 1 < frameNumber;
}

void Animation::setFrameDuration(float frameDuration) {
    this->frameDuration = frameDuration;
    this->animationDuration = keyFrames.size() * frameDuration;
}

float Animation::getFrameDuration() {
    return frameDuration;
}

float Animation::getAnimationDuration() {
    return animationDuration;
}
