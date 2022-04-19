#ifndef SHOOTINGSTAR_H_
#define SHOOTINGSTAR_H_

#include "math.h"
#include <RGBmatrixPanel.h> // Hardware-specific library

template <typename T>
struct _coordinates_2d{
    T _x;
    T _y;
};

class ShootingStar
{
private:
    const static int TAIL_LENGTH = 8;
    const static int offsetDisplay = 5;

    _coordinates_2d<double> pos;
    _coordinates_2d<double> dir;
    double speed;

    _coordinates_2d<int> pos_to_dis[TAIL_LENGTH];

    bool outOfDisplayFlag = false;

    struct{
        uint8_t red;
        uint8_t green;
        uint8_t blue;
    }colorTail[TAIL_LENGTH];

    void SetTail();

public:
    ShootingStar(_coordinates_2d<double> pos, _coordinates_2d<double> dir, double speed);
    ~ShootingStar();

    void UpdateStarPos(RGBmatrixPanel &matrix);
    bool isOutOfDisplayFlag(){return outOfDisplayFlag;};
    void UpdateOutOfDisplayFlag();
    uint8_t getStarColor(int index, const char* rgbColor);
    int getXPos(int index){return pos_to_dis[index]._x;};
    int getYPos(int index){return pos_to_dis[index]._y;};

    static int getTailLength(){return TAIL_LENGTH;};
    static int getOffsetDisplay(){return offsetDisplay;};
    static void NormalizedVec(_coordinates_2d<double> &vec);

    static int numOfDestruct;
};

#endif