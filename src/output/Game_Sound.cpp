#include "Game_Sound.h"

Game_Sound::Game_Sound(uint16_t tempo, uint8_t buzzer_pin, int16_t *music, uint32_t size)
{
    _wholenote = (60*1000*4)/ tempo;
    _buzzer_pin = buzzer_pin;
    _music=music;
    _size=size;
    _note_pointer=0;
    _interval=0;
    _play_song=false;
    // _timer->on_times_up(this->play_note);
}

void Game_Sound::_init_()
{
    pinMode(_buzzer_pin, OUTPUT);
}

void Game_Sound::start()
{
    _start_time=millis();
    _play_song=true;
}

void Game_Sound::play_note()
{
    int16_t note=_music[_note_pointer++];
    int16_t divider=_music[_note_pointer++];
    tone(_buzzer_pin,note);
    set_interval(convert_to_millis(divider));

    if (_note_pointer-1>=_size)
    {
        _note_pointer=0;
    }
}

void Game_Sound::check()
{
    uint32_t currentTime = millis();

    if (currentTime-_start_time>_interval)
    {
        noTone(_buzzer_pin);
        play_note();
        _start_time=currentTime;
    }
}
void Game_Sound::reset()
{
    _note_pointer=0;
    _interval=0;
}

//PRIVATE

uint32_t Game_Sound::convert_to_millis(int16_t divider)
{
    if (divider>0)
    {
        return _wholenote*4/divider;
    }
    else{
        return (_wholenote*1.5*4)/abs(divider);
    }
}

void Game_Sound::set_interval(uint16_t interval)
{
    _interval=interval;
}
