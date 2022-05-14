#ifndef _ICONS_H
#define _ICONS_H

#include <Arduino.h>


#define CLEAR_CHUNK 1
#define WALL 2
#define PLAYER 0
#define DEAD_PLAYER 3


static uint8_t main_player[8]=
{
    B00110,
    B01111,
    B11001,
    B11001,
    B11111,
    B01111,
    B01101,
    B01101,
};

static uint8_t end_player[8]=
{
    B00000,
    B00000,
    B01010,
    B00110,
    B11111,
    B11111,
    B01101,
    B01101,
};


static uint8_t clear_chunk[8]=
{
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
};

static uint8_t wall_lcd [8]=
{
    B11110,
    B11101,
    B11011,
    B10110,
    B01101,
    B11011,
    B10111,
    B01111,
};

#endif