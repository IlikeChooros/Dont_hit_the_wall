#ifndef _PLAYER_H
#define _PLAYER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "icons.h"


class Player{
    private:
        LiquidCrystal *_lcd;
        uint8_t _position;
        uint8_t _max_up_position;
        void draw();
        void clear();
    public:
        Player(LiquidCrystal *lcd, uint8_t max_up_postion);
        void move_up();
        void move_down();
        uint8_t get_position();  
        void _init_();  
        void draw_dead_player();
        void reset();
};

#endif