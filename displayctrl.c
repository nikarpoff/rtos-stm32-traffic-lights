/* displayctrl.c */
#include "cmsis_os.h"
#include "main.h"   
#include "AsciiLib.h"
#include "LCD_ILI9325.h"
#include "displayctrl.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

// ??????????? ??? ?????????
#define SET 1
#define RESET 0
#define CNTTXTKYPD 10

// ????????? ?????????
#define FSMST_RD  0 // RD
#define FSMST_OG  1 // OG
#define FSMST_YE  2 // YE
#define FSMST_AQ  3 // AQ
#define FSMST_GN  4 // GN

#define FSMST_RD1  5
#define FSMST_OG1  6
#define FSMST_YE1  7
#define FSMST_AQ1  8
#define FSMST_GN1  9

#define FSMST_RD2  10
#define FSMST_OG2  11
#define FSMST_YE2  12
#define FSMST_AQ2  13
#define FSMST_GN2  14

#define FSMST_RD3  15
#define FSMST_OG3  16
#define FSMST_YE3  17
#define FSMST_AQ3  18
#define FSMST_GN3  19

// ??????????? ?????? ? ??????? RGB565
#define RED      0xF800
#define ORANGE   0xFC00
#define YELLOW   0xFFE0
#define AQUAMARINE 0x7FF
#define GREEN    0x07E0

#define RED_DARK      0x6800
#define ORANGE_DARK   0xBC63
#define YELLOW_DARK   0xB5C0
#define AQUAMARINE_DARK 0x4411
#define GREEN_DARK    0x0340

#define BLACK   0x0000

// ??????? ???????
#define DISPLAY_WIDTH   240
#define DISPLAY_HEIGHT  320

// ?????? ? ?????????? ????? ????????????
#define LIGHT_RADIUS    15
#define LIGHT_SPACING   20

// ???? ????????? ??????????
typedef enum { T, Y, M } LightState;

// ??????? ????????? ?????????
typedef struct {
    LightState red;
    LightState orange;
    LightState yellow;
    LightState aquamarine;
    LightState green;
} TrafficLightState;

volatile TrafficLightState currentState;
volatile uint8_t blink_flag = 0;

static union FLAGSDISP {
    unsigned char fbyte;
    struct {
        unsigned fkypd4:1; // ???? ??? ??????????
        unsigned fmovepic:1; // ???? ??? ???????? ???????????
    } flagupdt;
} fDisp;

static struct DATDISP {
    struct {
        uint8_t cnt; 
        uint16_t kypd4status; 
    } kypd4dat;
    union {
        uint8_t serialbyte[3];
        struct {
            uint16_t key;
            uint8_t automatic_state; 
        } dat;
    } trafficData;
} tDATdisp;

void dispkypd4(uint16_t kypd4status);
void dispuartTx(void);
void dispuartRx(void);
void dispFRAM(uint8_t flagrw);
void updateState(void);

void InitDisplay(void) {
    init_lcd_ili9325();
    fDisp.fbyte = RESET;
    tDATdisp.kypd4dat.kypd4status = 0;
    tDATdisp.trafficData.dat.automatic_state = FSMST_RD;
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

void drawTrafficLight(TrafficLightState state) {
    uint16_t centerX = DISPLAY_WIDTH / 2;
    uint16_t startY = (DISPLAY_HEIGHT - (5 * LIGHT_RADIUS * 2 + 4 * LIGHT_SPACING)) / 2;

    uint16_t yPositions[5];
    for (int i = 0; i < 5; i++) {
        yPositions[i] = startY + i * (2 * LIGHT_RADIUS + LIGHT_SPACING) + LIGHT_RADIUS;
    }

    uint16_t colors[5] = {RED, ORANGE, YELLOW, AQUAMARINE, GREEN};
    uint16_t colorsDark[5] = {RED_DARK, ORANGE_DARK, YELLOW_DARK, AQUAMARINE_DARK, GREEN_DARK};

    LightState states[5] = {state.red, state.orange, state.yellow, state.aquamarine, state.green};

    for (int i = 0; i < 5; i++) {
        uint16_t color = YELLOW;
        switch (states[i]) {
            case Y:
                color = colors[i];
                break;
            case M:
                color = blink_flag ? colors[i] : BLACK;
                break;
            case T:
            default:
								color = colors[i];
                //color = colorsDark[i];
                break;
        }
        drawCircle(centerX, yPositions[i], LIGHT_RADIUS, color);
    }
}

void ProcessDisplay(void) {
    fDisp.flagupdt.fkypd4 = SET;
    osDelay(25);
    drawTrafficLight(currentState);
}

void dispkypd4(uint16_t kypd4status) {
    tDATdisp.trafficData.dat.automatic_state = FSMST_YE3;
    updateState();
}

void updateState(void) {
    // ???????????? ????????? (?????????? ???????????????? ????)
}
