#include "ws2812b.h"

uint16_t ledData[LED_DATA_SIZE];

uint8_t colors[6][3] = {
    {200, 0, 0},   // Piros
    {200, 20, 0}, // Narancs
    {200, 200, 0}, // Sárga
    {0, 200, 0},   // Zöld
    {0, 0, 200},   // Kék
    {74, 0, 130}   // Ibolya
};

uint8_t colorIndex = 0;  // Ciklikus színindex

uint8_t colorOffset = 0;  // Színindex-eltolás (lépésenként növekszik)

void SetLEDColor(uint8_t ledIndex, uint8_t red, uint8_t green, uint8_t blue) {
    if (ledIndex >= LED_COUNT) return;

    uint32_t color = (green << 16) | (red << 8) | blue;  // GRB sorrend

    // A LED 24 bitjének beállítása (8 bit zöld, 8 bit piros, 8 bit kék)
    for (int i = 0; i < 24; i++) {
        // A megfelelő bit eltolása és ellenőrzése
        if ((color & (1 << (23 - i))) != 0) {
            ledData[ledIndex * 24 + i] = WS2812B_1;
        } else {
            ledData[ledIndex * 24 + i] = WS2812B_0;
        }
    }
}

void WS2812B_Update(void) {
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)ledData, LED_DATA_SIZE);

    // DMA újraindítása
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)ledData, LED_DATA_SIZE);
}

void SetAllLEDs(uint8_t red, uint8_t green, uint8_t blue) {
    for (int i = 0; i < LED_COUNT; i++) {
        SetLEDColor(i, red, green, blue);
    }
    WS2812B_Update();
}
// Színek frissítése színkör-hatás létrehozásához
void UpdateColorWheel(void) {
    for (int i = 0; i < LED_COUNT; i++) {
        uint8_t colorIdx = ((colorOffset + i) / 4) % 6;  // 4-es blokkok megtartása, de eltolva
        SetLEDColor(i, colors[colorIdx][0], colors[colorIdx][1], colors[colorIdx][2]);
    }

    WS2812B_Update();  // LED-ek frissítése

    colorOffset = (colorOffset + 1) % (6 * 4);  // Egyesével tolás, de a 4-es blokkok megtartásával
}


// Callback függvény, amelyet a HAL hív meg a DMA átvitel befejeztével
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {

        HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);

}
