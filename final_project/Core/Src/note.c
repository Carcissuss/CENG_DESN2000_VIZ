/*
 * note.c
 *
 *  Created on: Aug 3, 2025
 *      Author: Zhaoji Gou
 */

#include "note.h"
#include "main.h"
#include "coast.h"

// 音符持续时间定义（毫秒）
#define Q 500  // quarter note
#define H 1000 // half note
#define W 2000 // full note

extern void coast_asm_delay(uint32_t millisecond);
void Play_Twinkle_Star(void) {
    // verse one：C C G G | A A G - | F F E E | D D C - |
    Play_Note(C4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS); // 音符间50ms间隔
    Play_Note(C4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(A4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(A4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_1000MS, 50);
    coast_asm_delay(DELAY_100MS); // 长音后100ms间隔

    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(D4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(D4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(C4, DELAY_1000MS, 50);
    coast_asm_delay(DELAY_100MS);

    // verse two：G G F F | E E D - | G G F F | E E D - |
    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(D4, DELAY_1000MS, 50);
    coast_asm_delay(DELAY_100MS);

    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(D4, DELAY_1000MS, 50);
    coast_asm_delay(DELAY_100MS);

    // verse three：C C G G | A A G - | F F E E | D D C - |
    Play_Note(C4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(C4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(A4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(A4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(G4, DELAY_1000MS, 50);
    coast_asm_delay(DELAY_100MS);

    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(F4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(E4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(D4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(D4, DELAY_500MS, 50);
    coast_asm_delay(DELAY_50MS);
    Play_Note(C4, DELAY_1000MS, 50);
    coast_asm_delay(DELAY_100MS);

    // ending rest note
    Stop_Sound();
    coast_asm_delay(DELAY_1000MS);
}

