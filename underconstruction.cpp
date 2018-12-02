#include "underconstruction.h"

UnderConstruction::UnderConstruction(TemplateForTower *templateForTower) {
    this->state = -1;
    this->templateForTower = templateForTower;
}

UnderConstruction::UnderConstruction(int startX, int startY, TemplateForTower *templateForTower) {
    this->state = 1;
    this->startX = startX;
    this->startY = startY;
    this->templateForTower = templateForTower;
}

bool UnderConstruction::setStartCoors(int startX, int startY) {
    qDebug() << "UnderConstruction::setStartCoors(" << startX << "," << startY << ");";
    this->state = 1;
    this->startX = startX;
    this->startY = startY;
    return true;
}

bool UnderConstruction::setEndCoors(int endX, int endY) {
//    qDebug() << "UnderConstruction::setEndCoors(" << endX << "," << endY << ");";
    this->endX = endX;
    this->endY = endY;

    if(state == -1) {
        state = 0;
    }
    if(state == 1 && templateForTower != NULL) {
        coorsX.clear();
        coorsY.clear();
        int towerSize = templateForTower->size;
        int deltaX = 0, deltaY = 0;
        if(towerSize != 1) {
            if(towerSize%2 == 0) {
                deltaX = towerSize/2;
                deltaY = (towerSize/2)-1;
            } else {
                deltaX = towerSize/2;
                deltaY = towerSize/2;
            }
        }
        int tmpX = startX-deltaX;
        int tmpY = startY-deltaY;
        if(endY == tmpY || (endY < (tmpY+towerSize) && endY > tmpY)) {
            if(endX >= tmpX) {
                for(int currX = tmpX+towerSize; currX <= endX; currX+=towerSize) {
                    this->coorsX.push_back(currX+deltaX);
                    this->coorsY.push_back(tmpY+deltaY);
                }
            } else {
                for(int currX = tmpX-towerSize; currX > endX-towerSize; currX-=towerSize) {
                    this->coorsX.push_back(currX+deltaX);
                    this->coorsY.push_back(tmpY+deltaY);
                }
            }
        } else if(endX == tmpX || endX < (tmpX+towerSize) && endX > tmpX) {
            if(endY >= tmpY) {
                for(int currY = tmpY+towerSize; currY <= endY; currY+=towerSize) {
                    this->coorsX.push_back(tmpX+deltaX);
                    this->coorsY.push_back(currY+deltaY);
                }
            } else {
                for(int currY = tmpY-towerSize; currY > endY-towerSize; currY-=towerSize) {
                    this->coorsX.push_back(tmpX+deltaX);
                    this->coorsY.push_back(currY+deltaY);
                }
            }
        }
        return true;
    }
    return false;
}

bool UnderConstruction::clearStartCoors() {
    this->state = 0;
    this->coorsX.clear();
    this->coorsY.clear();
    return true;
}
