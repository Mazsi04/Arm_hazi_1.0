#ifndef WS2812B_H
#define WS2812B_H

#include "main.h"  // main.h tartalmazza a HAL library definíciókat, így itt nem kell külön "stm32f4xx_hal.h"

#define LED_COUNT 24
#define LED_DATA_SIZE (LED_COUNT * 24)
#define WS2812B_0 8
#define WS2812B_1 16

// Külső változó deklarációja (feltételezve, hogy htim3-t a main.c-ben definiáltad)
extern TIM_HandleTypeDef htim3;

// Globális tömb a LED adatok tárolásához
extern uint16_t ledData[LED_DATA_SIZE];

// Függvény prototípusok
void SetLEDColor(uint8_t ledIndex, uint8_t red, uint8_t green, uint8_t blue);
void WS2812B_Update(void);
void SetAllLEDs(uint8_t red, uint8_t green, uint8_t blue);
void UpdateColorWheel(void);

#endif /* WS2812B_H */
