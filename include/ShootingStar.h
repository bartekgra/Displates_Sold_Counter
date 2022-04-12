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
    const int offsetDisplay = 5;

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
    void DrawPoint(RGBmatrixPanel &matrix, uint8_t power, int index);
    
public:
    ShootingStar(_coordinates_2d<double> pos, _coordinates_2d<double> dir, double speed);
    ~ShootingStar();

    void UpdateStarPos(RGBmatrixPanel &matrix);
    bool isOutOfDisplayFlag(){return outOfDisplayFlag;};
    void UpdateOutOfDisplayFlag();

    static void NormalizedVec(_coordinates_2d<double> &vec);

    static int numOfDestruct;
};

