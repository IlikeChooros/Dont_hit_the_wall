#include "Game_Sound.h"

typedef void (*fptr)();

fptr play(Game_Sound *gs) {
    return gs->play_note;
}

Game_Sound::Game_Sound(uint8_t tempo, uint8_t buzzer_pin, int16_t *music, uint8_t size)
{
    _wholenote = (60*1000*4)/ tempo;
    _buzzer_pin = buzzer_pin;
    _music=music;
    _size=size;
    _note_pointer=0;
    _timer=new Timer(0);
    void (*play)() = play(this);

    // _timer->on_times_up(this->play_note);
}

void Game_Sound::_init_()
{
    pinMode(_buzzer_pin, OUTPUT);
}

void Game_Sound::start()
{
    _timer->start();
}

void Game_Sound::play_note()
{
    int16_t note=_music[_note_pointer++];
    int16_t divider=_music[_note_pointer++];
    tone(_buzzer_pin,note);
    _timer->set_interval(convert_to_millis(divider));

    if (_note_pointer>=_size)
    {
        _note_pointer=0;
    }
}

void Game_Sound::play_music()
{
    _timer->check();
}
//PRIVATE

uint16_t Game_Sound::convert_to_millis(int16_t divider)
{
    if (divider>0)
    {
        return _wholenote/divider;
    }
    else{
        return (_wholenote*(-1.5))/divider;
    }
}
