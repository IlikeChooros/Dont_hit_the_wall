#ifndef _WALL_DRAWER_H
#define _WALL_DRAWER_H

#include "icons.h"
#include <Arduino.h>
#include <LiquidCrystal.h>



class Wall_Drawer
{
private:
    LiquidCrystal *_lcd;
    uint8_t _rows;
    uint8_t _collumns;
public:
    Wall_Drawer(LiquidCrystal *lcd, uint8_t rows, uint8_t collumns);
    void draw(uint8_t **wall_matrix, uint8_t player_postion);
    void _init_();
};

#endif