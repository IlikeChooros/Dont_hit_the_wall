#include "Wall_Generator.h"

Wall_Generator::Wall_Generator (uint8_t rows, uint8_t col)
{
    _rows = rows;
    _collumns = col;
    _matrix = new uint8_t*[_rows];
    clear();
    randomSeed(analogRead(0));
}

uint8_t** Wall_Generator::generate_next()
{
    uint8_t last_collumn [_rows];
    push_to_end(random_last_collumn(last_collumn));

    return _matrix;
}

void Wall_Generator::clear()
{
    for (uint8_t i=0; i<_rows; ++i)
    {
        _matrix[i] = new uint8_t[_collumns];
        for (uint8_t j=0;j<_collumns;j++)
        {
            _matrix[i][j] = 0;
        }
    }
}

uint8_t **Wall_Generator::current_wall_postions()
{
    return _matrix;
}

// PRIVATE

void Wall_Generator::move_walls()
{
    for (uint8_t i=0; i<_rows; i++)
    {
        for (uint8_t j=0; j<_collumns-1;j++)
        {
            _matrix[i][j]=_matrix[i][j+1];
        }
    }
}

void Wall_Generator::push_to_end(uint8_t *last_collumn)
{
    move_walls();
    for (int i=0; i<_rows;i++)
    {
        _matrix[i][_collumns-1]=last_collumn[i];
    }
}

uint8_t* Wall_Generator::random_last_collumn(uint8_t *last_collumn)
{
    int8_t wall_row_index=-1;
    for (int i=0;i<_rows;i++)
    {
        last_collumn[i]=0;
        if (_matrix[i][_collumns-1]==1)
        {
            wall_row_index=i;
        }
    }
    uint8_t wall = random(0, 2);
    uint8_t row;
    if (wall_row_index<0){
        row = random(0, _rows);
    }
    else{
        row = wall_row_index;
    }

    last_collumn[row]=wall;
    return last_collumn;
}

