#include "src\Wall_Generator.h"
#include "src\input\Button.h"
#include "src\output\Wall_Drawer.h"
#include "src\output\Player.h"
#include "src\output\Game_Sound.h"
#include "src\Timer.h"

#include <LiquidCrystal.h>

const uint8_t rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd (rs,en,d4,d5,d6,d7);

#define INITIAL_CURRENT_WALL_INTERVAL 500
#define INITIAL_CHANGE_SPEED_INTERVAL 6*1000
#define MOVE_BUTTON_PIN 7
#define RESET_BUTTON_PIN 9
#define NUMBER_OF_ROWS 2
#define NUMBER_OF_COLLUMNS 16
#define BUZZER_PIN 13

uint16_t points=0;
uint16_t current_wall_interval=INITIAL_CURRENT_WALL_INTERVAL;
uint16_t current_change_speed_interval = INITIAL_CHANGE_SPEED_INTERVAL;
Timer wall_timer(current_wall_interval);
Timer change_speed_timer(current_change_speed_interval);
Timer counter (1000);
Button move_button(MOVE_BUTTON_PIN);
Button reset_button(RESET_BUTTON_PIN);

Wall_Generator wall_gen(NUMBER_OF_ROWS,NUMBER_OF_COLLUMNS);

Wall_Drawer wall_drawer(&lcd,NUMBER_OF_ROWS,NUMBER_OF_COLLUMNS);
Player player(&lcd, NUMBER_OF_ROWS);

bool isLost = false;

int16_t ending_music[]=
{
    NOTE_D4,8, NOTE_E4,8, NOTE_F4,8, NOTE_G4,8, NOTE_E4,4, NOTE_C4,8, NOTE_D4,1,
};

int16_t tetris[] = { // tempo = 120
  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,

  NOTE_E5, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_C5,8,  NOTE_B4,8,
  NOTE_A4, 4,  NOTE_A4,8,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, -4,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,8,  NOTE_A4,4,  NOTE_B4,8,  NOTE_C5,8,

  NOTE_D5, -4,  NOTE_F5,8,  NOTE_A5,4,  NOTE_G5,8,  NOTE_F5,8,
  NOTE_E5, -4,  NOTE_C5,8,  NOTE_E5,4,  NOTE_D5,8,  NOTE_C5,8,
  NOTE_B4, 4,  NOTE_B4,8,  NOTE_C5,8,  NOTE_D5,4,  NOTE_E5,4,
  NOTE_C5, 4,  NOTE_A4,4,  NOTE_A4,4, REST, 4,
  

  NOTE_E5,2,  NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,2,   NOTE_A4,2,
  NOTE_GS4,2,  NOTE_B4,4,  REST,8, 
  NOTE_E5,2,   NOTE_C5,2,
  NOTE_D5,2,   NOTE_B4,2,
  NOTE_C5,4,   NOTE_E5,4,  NOTE_A5,2,
  NOTE_GS5,2,
};

int16_t ultra_epic[]={ // tempo = 140

  NOTE_A5, -8, NOTE_G5, -4, //50
  NOTE_A5, -8, NOTE_G5, -4,
  NOTE_AS5, 8, NOTE_A5, 8, NOTE_G5, 8, NOTE_F5, 8,
  NOTE_A5, -8, NOTE_G5, -8, NOTE_D5, 8,
  NOTE_A5, -8, NOTE_G5, -8, NOTE_D5, 8,
  NOTE_A5, -8, NOTE_G5, -8, NOTE_D5, 8,

  NOTE_AS5, 4, NOTE_C6, 4, NOTE_A5, 4, NOTE_AS5, 4,
  NOTE_G5,16, NOTE_D5,16, NOTE_D6,16, NOTE_D5,16, NOTE_C6,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16,//56 //r
  NOTE_A5,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16, NOTE_A5,16, NOTE_D5,16, NOTE_G5,16, NOTE_D5,16,
  NOTE_A5,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16, NOTE_C6,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16,
  NOTE_A5,16, NOTE_D5,16, NOTE_F5,16, NOTE_D5,16, NOTE_A5,16, NOTE_D5,16, NOTE_G5,16, NOTE_D5,16,

  NOTE_G5,16, NOTE_D5,16, NOTE_D6,16, NOTE_D5,16, NOTE_C6,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16,//61
  NOTE_A5,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16, NOTE_A5,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16,
  NOTE_A5,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16, NOTE_C6,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16,
  NOTE_A5,16, NOTE_D5,16, NOTE_F5,16, NOTE_D5,16, NOTE_A5,16, NOTE_D5,16, NOTE_G5,16, NOTE_D5,16,
  NOTE_G5,16, NOTE_D5,16, NOTE_D6,16, NOTE_D5,16, NOTE_C6,16, NOTE_D5,16, NOTE_AS5,16, NOTE_D5,16,

  NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
  NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
  NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
  
  NOTE_G5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, //25
  NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
  NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
  NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
  NOTE_AS5, 16, NOTE_D5, 16, NOTE_D6, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,

  NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
  NOTE_A5, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16, NOTE_C6, 16, NOTE_D5, 16, NOTE_AS5, 16, NOTE_D5, 16,
  NOTE_A5, 16, NOTE_D5, 16, NOTE_F5, 16, NOTE_D5, 16, NOTE_A5, 16, NOTE_D5, 16, NOTE_G5, 16, NOTE_D5, 16,
  NOTE_C6, 16, NOTE_C6, 16, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8,
    REST, 4, NOTE_C6, 16, NOTE_AS5, 16,

  NOTE_C6, -8,  NOTE_F6, -8, NOTE_D6, -4, //35
  NOTE_C6, 8, NOTE_AS5, 8,
  NOTE_C6, 8, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8,
  REST, 4, NOTE_C6, 8, NOTE_D6, 8,
  NOTE_DS6, -8, NOTE_F6, -8,

  NOTE_D6, -8, REST, 16, NOTE_DS6, 8, REST, 8, //40
  NOTE_C6, 8, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8,
  REST, 4, NOTE_C6, 8, NOTE_AS5, 8,
  NOTE_C6, -8,  NOTE_F6, -8, NOTE_D6, -4,
  NOTE_C6, 8, NOTE_AS5, 8,

  NOTE_C6, 8, NOTE_F6, 16, NOTE_D6, 8, REST, 16, REST, 8, //45
  REST, 4, NOTE_C6, 8, NOTE_D6, 8,
  NOTE_DS6, -8, NOTE_F6, -8,
  NOTE_D5, 8, NOTE_FS5, 8, NOTE_F5, 8, NOTE_A5, 8,
  NOTE_A5, -8, NOTE_G5, -4,
};

Game_Sound game_sound_ultra_epic(140, BUZZER_PIN, ultra_epic, sizeof(ultra_epic)/sizeof(ultra_epic[0]));

Game_Sound game_sound_tetris(120, BUZZER_PIN, tetris, sizeof(tetris)/sizeof(tetris[0]));

Game_Sound game_sound_ending(108, BUZZER_PIN, ending_music, sizeof(ending_music)/sizeof(ending_music[0]));

void reset_game()
{
    lcd.clear();
    wall_gen.clear();
    current_change_speed_interval = INITIAL_CHANGE_SPEED_INTERVAL;
    current_wall_interval=INITIAL_CURRENT_WALL_INTERVAL;
    wall_timer.set_interval(current_wall_interval);
    change_speed_timer.set_interval(current_change_speed_interval);
    points=0;

    player.reset();

    game_sound_tetris.reset();
    game_sound_ultra_epic.reset();
    game_sound_ending.reset();

    isLost=false;
}

void ending_scene()
{
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("You lost!");
    lcd.setCursor(0,1);
    player.draw_dead_player();
    lcd.setCursor(4,1);
    lcd.print("Score: "+String(points));
}

void check_collision()
{
    bool isWall = wall_gen.current_wall_postions()[player.get_position()][0] == 1;
    if (isWall)
    {
        ending_scene();
        isLost=true;
    }
}

void move_up()
{
    player.move_up();
}

void move_down()
{
    player.move_down();
}

void redraw_walls()
{
    uint8_t **wall = wall_gen.generate_next();
    wall_drawer.draw(wall, player.get_position());
    check_collision();
}

void increase_speed()
{
    current_wall_interval= current_wall_interval> 100? current_wall_interval-100: current_wall_interval;
    current_change_speed_interval= current_change_speed_interval> 1000? current_change_speed_interval-1000:current_change_speed_interval;
    change_speed_timer.set_interval(current_change_speed_interval);
    wall_timer.set_interval(current_wall_interval);
}

void add_point()
{
    points++;
}

void setup()
{
    // Serial.begin(9600);
    
    lcd.begin(NUMBER_OF_COLLUMNS,NUMBER_OF_ROWS);

    move_button._init_();
    move_button.on_press(move_up);
    move_button.on_release(move_down);

    reset_button._init_();
    reset_button.on_press(reset_game);

    wall_drawer._init_();
    player._init_();

    wall_timer.on_times_up(redraw_walls);
    wall_timer.start();

    change_speed_timer.on_times_up(increase_speed);
    change_speed_timer.start();
    
    counter.on_times_up(add_point);
    counter.start();

    game_sound_tetris._init_();
    game_sound_tetris.start();

    game_sound_ultra_epic._init_();

    game_sound_ending._init_();
}


bool clear_sound=true;
void loop()
{
    reset_button.read();
    if (!isLost)
    {
        if (points<11)
        {
            game_sound_tetris.check();
        }
        else if (points==11)
        {
            game_sound_ultra_epic.start();
        }
        else if(points>11)
        {
            game_sound_ultra_epic.check();
        }
        move_button.read();
        wall_timer.check();
        change_speed_timer.check();
        counter.check();
    }
    else if (isLost)
    {
        if (clear_sound){
            noTone(BUZZER_PIN);
            clear_sound=false;
            game_sound_ending.start();
        }
        game_sound_ending.check();
        
    }
}
