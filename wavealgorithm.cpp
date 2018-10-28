#include "wavealgorithm.h"

WaveAlgorithm::WaveAlgorithm() {

}

//void Field::waveAlgorithm(int x, int y) {
//    qDebug() << "Field::waveAlgorithm() :: X: " << x << " Y: " << y;
//    if(x == -1 && y == -1) {
//        if(isSetExitPoint()) {
//            waveAlgorithm(exitPointX, exitPointY);
//            return;
//        }
//    }
//    if(!getCell(x, y)->isTerrain() && !getCell(x, y)->getTower()) {
//        for(int tmpX = 0; tmpX < getSizeX(); tmpX++) {
//            for(int tmpY = 0; tmpY < getSizeY(); tmpY++) {
//                clearStepCell(tmpX, tmpY);
//            }
//        }
//        setStepCell(x, y, 1);
//        waveStep(x, y, 1);
//    }
//}

//void Field::waveStep(int x, int y, int step) {
//#ifdef CIRCLET8
//    bool mass[3][3];
//    int nextStep = step+1;

//    for(int tmpY = -1; tmpY < 2; tmpY++)
//        for(int tmpX = -1; tmpX < 2; tmpX++)
//            mass[tmpX+1][tmpY+1] = setNumOfCell(x + tmpX, y + tmpY, nextStep);

//    for(int tmpY = -1; tmpY < 2; tmpY++)
//        for(int tmpX = -1; tmpX < 2; tmpX++)
//            if(mass[tmpX+1][tmpY+1])
//                waveStep(x + tmpX, y + tmpY, nextStep);
//#else
//    bool mass[4];
//    int nextStep = step+1;
//    int x1 = x-1, x2 = x, x3 = x+1;
//    int y1 = y-1, y2 = y, y3 = y+1;

//    mass[0] = setNumOfCell(x1, y2, nextStep);
//    mass[1] = setNumOfCell(x2, y1, nextStep);
//    mass[2] = setNumOfCell(x2, y3, nextStep);
//    mass[3] = setNumOfCell(x3, y2, nextStep);

//    if(mass[0])
//        waveStep(x1, y2, nextStep);
//    if(mass[1])
//        waveStep(x2, y1, nextStep);
//    if(mass[2])
//        waveStep(x2, y3, nextStep);
//    if(mass[3])
//        waveStep(x3, y2, nextStep);
//#endif
//}

//int WaveAlgorithm::getNumStep(int x, int y) {
//    if(x >= 0 && x < getSizeX())
//        if(y >= 0 && y < getSizeY())
//            if(!getCell(x, y)->isTerrain())
//                if(!getCell(x, y)->getTower())
//                    return getStepCell(x, y);
//    return 0;
//}

//int WaveAlgorithm::getStepCell(int x, int y) {
//    return field[sizeFieldX*y + x].step;
//}

//bool WaveAlgorithm::setNumOfCell(int x, int y, int step) {
//    if(x >= 0 && x < getSizeX()) {
//        if(y >= 0 && y < getSizeY()) {
//            if(!getCell(x, y)->isTerrain() && !getCell(x, y)->getTower()) {
//                if(getStepCell(x, y) > step || getStepCell(x, y) == 0) {
//                    setStepCell(x, y, step);
//                    return true;
//                }
//            }
//        }
//    }
//    return false;
//}

//void WaveAlgorithm::setStepCell(int x, int y, int step) {
//    field[sizeFieldX*y + x].step = step;
//}

//void WaveAlgorithm::clearStepCell(int x, int y) {
//    field[sizeFieldX*y + x].step = 0;
//}
