/* displayctrl.h */
#ifndef DISPLAYCTRL_H
#define DISPLAYCTRL_H

#include <stdint.h>

void InitDisplay(uint16_t tlNumber, uint16_t lghtsNumber);
void DrawTrafficLight(uint16_t idTraffic, uint16_t idLight, uint32_t color);

#endif
