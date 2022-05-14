#include "Player.h"

Player::Player (LiquidCrystal *lcd, uint8_t max_up_postion)
{
    _lcd=lcd;
    _max_up_position=max_up_postion-1;
    _position=_max_up_position;
}

void Player::_init_()
{
    _lcd->createChar(PLAYER,main_player);
    _lcd->createChar(DEAD_PLAYER,end_player);
    _lcd->createChar(CLEAR_CHUNK, clear_chunk);

    draw();
}

void Player::reset()
{
    _position=_max_up_position;
    draw();
}

void Player::move_down()
{
    if (_position<_max_up_position)
    {
        clear();
        _position++;
        draw();
    }
}

void Player::move_up()
{
    if (_position>0)
    {
        clear();
        _position--;
        draw();
    }
}

uint8_t Player::get_position()
{
    return _position;
}

void Player::draw_dead_player()
{
    _lcd->write(byte(DEAD_PLAYER));
}

// PRIVATE
void Player::draw()
{
    _lcd->setCursor(0,_position);
    _lcd->write(byte(PLAYER));
}

void Player::clear()
{
    _lcd->setCursor(0,_position);
    _lcd->write(byte(CLEAR_CHUNK));
}


