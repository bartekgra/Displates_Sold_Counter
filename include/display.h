#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library
// #include <DFRobot_RGBMatrix.h> // Hardware-specific library
#include <Wire.h>
#include <SPI.h>
#include "ShootingStar.h"


#define CLK     19
#define LAT     21
#define OE      22
#define A_L     16
#define B_L     17
#define C_L     5
#define D_L     18
#define WIDTH   64

class Display{
public:
    Display();
    void Loop(uint32_t num_to_print);

private:
    uint16_t Wheel(uint32_t WheelPos);
    void Print_Number(uint32_t num_to_print);

    void UpdateShootingStars();
    ShootingStar* GenerateShootingStar();
    std::vector<ShootingStar*> shootingstarVec;
    uint32_t actualCounterGenerateSS = 0;
    const uint32_t CounterGenerateSS = 40;

    uint32_t color_pos = 0;
    
    const uint32_t color_numb_step = 10;
    uint32_t color_numb_pos = 0;

    const uint32_t offset_x_start = 1;
    const uint32_t offset_x_next_number = 6;

    const uint8_t pin_list[6]{
        15,
        2,
        4,
        12,
        14,
        27
    };
    const String text[2] = {
        "DISPLATES",
        "SOLD:"
    };

    RGBmatrixPanel *matrix;// = RGBmatrixPanel(A, B, C, D, CLK, LAT, OE, false, WIDTH, pin_list);
};