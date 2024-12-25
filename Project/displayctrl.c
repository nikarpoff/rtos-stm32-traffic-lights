/* displayctrl.c */
#include "cmsis_os.h"
#include "main.h"
#include "LCD_ILI9325.h"
#include "displayctrl.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// Display params
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  320

// Traffic light params
uint16_t trafficsNumber;    // number of traffic lights
uint16_t lightsNumber;      // number of lights (circles) in traffic lights
uint16_t lightHorSpacing;   // horizontal distance between centers of two circles
uint16_t lightVerSpacing;   // vertical distance between centers of two circles
uint16_t horShift;          // horizontal shift for traffic lights
uint16_t verShift;          // vertical shift for traffic lights
uint16_t lightRadius;       // radius of one ~more~ light


void InitDisplay(void, uint16_t tlNumber, uint16_t lghtsNumber) {
    init_lcd_ili9325();
    
    trafficsNumber = tlNumber;
    lightsNumber = lghtsNumber;

    // You should to know that we swaped width and height (cause we want to use albume orientation)
    lightRadius = (DISPLAY_HEIGHT / trafficsNumber) / 2;
    
    // Uniformly split space
    if (lightsNumber == 1) {
        lightVerSpacing = DISPLAY_WIDTH / 2;
        verShift = 0;
    }
    else {
        lightVerSpacing = DISPLAY_WIDTH / lightsNumber;
        verShift = lightVerSpacing / 2;
    }

    lightHorSpacing = DISPLAY_HEIGHT / trafficsNumber;
    horShift = lightHorSpacing / 2;
}

void drawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color) {
    for (int16_t y1 = -radius; y1 <= radius; y1++) {
        for (int16_t x1 = -radius; x1 <= radius; x1++) {
            if (x1 * x1 + y1 * y1 <= radius * radius) {
                lcd_set_pixel(x + x1, y + y1, color);
            }
        }
    }
}

void drawTrafficLight(uint16_t idTraffic, uint16_t idLight, uint32_t color) {
    uint16_t centerX = lightVerSpacing * (idLight + 1) - verShift;
    uint16_t centerY = lightHorSpacing * (idTraffic + 1) - horShift;

    drawCircle(centerX, centerY, lightRadius, color);
}
