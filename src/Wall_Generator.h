#ifndef _WALL_GENENEARTOR_H
#define _WALL_GENENEARTOR__H
#include <Arduino.h>


class Wall_Generator 
{
    uint8_t _rows;
    uint8_t _collumns;
    uint8_t **_matrix;

    void move_walls();
    void push_to_end(uint8_t *last_collumn);
    uint8_t *random_last_collumn(uint8_t *last_collumn);
    public:
        Wall_Generator(uint8_t rows, uint8_t col);
        uint8_t **generate_next();
        void clear();
        uint8_t **current_wall_postions();
};

#endif