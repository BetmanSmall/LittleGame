#ifndef WAVEALGORITHM_H
#define WAVEALGORITHM_H

#define CIRCLET8

class WaveAlgorithm
{
public:
    WaveAlgorithm();
    void waveAlgorithm(int x = -1, int y = -1);
    void waveStep(int x, int y, int step);

    int getNumStep(int x, int y);
    int getStepCell(int x, int y);
    bool setNumOfCell(int x, int y, int step);
    void setStepCell(int x, int y, int step);
    void clearStepCell(int x, int y);
};

#endif // WAVEALGORITHM_H
