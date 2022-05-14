#include <LiquidCrystal.h>

#define BUTTON_LEFT 7
#define BUTTON_RIGHT 9
#define PLAYER_LCD 0
#define CLEAR_CHUNK_LCD 1
#define WALL_LCD 2
#define DEAD_PLAYER 3

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd (rs,en,d4,d5,d6,d7);

uint8_t player_state_left = HIGH;
uint8_t player_previous_state_left = HIGH;
uint8_t reading_button_state_left=HIGH;


uint8_t player_state_right = HIGH;
uint8_t player_previous_state_right = HIGH;
uint8_t reading_button_state_right=HIGH;


int8_t player_position = 0;
uint8_t player_position_previous = 0;

uint8_t wall_delay = 200; // tempo of wall speed
uint32_t last_wall_time=0; 
unsigned long currentMilis = 0;

bool check_if_clear_chunk = true;

byte main_player[8]=
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

byte end_player[8]=
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



byte clear_chunk[8]=
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

byte wall_lcd [8]=
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



uint8_t wall_lenghts[10]={3,2,2,1,3,1,3,2,1,3};
uint8_t wall_intervals[10]={1,2,1,3,1,3,1,2,1,1};
uint8_t wall_positons[10]={0,1,1,0,0,1,0,0,0,1};

int8_t pointer=16;  // 0-15

uint8_t table_of_wall_pointer=0;
uint8_t lenght_of_nearest_wall = 1;

bool check_if_collision=false;


void ending_scene()
{
    lcd.clear();
    lcd.setCursor(4,0);
    lcd.print("You lost!");
    lcd.setCursor(6,1);
    lcd.write(byte(DEAD_PLAYER));
    lcd.setCursor(7,1);
    lcd.setCursor(0,player_position);
    lcd.write(byte(CLEAR_CHUNK_LCD));
}

void wall_movement(uint8_t position_pointer_of_wall, uint8_t row, uint8_t lenght)
{
    lcd.setCursor(position_pointer_of_wall,row);
    lcd.write(byte(WALL_LCD));

    lcd.setCursor(position_pointer_of_wall+lenght,row);
    lcd.write(byte(CLEAR_CHUNK_LCD));

}

void print_player_positon(int position,bool clear ,  int previous_pos, int row) 
{
    lcd.setCursor(0, previous_pos); // czyszczenie poprzedniej pozycji
    lcd.write(byte(CLEAR_CHUNK_LCD));

    lcd.setCursor(0,position); // wiersz , kolumna
    lcd.write(byte(PLAYER_LCD)); // wpisywanie nowej pozycji
}


void player_movement(int8_t change_pos)
{
        player_position=player_position+change_pos;
        // lcd.setCursor(10,0);
        // lcd.print(String(player_position));

        if (player_position<0)
        {
            player_position=0;
            player_position_previous=0;
            check_if_clear_chunk=false;
        }
        else if (player_position>1)
        {
            player_position=1;
            player_position_previous=1;
            check_if_clear_chunk=false;
        }
        print_player_positon(player_position,check_if_clear_chunk, player_position_previous, 1);
        player_position_previous=player_position;
        check_if_clear_chunk=true;
}

void setup()
{
    Serial.begin(9600);
    lcd.begin(16,2);
    lcd.createChar(PLAYER_LCD,main_player);
    lcd.createChar(CLEAR_CHUNK_LCD, clear_chunk);
    lcd.createChar(WALL_LCD, wall_lcd);
    lcd.createChar(DEAD_PLAYER, end_player);

    pinMode(BUTTON_LEFT, INPUT_PULLUP);
    pinMode(BUTTON_RIGHT, INPUT_PULLUP);

    lcd.setCursor(0,0);
    lcd.write(byte(PLAYER_LCD));
}


uint8_t lenght_of_next_wall =1;
uint8_t currentInterval=0;

void loop()
{
    currentMilis=millis();

    reading_button_state_left = digitalRead(BUTTON_LEFT); // 1 (HIGH) -> brak ruchu, 0 (LOW) -> ruch 
    reading_button_state_right = digitalRead(BUTTON_RIGHT);

// ------------------RUCH SCIAN---------------------------------
    if (currentMilis-last_wall_time>=wall_delay)
    {

        pointer-=1;
        if (pointer<0)
        {
            pointer=15;
            check_if_collision=true;

            lenght_of_nearest_wall=wall_lenghts[(table_of_wall_pointer)%10];   //wall_lenghts[table_of_wall_pointer];
            lenght_of_next_wall=wall_lenghts[(table_of_wall_pointer+1)%10];
        }

        if (check_if_collision)
        {
            if(player_position==wall_positons[table_of_wall_pointer]) // kolizja gracza ze sciana
            {
                ending_scene();
                exit(0);
            }

            lenght_of_nearest_wall-=1; 
            lcd.setCursor(lenght_of_nearest_wall,wall_positons[table_of_wall_pointer] ); // usuwanie sciany obok gracza
            lcd.write(byte(CLEAR_CHUNK_LCD));


            if (lenght_of_nearest_wall<=0)
            {

                check_if_collision=false;
                table_of_wall_pointer+=1;
                lenght_of_nearest_wall=wall_lenghts[table_of_wall_pointer];
                if (table_of_wall_pointer>10)
                    table_of_wall_pointer=0;
            
            }
        }




        // lcd.setCursor(5,0);
        // lcd.print(String(pointer));
        // Serial.println(String(currentMilis));
        if (!check_if_collision)
        {
            wall_movement(pointer, wall_positons[table_of_wall_pointer],wall_lenghts[table_of_wall_pointer]);
        } // wall_lenghts[table_of_wall_pointer]
        last_wall_time=(uint32_t)currentMilis;

        if (15-currentInterval==0)
        {
            
        }
        Serial.println("pos_pointer = "+String(pointer)+"  , lenght_of_nearest_wall = "+String(lenght_of_nearest_wall));

        lcd.setCursor(13,0);
        lcd.print(String(currentMilis/1000));
    }

    // --------------------RUCH GRACZA --------------------------------
        if (reading_button_state_left!=player_state_left)
        {
            player_state_left=reading_button_state_left;
            if (player_state_left==LOW)
                player_movement(1);
        }

        else if (reading_button_state_right!=player_state_right){
            player_state_right=reading_button_state_right;
            if (player_state_right==LOW)
                player_movement(-1);
        }
}