/*
 * note.h
 *
 *  Created on: Aug 3, 2025
 *      Author: Zhaoji Gou
 */

#ifndef INC_NOTE_H_
#define INC_NOTE_H_

#include <stdint.h>

/* ================= Frequency Definition (Unit: Hz) ================= */
#define REST         0     // Rest

/* ------------------ octave 0 ------------------ */
#define A0        28
#define AS0       29
#define B0        31

/* ------------------ octave 1 ------------------ */
#define C1        33
#define CS1       35
#define D1        37
#define DS1       39
#define E1        41
#define F1        44
#define FS1       46
#define G1        49
#define GS1       52
#define A1        55
#define AS1       58
#define B1        62

/* ------------------ octave 2 ------------------ */
#define C2        65
#define CS2       69
#define D2        73
#define DS2       78
#define E2        82
#define F2        87
#define FS2       93
#define G2        98
#define GS2      104
#define A2       110
#define AS2      117
#define B2       123

/* ------------------ octave 3 (Middle C) ------------------ */
#define C3       131
#define CS3      139
#define D3       147
#define DS3      156
#define E3       165
#define F3       175
#define FS3      185
#define G3       196
#define GS3      208
#define A3       220
#define AS3      233
#define B3       247

/* ------------------ octave 4 (standard A4=440Hz) ------------------ */
#define C4       262
#define CS4      277
#define D4       294
#define DS4      311
#define E4       330
#define F4       349
#define FS4      370
#define G4       392
#define GS4      415
#define A4       440
#define AS4      466
#define B4       494

/* ------------------ octave 5 ------------------ */
#define C5       523
#define CS5      554
#define D5       587
#define DS5      622
#define E5       659
#define F5       698
#define FS5      740
#define G5       784
#define GS5      831
#define A5       880
#define AS5      932
#define B5       988

/* ------------------ octave 6 ------------------ */
#define C6      1047
#define CS6     1109
#define D6      1175
#define DS6     1245
#define E6      1319
#define F6      1397
#define FS6     1480
#define G6      1568
#define GS6     1661
#define A6      1760
#define AS6     1865
#define B6      1976

/* ------------------ octave 7 ------------------ */
#define C7      2093
#define CS7     2217
#define D7      2349
#define DS7     2489
#define E7      2637
#define F7      2794
#define FS7     2960
#define G7      3136
#define GS7     3322
#define A7      3520
#define AS7     3729
#define B7      3951

/* ------------------ octave 8 ------------------ */
#define C8      4186

/* ================= Note (Sharp/Flat) ================= */
#define Db1 CS1
#define Eb1 DS1
#define Gb1 FS1
#define Ab1 GS1
#define Bb1 AS1

#define Db2 CS2
#define Eb2 DS2
#define Gb2 FS2
#define Ab2 GS2
#define Bb2 AS2

#define Db3 CS3
#define Eb3 DS3
#define Gb3 FS3
#define Ab3 GS3
#define Bb3 AS3

#define Db4 CS4
#define Eb4 DS4
#define Gb4 FS4
#define Ab4 GS4
#define Bb4 AS4

#define Db5 CS5
#define Eb5 DS5
#define Gb5 FS5
#define Ab5 GS5
#define Bb5 AS5

#define Db6 CS6
#define Eb6 DS6
#define Gb6 FS6
#define Ab6 GS6
#define Bb6 AS6

#define Db7 CS7
#define Eb7 DS7
#define Gb7 FS7
#define Ab7 GS7
#define Bb7 AS7

#endif /* INC_NOTE_H_ */
