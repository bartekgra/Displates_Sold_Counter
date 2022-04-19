#include "display.h"


Display::Display(){
    matrix = new RGBmatrixPanel(A_L, B_L, C_L, D_L, CLK, LAT, OE, true, WIDTH, (uint8_t*)pin_list);

    matrix->begin();
    matrix->setTextSize(1);     // size 1 == 8 pixels high
    matrix->setTextWrap(false);
}

void Display::Loop(uint32_t num_to_print){
    matrix->fillScreen(matrix->Color333(0, 0, 0));

    UpdateShootingStars();

    matrix->setCursor(5, 2);
    uint32_t color_base = matrix->Color888(80, 0, 210);
    matrix->setTextColor(color_base);
    matrix->print(text[0]);

    matrix->setCursor(5, 13);
    matrix->setTextColor(color_base);
    matrix->print(text[1]);

    Print_Number(num_to_print);

    matrix->swapBuffers(false);
}

// Input a value 0 to 767 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Display::Wheel(uint32_t WheelPos) {
    WheelPos %= 768;
    if(WheelPos < 256) {
        return matrix->Color888(255 - WheelPos, WheelPos, 0);
    } else if(WheelPos < 512) {
        WheelPos -= 256;
        return matrix->Color888(0, 255-WheelPos, WheelPos);
    } else {
        WheelPos -= 512;
        return matrix->Color888(WheelPos, 0, 255 - WheelPos);
    }
}

void Display::Print_Number(uint32_t num_to_print){
    uint32_t single_num;
    uint32_t pos_x = 64 - offset_x_start - offset_x_next_number;
    uint32_t tmp_3_num = 0;
    uint32_t iter = 0;
    
    color_numb_pos += color_numb_step;
    color_numb_pos %= 768;

    do{
        single_num = num_to_print - ((num_to_print / 10) * 10);
        matrix->setTextColor(Wheel(color_numb_pos + iter * 40));
        matrix->setCursor(pos_x, 23);
        matrix->print(single_num);
        pos_x -= offset_x_next_number;
        tmp_3_num++;
        if(tmp_3_num == 3){
            pos_x -= 2;
            tmp_3_num = 0;
        }
        num_to_print /= 10;
        iter++;
    }while(num_to_print > 0);
}

ShootingStar* Display::GenerateShootingStar(){
    unsigned int tempPos = random(96);
    _coordinates_2d<double> startPos;
    if(tempPos < 64){
        startPos._x = (double)(tempPos);
        startPos._y = 0.0;
    }else{
        startPos._x = 0.0;
        startPos._y = (double)(tempPos - 64);
    }

    ShootingStar* tmpStar = new ShootingStar(
        startPos,
        {8.0 + (double)random(4), 8.0 + (double)random(4)},
        (double)random(15) / 100.0 + 0.2);

    return tmpStar;
}

void Display::UpdateShootingStars(){
    if(++actualCounterGenerateSS > CounterGenerateSS){
        actualCounterGenerateSS = 0;
        shootingstarVec.push_back(GenerateShootingStar());
    }

    for(int i = shootingstarVec.size() - 1; i >= 0; i--){
        ShootingStar *tempStar = shootingstarVec.at(i);
        tempStar->UpdateStarPos(*matrix);
        // shootingstarVec.at(i)->UpdateStarPos(*matrix);

        for(int tail_pos = 0; tail_pos < ShootingStar::getTailLength(); tail_pos++){
            uint16_t tempColor = matrix->Color888(
                tempStar->getStarColor(tail_pos, "red"),
                tempStar->getStarColor(tail_pos, "green"),
                tempStar->getStarColor(tail_pos, "blue"));
            matrix->drawPixel(
                tempStar->getXPos(tail_pos) - ShootingStar::getOffsetDisplay(),
                tempStar->getYPos(tail_pos) - ShootingStar::getOffsetDisplay(),
                tempColor);
        }
        
        if(shootingstarVec.at(i)->isOutOfDisplayFlag()){
            delete shootingstarVec.at(i);
            shootingstarVec.erase(shootingstarVec.begin() + i);
        }
    }
}
