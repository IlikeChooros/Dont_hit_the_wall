#include "Wall_Drawer.h"


Wall_Drawer::Wall_Drawer(LiquidCrystal *lcd, uint8_t rows, uint8_t col)
{
    _lcd=lcd;
    _rows=rows;
    _collumns=col;
}

void Wall_Drawer::draw(uint8_t **wall_matrix, uint8_t player_position)
{
    for (uint8_t i=0;i<_rows;i++)
    {
        uint8_t j=0;
        if (i==player_position)
        {
            j=1;
        }
        for (;j<_collumns;j++)
        {
            _lcd->setCursor(j,i);
            if (wall_matrix[i][j]==0)
            {
                _lcd->write(byte(CLEAR_CHUNK));
            }
            else {
                _lcd->write(byte(WALL));
            }
        }
    }
}

void Wall_Drawer::_init_()
{
    _lcd->createChar(CLEAR_CHUNK, clear_chunk);
    _lcd->createChar(WALL, wall_lcd);
}