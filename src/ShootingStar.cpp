#include "ShootingStar.h"

int ShootingStar::numOfDestruct = 0;

ShootingStar::ShootingStar(_coordinates_2d<double> pos, _coordinates_2d<double> dir, double speed)
{
    NormalizedVec(dir);
    this->dir = dir;
    this->pos = pos;
    this->speed = speed;

    for(int i = 0; i < TAIL_LENGTH; i++){
        pos_to_dis[i]._x = -1;
        pos_to_dis[i]._y = -1;
    }
    pos_to_dis[0]._x = offsetDisplay + pos._x;
    pos_to_dis[0]._y = offsetDisplay + pos._y;
    SetTail();
}

ShootingStar::~ShootingStar()
{
    numOfDestruct++;
}

void ShootingStar::UpdateStarPos(RGBmatrixPanel &matrix)
{
    pos._x += dir._x * speed;
    pos._y += dir._y * speed;
    SetTail();

    for(int i = TAIL_LENGTH - 1; i > 0; i--){
        colorTail[i].red = 255 * (TAIL_LENGTH - i) / TAIL_LENGTH * 0.3;
        colorTail[i].green = 255 * (TAIL_LENGTH - i) / TAIL_LENGTH * 0.3;
        colorTail[i].blue = 255 * (TAIL_LENGTH - i) / TAIL_LENGTH * 0.1;

        DrawPoint(matrix, 255 * (TAIL_LENGTH - i) / TAIL_LENGTH * 0.3, i);
    }
    
    colorTail[0].red = 255;
    colorTail[0].green = 255;
    colorTail[0].blue = 255 * 0.3;
    
    DrawPoint(matrix, 255, 0);

    UpdateOutOfDisplayFlag();
    
}

void ShootingStar::DrawPoint(RGBmatrixPanel &matrix, uint8_t power, int index){
        uint16_t tempColor = matrix.Color888(power, power, power * 0.3);
        matrix.drawPixel(pos_to_dis[index]._x - offsetDisplay, pos_to_dis[index]._y - offsetDisplay, tempColor);
}

void ShootingStar::SetTail(){
    int temp_x = offsetDisplay + pos._x;
    int temp_y = offsetDisplay + pos._y;

    if(temp_x != pos_to_dis[0]._x && temp_y != pos_to_dis[0]._y){
        for(int i = TAIL_LENGTH - 1; i > 0; i--){
            pos_to_dis[i]._x = pos_to_dis[i - 1]._x;
            pos_to_dis[i]._y = pos_to_dis[i - 1]._y;
        }
        pos_to_dis[0]._x = temp_x;
        pos_to_dis[0]._y = temp_y;
    }
}

void ShootingStar::UpdateOutOfDisplayFlag(){
    bool tmp_flag = true;
    for(int i = 0; i < TAIL_LENGTH; i++){
        if(!(pos_to_dis[i]._x - offsetDisplay < 0 ||
         pos_to_dis[i]._x - offsetDisplay >= 64 ||
          pos_to_dis[i]._y - offsetDisplay < 0 ||
           pos_to_dis[i]._y - offsetDisplay >= 32)){
            tmp_flag = false;
        }
    }
    outOfDisplayFlag = tmp_flag;
}

void ShootingStar::NormalizedVec(_coordinates_2d<double> &vec){
    if(vec._x != 0 || vec._y != 0){
        double temp = sqrt(vec._x * vec._x + vec._y * vec._y);
        vec._x /= temp;
        vec._y /= temp;
    }
}
