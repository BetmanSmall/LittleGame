#ifndef ANIMATION_H
#define ANIMATION_H

#include <QDebug>
#include <QPixmap>

class Animation {
//    enum class PlayMode {
//        NORMAL,
//        REVERSED,
//        LOOP,
//        LOOP_REVERSED,
//        LOOP_PINGPONG,
//        LOOP_RANDOM
//    };

//    std::vector<QPixmap> keyFrames;
//    float frameDuration;
//    float animationDuration;
//    int lastFrameNumber;
//    float lastStateTime;
//    PlayMode playMode = PlayMode.NORMAL;

//    Animation (float frameDuration, std::vector<QPixmap> keyFrames) {
//        this.frameDuration = frameDuration;
//        this.animationDuration = keyFrames.size * frameDuration;
//        this.keyFrames = new TextureRegion[keyFrames.size];
//        for (int i = 0, n = keyFrames.size; i < n; i++) {
//            this.keyFrames[i] = keyFrames.get(i);
//        }

//        this.playMode = PlayMode.NORMAL;
//    }

//    Animation (float frameDuration, Array<? extends TextureRegion> keyFrames, PlayMode playMode) {

//        this.frameDuration = frameDuration;
//        this.animationDuration = keyFrames.size * frameDuration;
//        this.keyFrames = new TextureRegion[keyFrames.size];
//        for (int i = 0, n = keyFrames.size; i < n; i++) {
//            this.keyFrames[i] = keyFrames.get(i);
//        }

//        this.playMode = playMode;
//    }

//    Animation (float frameDuration, TextureRegion... keyFrames) {
//        this.frameDuration = frameDuration;
//        this.animationDuration = keyFrames.length * frameDuration;
//        this.keyFrames = keyFrames;
//        this.playMode = PlayMode.NORMAL;
//    }

//    TextureRegion getKeyFrame (float stateTime, boolean looping) {
//        // we set the play mode by overriding the previous mode based on looping
//        // parameter value
//        PlayMode oldPlayMode = playMode;
//        if (looping && (playMode == PlayMode.NORMAL || playMode == PlayMode.REVERSED)) {
//            if (playMode == PlayMode.NORMAL)
//                playMode = PlayMode.LOOP;
//            else
//                playMode = PlayMode.LOOP_REVERSED;
//        } else if (!looping && !(playMode == PlayMode.NORMAL || playMode == PlayMode.REVERSED)) {
//            if (playMode == PlayMode.LOOP_REVERSED)
//                playMode = PlayMode.REVERSED;
//            else
//                playMode = PlayMode.LOOP;
//        }

//        TextureRegion frame = getKeyFrame(stateTime);
//        playMode = oldPlayMode;
//        return frame;
//    }

//    TextureRegion getKeyFrame (float stateTime) {
//        int frameNumber = getKeyFrameIndex(stateTime);
//        return keyFrames[frameNumber];
//    }

//    int getKeyFrameIndex (float stateTime) {
//        if (keyFrames.length == 1) return 0;

//        int frameNumber = (int)(stateTime / frameDuration);
//        switch (playMode) {
//        case NORMAL:
//            frameNumber = Math.min(keyFrames.length - 1, frameNumber);
//            break;
//        case LOOP:
//            frameNumber = frameNumber % keyFrames.length;
//            break;
//        case LOOP_PINGPONG:
//            frameNumber = frameNumber % ((keyFrames.length * 2) - 2);
//            if (frameNumber >= keyFrames.length) frameNumber = keyFrames.length - 2 - (frameNumber - keyFrames.length);
//            break;
//        case LOOP_RANDOM:
//            int lastFrameNumber = (int) ((lastStateTime) / frameDuration);
//            if (lastFrameNumber != frameNumber) {
//                frameNumber = MathUtils.random(keyFrames.length - 1);
//            } else {
//                frameNumber = this.lastFrameNumber;
//            }
//            break;
//        case REVERSED:
//            frameNumber = Math.max(keyFrames.length - frameNumber - 1, 0);
//            break;
//        case LOOP_REVERSED:
//            frameNumber = frameNumber % keyFrames.length;
//            frameNumber = keyFrames.length - frameNumber - 1;
//            break;
//        }

//        lastFrameNumber = frameNumber;
//        lastStateTime = stateTime;

//        return frameNumber;
//    }

//    TextureRegion[] getKeyFrames () {
//        return keyFrames;
//    }

//    PlayMode getPlayMode () {
//        return playMode;
//    }

//    void setPlayMode (PlayMode playMode) {
//        this.playMode = playMode;
//    }

//    boolean isAnimationFinished (float stateTime) {
//        int frameNumber = (int)(stateTime / frameDuration);
//        return keyFrames.length - 1 < frameNumber;
//    }

//    void setFrameDuration (float frameDuration) {
//        this.frameDuration = frameDuration;
//        this.animationDuration = keyFrames.length * frameDuration;
//    }

//    float getFrameDuration () {
//        return frameDuration;
//    }

//    float getAnimationDuration () {
//        return animationDuration;
//    }
};

#endif // ANIMATION_H
