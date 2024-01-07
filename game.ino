/* Include the LedControl library */
// #include <LedControl.h>
#include "LedControl.h"

// Object for controlling the LED matrix.
LedControl lc = LedControl(12, 11, 10, 2); // (dataPin, clockPin, csPin, totalDevices)

// Arrays representing the current state of each column on the LED matrix.
int lc0[] = {0, 0, 0, 0, 0, 0, 0, 0};
int lc1[] = {0, 0, 0, 0, 0, 0, 0, 0};

// Arrays representing the active (moving) and stationary parts of the Tetris game on the screen.
long active[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
long screen[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

// Temporary storage for column data.
int tmpCol = 0;

// self
unsigned long lastMoveTime = 0;
unsigned long lastDownMoveTime = 0;
unsigned long downwardDelay = 300;
const unsigned long debounceDelay = 60;
const unsigned long turnDelay = 160;
unsigned long lastDownMoveTime1 = 0;
const unsigned long downwardDelay1 = 40;

bool checkii = true;
bool matrixFiguraBool = false;
int ownFigureArray[4][4] = {{0,0,0,0}, {0,0,0,0}, {0,0,0,0}, {0,0,0,0}};

// Track the last time the tiles moved and the last button action.
unsigned long previousMoveMillis = 0; // Tracks the last time the tiles were moved
// Time intervals for moving tiles and button action responsiveness.
const long moveInterval = 500; // Interval for moving tiles down (in milliseconds)

unsigned long previousButtonMillis = 0; // Last time a button action was taken
const long buttonInterval = 100;        // Interval for button action responsiveness (milliseconds)

// Current and next Tetris figure.
int figura = 0;
int figuraNext = 0;

// Variables indicating the distance from the left and right edges.
int fromLeft = 0;
int fromRight = 0;

// Current rotation angle of the active Tetris piece.
int angle = 0;

// Check variables for column states and movement.
int colCheck = 0;
int moveCheck = 0;

// Game state variables like score, lines completed, delays, and game level.
int score = 0;      // Current score of player
int started = 0;    // boolean flag for started (1) not started(0)
int lcRows = 16;    // Stands for "LED Control Rows". This variable likely represents the number of rows in the LED matrix or matrices being used for the display.
int lcCols = 8;     // Stands for "LED Control Columns". This variable represents the number of columns in the LED matrix or matrices.
int allLines = 0;   // Tracks the total number of lines cleared by the player throughout the game. This could be used for scoring, leveling up, or other game progression mechanics.
int currLines = 0;  // Represents the number of lines currently cleared in a single move or action. For instance, if a player clears two lines simultaneously, currLines would be 2.
int brickDelay = 0; // This is a delay variable, possibly used to control the speed at which the Tetris pieces fall. A lower value would make the game faster.
int defDelay = 200; // Likely stands for "default delay". This could be the initial or base delay for the falling Tetris pieces, which might be adjusted as the game progresses (e.g., pieces falling faster at higher levels).
int level = 0;      // Indicates the current level of the game. As the player clears more lines (allLines), they could advance to higher levels, typically making the game more challenging.

// Boolean for sound state.
boolean sound = true;

// Pinos

// Pin numbers for various buttons and speaker.
int joystickX = A0;
int joystickY = A1;
int threshold = 150;
// bool finn = true;

int rotate_button = 2;
int left_button = 3;
int down_button = 4;
int right_button = 5;
int start_button = 6;
int speaker_pin = 0;
// mute button pin
int sound_button = 19;

int reihe[]={7,8,9,13};
int spalte[]={5,4,3,2};
int col_scan;


// Byte arrays representing different Tetris pieces.
byte X[8] =
    {
        0b00000,
        0b10001,
        0b01010,
        0b00100,
        0b01010,
        0b10001,
        0b00000,
        0b00000};

byte O[8] =
    {
        0b00000,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b11111,
        0b00000,
        0b00000};

byte L[8] =
    {
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11000,
        0b11111,
        0b11111,
        0b00000};

byte J[8] =
    {
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b00011,
        0b11111,
        0b11111,
        0b00000};

byte T[8] =
    {
        0b00000,
        0b00000,
        0b11111,
        0b11111,
        0b01110,
        0b01110,
        0b00000,
        0b00000};

byte I[8] =
    {
        0b01100,
        0b01100,
        0b01100,
        0b01100,
        0b01100,
        0b01100,
        0b01100,
        0b00000};

byte Z[8] =
    {
        0b00000,
        0b00000,
        0b11110,
        0b11110,
        0b01111,
        0b01111,
        0b00000,
        0b00000};

byte S[8] =
    {
        0b00000,
        0b00000,
        0b01111,
        0b01111,
        0b11110,
        0b11110,
        0b00000,
        0b00000};

// Nuty

// Music notes, their beats, and tempo for game sounds.
int length = 99;
char notes[] = "EbCDCbaaCEDCbbCDECaa DFAGFEECEDCbbCDECaa EbCDCbaaCEDCbbCDECaa DFAGFEECEDCbbCDECaa ECDbCab ECDbCEAJ ";
int beats[] = // Som
    {
        2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 2,
        2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 1,
        2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 2,
        2, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 2, 2, 2, 4, 1,
        5, 5, 5, 5, 5, 5, 7, 2, 5, 5, 5, 5, 2, 2, 5, 5, 3};
int tempo = 128; // Tempo

// Plays a tone through the speaker for a given duration.
void playTone(int tone, int duration)
{
    for (long i = 0; i < duration * 1000L; i += tone * 2)
    {
        digitalWrite(speaker_pin, HIGH);
        delayMicroseconds(tone);
        digitalWrite(speaker_pin, LOW);
        delayMicroseconds(tone);
    }
}

// Translates a note character into a specific tone and plays it.
void playNote(char note, int duration)
{
    char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'F', 'G', 'J', 'A', 'B'};
    int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956, 850, 760, 716, 637, 603, 568, 507};

    for (int i = 0; i < 14; i++)
    {
        if (names[i] == note)
        {
            playTone(tones[i], duration);
        }
    }
}

// Updates the LED matrix for a given column.
void updateColumn(int colnum)
{
    lc0[colnum] = active[colnum] >> (lcRows / 2);
    lc1[colnum] = active[colnum];
    lc.setColumn(0, colnum, (screen[colnum] >> (lcRows / 2)) | lc0[colnum]);
    lc.setColumn(1, colnum, screen[colnum] | lc1[colnum]);
}

// Introduces a delay after a button press.
void buttonDelay(int bdelay)
{
    if (brickDelay > bdelay)
    {
        brickDelay -= bdelay;
    }
    delay(bdelay);
}

// Displays the initial splash screen.
void splashScreen()
{
    int up[] =
        {
            B11101110, //   o o o   o o o
            B01001000, //     o     o
            B01001100, //     o     o o
            B01001000, //     o     o
            B01001110, //     o     o o o
            B00000000, //
            B11101110, //   o o o   o o o
            B01001010  //     o     o   o
        };

    int down[] =
        {
            B01001100, //     o     o o
            B01001010, //     o     o   o
            B01001001, //     o     o     o
            B00000000, //
            B01000111, //     o       o o o
            B01000100, //     o       o
            B01000010, //     o         o
            B01001110  //     o     o o o
        };

    for (int rownum = 0; rownum < 8; rownum++)
    {
        lc.setRow(0, rownum, up[rownum]);
        lc.setRow(1, rownum, down[rownum]);
    }
}

void knopfDruck(int i,int j){
  ownFigureArray[i][j] = 1;
  Serial.println(ownFigureArray[i][j]);

  if(i==0&&j==0){ //Knopf S1 gedrückt
    Serial.println("S1");
  }
  if(i==0&&j==1) //Knopf S2 gedrückt
    Serial.println("S2");
  if(i==0&&j==2) //Knopf S3 gedrückt
    Serial.println("S3");
  if(i==0&&j==3) //Knopf S4 gedrückt
    Serial.println("S4");
  if(i==1&&j==0) //Knopf S5 gedrückt
    Serial.println("S5");
  if(i==1&&j==1) //Knopf S6 gedrückt
    Serial.println("S6");
  if(i==1&&j==2) //Knopf S7 gedrückt
    Serial.println("S7");
  if(i==1&&j==3) //Knopf S8 gedrückt
    Serial.println("S8");
  if(i==2&&j==0) //Knopf S9 gedrückt
    Serial.println("S9");
  if(i==2&&j==1) //Knopf S10 gedrückt
    Serial.println("S10");
  if(i==2&&j==2) //Knopf S11 gedrückt
    Serial.println("S11");
  if(i==2&&j==3) //Knopf S12 gedrückt
    Serial.println("S12");
  if(i==3&&j==0) //Knopf S13 gedrückt
    Serial.println("S13");
  if(i==3&&j==1) //Knopf S14 gedrückt
    Serial.println("S14");
  if(i==3&&j==2) //Knopf S15 gedrückt
    Serial.println("S15");
  if(i==3&&j==3) //Knopf S16 gedrückt
    Serial.println("S16");
}


// Initializes the game, setting up pins, intensity, and display.
void setup()
{

    pinMode(speaker_pin, OUTPUT);
    pinMode(rotate_button, INPUT_PULLUP);
    pinMode(down_button, INPUT_PULLUP);
    pinMode(right_button, INPUT_PULLUP);
    pinMode(left_button, INPUT_PULLUP);
    pinMode(start_button, INPUT_PULLUP);
    //pinMode(sound_button, INPUT_PULLUP);

    //Serial.begin(9600);
    for(int i=0;i<=3;i++){
        //Initialisierung der PINs
        pinMode(reihe[i],OUTPUT);
        pinMode(spalte[i],INPUT);
        digitalWrite(spalte[i],HIGH);
    } 


    lc.shutdown(0, false);
    lc.shutdown(1, false);

    lc.setIntensity(0, 5); // 0,5
    lc.setIntensity(1, 5); // 1,5

    lc.clearDisplay(0);
    lc.clearDisplay(1);
    Serial.begin(9600);

    randomSeed(analogRead(5));
}

// The main game loop, handling game logic, movements, rotations, and game over conditions.
void loop()
{

    // analogWrite(speaker_pin, 10);

    // ... (code for splash screen and initial game setup)
    if (started == 0)
    {
        splashScreen();

        for (int i = 0; i < length; i++)
        {

            // for muting
            if (digitalRead(sound_button) == LOW)
            {
                sound = !sound;
                delay(300);
            }

            if (digitalRead(start_button) == LOW)
            {
                started = 1;
                break;
            }
            if (notes[i] == ' ')
            {
                delay(beats[i] * tempo); // Pausa
            }
            else
            {
                if (sound == 1)
                {
                    playNote(notes[i], beats[i] * tempo);
                }
                else
                {
                    digitalWrite(speaker_pin, LOW);
                }
            }

            delay(tempo / 2);
        }
    }

    // ... (code for the main game loop)
    else
    {
        figura = 0;
        checkii = true;
        matrixFiguraBool = false;
        for(int i=0; i<4; i++){
          for(int j=0; j<4; j++){
            ownFigureArray[i][j] = 0;
          }
        } 
        score = 0;
        allLines = 0;
        lc.clearDisplay(0);
        lc.clearDisplay(1);
        memset(lc0, 0, sizeof(lc0));
        memset(lc1, 0, sizeof(lc1));
        memset(active, 0, sizeof(active));
        memset(screen, 0, sizeof(screen));
        tmpCol = 0;
        Serial.println("Begin");

        while (started == 1)
        {

            // Level setting based on "allLines"
            // sets game level based on number of lines cleared("allLines")
            if (score < 1)
            {
                level = 0; // Level 0
            }
            else if (score < 8)
            {
                level = 1; // Level 1
            }
            else if (score < 20)
            {
                level = 2; // Level 2
            }
            else if (score < 50)
            {
                level = 3; // Level 3
            }
            else if (score < 80)
            {
                level = 4; // Level 4
            }
            else
            {
                level = 5; // Level 5
            }

            downwardDelay = (5 - level) * 75;
            defDelay = (5 - level) * 100;
            brickDelay = defDelay;

            // Randomly selects a new Tetris figure if the current figure (figura) is 0.

            while(matrixFiguraBool){
                for(int i=0;i<=3;i++) {
                  digitalWrite(reihe[0],HIGH);
                  digitalWrite(reihe[1],HIGH);
                  digitalWrite(reihe[2],HIGH);
                  digitalWrite(reihe[3],HIGH);
                  digitalWrite(reihe[i],LOW);

                  for(int j=0;j<=3;j++){
                    col_scan=digitalRead(spalte[j]);
                    if(col_scan==LOW){
                        //Wenn gedrückter Knopf erkannt
                        //führe knopfDruck aus
                        knopfDruck(i,j);
                        delay(300); //war 300
                    }
                  }
                }

                if(digitalRead(start_button) == LOW)
                {
                    figura = 8;
                    matrixFiguraBool = false;
                }

            }

            if (figura == 0 || figura == 8)
            {
                if(figura == 0){
                    figura = random(1, 8);
                }
                
                
                // figura = 4;
                Serial.print("fig:");
                Serial.println(figura);
                lastDownMoveTime1 = millis(); // Update the last move time
                switch (figura)
                {
                case 1:
                    //"O"
                    active[3] = 131072 + 65536;
                    active[4] = 131072 + 65536;
                    //active[2] = 131072 + 65536;
                    //active[3] = 131072 + 65536;
                    //active[4] = 131072 + 65536;
                    //active[5] = 131072 + 65536;
                    fromLeft = 3;
                    fromRight = 3;
                    Serial.println("ICH1");
                    break;

                case 2:
                    //"L"
                    active[3] = 262144 + 131072 + 65536;
                    active[4] = 65536;
                    fromLeft = 3;
                    fromRight = 3;
                    Serial.println("ICH2");

                    break;

                case 3:
                    //"J"
                    active[3] = 65536;
                    active[4] = 262144 + 131072 + 65536;
                    fromLeft = 3;
                    fromRight = 3;
                    Serial.println("ICH3");

                    break;

                case 4:
                    //"T"
                    active[2] = 131072;
                    active[3] = 131072 + 65536;
                    active[4] = 131072;
                    fromLeft = 2;
                    fromRight = 3;
                    Serial.println("ICH4");

                    break;

                case 5:
                    //"I"
                    active[3] = 524288 + 262144 + 131072 + 65536;
                    fromLeft = 3;
                    fromRight = 4;
                    Serial.println("ICH5");

                    break;

                case 6:
                    //"Z"
                    active[2] = 131072;
                    active[3] = 131072 + 65536;
                    active[4] = 65536;
                    fromLeft = 2;
                    fromRight = 3;
                    Serial.println("ICH6");

                    break;

                case 7:
                    //"S"
                    active[2] = 65536;
                    active[3] = 131072 + 65536;
                    active[4] = 131072;
                    fromLeft = 2;
                    fromRight = 3;
                    Serial.println("ICH7");

                    break;
                

                case 8:
                    //own figure

                    int j0=0, j1=0, j2=0, j3=0;
                    for(int i=0; i<4; i++){
                      for(int j=0; j<4; j++){
                          if(ownFigureArray[i][j] == 1){
                            if(j == 0) j0=1;
                            if(j == 1) j1=1;
                            if(j == 2) j2=1;  
                            if(j == 3) j3=1;

                            switch (i) {
                              case 0:
                                active[j+2] += 524288;
                                break;
                              case 1:
                                active[j+2] += 262144;
                                break;
                              case 2:
                                active[j+2] += 131072;
                                break;
                              case 3:
                                active[j+2] += 65536;
                                break;
                            }

                          }
                      }
                    }

                    if(j3 == 1){
                      fromLeft = 5;
                    }
                    if(j2 == 1){
                      fromLeft = 4;
                    }
                    if(j1 == 1){
                      fromLeft = 3;
                    }
                    if(j0 == 1){
                      fromLeft = 2;
                    }
                    if(j0 == 1){
                      fromRight = 5;
                    }
                    if(j1 == 1){
                      fromRight = 4;
                    }
                    if(j2 == 1){
                      fromRight = 3;
                    }
                    if(j3 == 1){
                      fromRight = 2;
                    }

                    /*active[2] = 524288 + 262144 + 131072 + 65536;
                    active[3] =                   131072 + 65536;
                    active[4] =                   131072 + 65536;
                    active[5] =                   131072 + 65536;
                    */
                    //fromLeft = 2;
                    //fromRight = 2;
                    Serial.println("ICH8");
                    break;
                }

                // delay(1000);
            }
            angle = 0;

            // Non-blocking delay for button actions
            // if (currentMillis - previousButtonMillis >= buttonInterval) {
            // previousButtonMillis = currentMillis;
            // Logic for handling button presses
            // ... (your code for button handling)

            int krok = 0;
            while (krok < lcRows + 1)
            {
                colCheck = 0;

                int xValue = analogRead(joystickX);
                int yValue = analogRead(joystickY);

                for (int i = 0; i < (lcCols / 2); i++)
                {
                    // if((digitalRead(left_button) == LOW) && (fromLeft > 0))

                    if ((xValue < (512 - threshold)) && (fromLeft > 0))

                    {
                        if (millis() - lastMoveTime > debounceDelay)
                        {

                            moveCheck = 0;
                            for (int colnum = fromLeft; colnum < (lcCols - fromRight); colnum++)
                            {
                                if ((active[colnum] & screen[colnum - 1]) == 0)
                                {
                                    moveCheck++;
                                }
                            }

                            if (moveCheck == (lcCols - fromLeft - fromRight))
                            {
                                for (int colnum = (fromLeft - 1); colnum < (lcCols - fromRight); colnum++)
                                {
                                    if (colnum < (lcCols - 1))
                                    {
                                        active[colnum] = active[colnum + 1];
                                    }
                                    else
                                    {
                                        active[colnum] = 0;
                                    }
                                    updateColumn(colnum);
                                }
                                fromLeft--;
                                fromRight++;
                                playNote('E', 10);
                                // buttonDelay(10);
                                lastMoveTime = millis();
                            }
                        }
                    }
                }

                for (int i = 0; i < (lcCols / 2); i++)
                {
                    // if((digitalRead(right_button) == LOW) && (fromRight > 0))
                    if ((xValue > (512 + threshold)) && (fromRight > 0))

                    {
                        if (millis() - lastMoveTime > debounceDelay)
                        {
                            moveCheck = 0;
                            for (int colnum = fromLeft; colnum < (lcCols - fromRight); colnum++)
                            {
                                if ((active[colnum] & screen[colnum + 1]) == 0)
                                {
                                    moveCheck++;
                                }
                            }

                            if (moveCheck == (lcCols - fromLeft - fromRight))
                            {
                                for (int colnum = (lcCols - fromRight); colnum > (fromLeft - 1); colnum--)
                                {
                                    if (colnum > 0)
                                    {
                                        active[colnum] = active[colnum - 1];
                                    }
                                    else
                                    {
                                        active[colnum] = 0;
                                    }
                                    updateColumn(colnum);
                                }
                                fromLeft++;
                                fromRight--;
                                playNote('E', 10);
                                // buttonDelay(10);
                                lastMoveTime = millis();
                            }
                        }
                    }
                }

                // if(digitalRead(down_button) == LOW)
                if (yValue > (512 + threshold))
                {
                    if (millis() - lastMoveTime > debounceDelay)
                    {
                        checkii = true;
                        brickDelay = 0;
                        playNote('b', 10);
                        lastMoveTime = millis();
                    }
                }
                else
                {
                    brickDelay = defDelay;
                }

                for (int i = 0; i < (lcCols / 2); i++)
                {
                    // if(digitalRead(rotate_button) == LOW)
                    if (yValue < (512 - threshold))
                    {
                        if (millis() - lastMoveTime > turnDelay)
                        {

                            switch (figura)
                            {
                            case 1:
                                //"O"
                                break;
                            case 8:
                                //"own"
                                break;

                            case 2:
                                //"L"
                                switch (angle)
                                {
                                case 0:
                                    // . o .        . . .
                                    // . o .  --->  o o o
                                    // . o o        o . .
                                    if ((fromLeft > 0) && (((active[fromLeft + 1] | (active[fromLeft + 1] << 1)) & screen[fromLeft - 1]) == 0))
                                    {
                                        active[fromLeft - 1] = (active[fromLeft + 1] | (active[fromLeft + 1] << 1));
                                        updateColumn(fromLeft - 1);
                                        active[fromLeft] = (active[fromLeft + 1] << 1);
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft + 1] << 1);
                                        updateColumn(fromLeft + 1);
                                        fromLeft--;
                                        angle = 1;
                                    }
                                    break;

                                case 1:
                                    // . . .        o o .
                                    // o o o  --->  . o .
                                    // o . .        . o .
                                    if ((((active[fromLeft + 2] << 1) & screen[fromLeft]) == 0) && ((((active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1)) & screen[fromLeft + 1]) == 0))
                                    {
                                        active[fromLeft] = (active[fromLeft + 2] << 1);
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = active[fromLeft + 1] | (active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1);
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = 0;
                                        updateColumn(fromLeft + 2);
                                        fromRight++;
                                        angle = 2;
                                    }
                                    break;

                                case 2:
                                    // o o .        . . o
                                    // . o .  --->  o o o
                                    // . o .        . . .
                                    if ((fromRight > 0) && (((active[fromLeft] >> 1) & screen[fromLeft]) == 0) && ((((active[fromLeft + 1] << 1) & active[fromLeft + 1]) & screen[fromLeft + 1]) == 0))
                                    {
                                        active[fromLeft] = (active[fromLeft] >> 1);
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = active[fromLeft];
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = ((active[fromLeft + 1] << 1) | active[fromLeft + 1]);
                                        updateColumn(fromLeft + 2);
                                        fromRight--;
                                        krok--;
                                        angle = 3;
                                    }
                                    break;

                                case 3:
                                    // . . o        . o .
                                    // o o o  --->  . o .
                                    // . . .        . o o
                                    if (((((active[fromLeft] << 1) | (active[fromLeft] >> 1)) & screen[fromLeft + 1]) == 0) && (((active[fromLeft] >> 1) & screen[fromLeft + 2]) == 0) && (krok < lcRows))
                                    {
                                        active[fromLeft] = 0;
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft + 2] | (active[fromLeft + 2] >> 1));
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = ((active[fromLeft + 2] >> 1) & (active[fromLeft + 2] >> 2));
                                        updateColumn(fromLeft + 2);
                                        fromLeft++;
                                        krok++;
                                        angle = 0;
                                    }
                                    break;
                                }
                                break;

                            case 3:
                                //"J"
                                switch (angle)
                                {
                                case 0:
                                    // . o .        o . .
                                    // . o .  --->  o o o
                                    // o o .        . . .
                                    if ((fromRight > 0) && ((((active[fromLeft] << 2) | (active[fromLeft] << 1)) & screen[fromLeft]) == 0) && (((active[fromLeft] << 1) & screen[fromLeft + 2]) == 0))
                                    {
                                        active[fromLeft] = ((active[fromLeft] << 2) | (active[fromLeft] << 1));
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = ((active[fromLeft + 1] << 1) & (active[fromLeft + 1] >> 1));
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = active[fromLeft + 1];
                                        updateColumn(fromLeft + 2);
                                        fromRight--;
                                        krok--;
                                        angle = 1;
                                    }
                                    break;

                                case 1:
                                    // o . .        . o o
                                    // o o o  --->  . o .
                                    // . . .        . o .
                                    if ((krok < lcRows) && ((((active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1)) & screen[fromLeft + 1]) == 0) && (((active[fromLeft + 2] << 1) & screen[fromLeft + 2]) == 0))
                                    {
                                        active[fromLeft] = 0;
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft + 1] | (active[fromLeft + 1] << 1) | (active[fromLeft + 1] >> 1));
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = (active[fromLeft + 2] << 1);
                                        updateColumn(fromLeft + 2);
                                        fromLeft++;
                                        krok++;
                                        angle = 2;
                                    }
                                    break;

                                case 2:
                                    // . o o        . . .
                                    // . o .  --->  o o o
                                    // . o .        . . o
                                    if ((fromLeft > 0) && (((active[fromLeft + 1] >> 1) & screen[fromLeft - 1]) == 0) && ((((active[fromLeft + 1] >> 1) | (active[fromLeft + 1] >> 2)) & screen[fromLeft + 1]) == 0))
                                    {
                                        active[fromLeft - 1] = (active[fromLeft + 1] >> 1);
                                        updateColumn(fromLeft - 1);
                                        active[fromLeft] = active[fromLeft - 1];
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft] | (active[fromLeft + 1] >> 2));
                                        updateColumn(fromLeft + 1);
                                        fromLeft--;
                                        angle = 3;
                                    }
                                    break;

                                case 3:
                                    // . . .        . o .
                                    // o o o  --->  . o .
                                    // . . o        o o .
                                    if ((((active[fromLeft] >> 1) & screen[fromLeft]) == 0) && ((((active[fromLeft] << 1) | (active[fromLeft >> 1])) & screen[fromLeft + 1]) == 0))
                                    {
                                        active[fromLeft] = (active[fromLeft] >> 1);
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = ((active[fromLeft + 1] << 1) | active[fromLeft + 2]);
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = 0;
                                        updateColumn(fromLeft + 2);
                                        fromRight++;
                                        angle = 0;
                                    }
                                    break;
                                }
                                break;

                            case 4:
                                //"T"
                                switch (angle)
                                {
                                case 0:
                                    // . . .        . o .
                                    // o o o  --->  o o .
                                    // . o .        . o .
                                    if (((active[fromLeft + 1] << 1) & screen[fromLeft + 1]) == 0)
                                    {
                                        // active[fromLeft]
                                        active[fromLeft + 1] = active[fromLeft + 1] | (active[fromLeft + 1] << 1);
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = 0;
                                        updateColumn(fromLeft + 2);
                                        fromRight++;
                                        angle = 1;
                                    }
                                    break;

                                case 1:
                                    // . o .        . o .
                                    // o o .  --->  o o o
                                    // . o .        . . .
                                    if ((fromRight > 0) && ((active[fromLeft] & screen[fromLeft + 2]) == 0))
                                    {
                                        // active[fromLeft]
                                        active[fromLeft + 1] = active[fromLeft + 1] & (active[fromLeft + 1] << 1);
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = active[fromLeft];
                                        updateColumn(fromLeft + 2);
                                        fromRight--;
                                        krok--;
                                        angle = 2;
                                    }
                                    break;

                                case 2:
                                    // . o .        . o .
                                    // o o o  --->  . o o
                                    // . . .        . o .
                                    if ((((active[fromLeft + 1] >> 1) & screen[fromLeft + 1]) == 0) && (krok < lcRows))
                                    {
                                        active[fromLeft] = 0;
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = active[fromLeft + 1] | (active[fromLeft + 1] >> 1);
                                        updateColumn(fromLeft + 1);
                                        // active[fromLeft + 2]
                                        fromLeft++;
                                        krok++;
                                        angle = 3;
                                    }
                                    break;

                                case 3:
                                    if ((fromLeft > 0) && ((active[fromLeft + 1] & screen[fromLeft - 1]) == 0))
                                    {
                                        active[fromLeft - 1] = active[fromLeft + 1];
                                        updateColumn(fromLeft - 1);
                                        active[fromLeft] = active[fromLeft] & (active[fromLeft] >> 1);
                                        updateColumn(fromLeft);
                                        fromLeft--;
                                        angle = 0;
                                    }
                                    break;
                                }
                                break;

                            case 5:
                                //"I"
                                switch (angle)
                                {
                                case 0:
                                    // . o . .        . . . .
                                    // . o . .  --->  o o o o
                                    // . o . .        . . . .
                                    // . o . .        . . . .
                                    if ((fromLeft > 0) && (fromRight > 1)

                                        && ((((((active[fromLeft] >> 1) & (active[fromLeft] << 2)) & screen[fromLeft - 1]) & screen[fromLeft + 1]) & screen[fromLeft + 2]) == 0))
                                    {
                                        active[fromLeft - 1] = ((active[fromLeft] >> 1) & (active[fromLeft] << 2));
                                        updateColumn(fromLeft - 1);
                                        active[fromLeft] = active[fromLeft - 1];
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = active[fromLeft];
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = active[fromLeft];
                                        updateColumn(fromLeft + 2);
                                        fromLeft--;
                                        fromRight -= 2;
                                        krok -= 2;
                                        angle = 1;
                                    }
                                    break;

                                case 1:
                                    // . . . .        . . o .
                                    // o o o o  --->  . . o .
                                    // . . . .        . . o .
                                    // . . . .        . . o .
                                    if ((krok < (lcRows - 1)) && (((active[fromLeft] << 1) | (active[fromLeft] >> 1) | (active[fromLeft] >> 2)) & screen[fromLeft + 2]) == 0)
                                    {
                                        active[fromLeft] = 0;
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = 0;
                                        updateColumn(fromLeft + 1);

                                        active[fromLeft + 2] = (active[fromLeft + 2] | (active[fromLeft + 2] << 1) | (active[fromLeft + 2] >> 1) | (active[fromLeft + 2] >> 2));
                                        updateColumn(fromLeft + 2);
                                        active[fromLeft + 3] = 0;
                                        updateColumn(fromLeft + 3);
                                        fromLeft += 2;
                                        fromRight++;
                                        krok += 2;
                                        angle = 2;
                                    }
                                    break;

                                case 2:
                                    // . . o .        . . . .
                                    // . . o .  --->  . . . .
                                    // . . o .        o o o o
                                    // . . o .        . . . .
                                    if ((fromLeft > 1) && (fromRight > 0)

                                        && ((((((active[fromLeft] << 1) & (active[fromLeft] >> 2)) & screen[fromLeft - 2]) & screen[fromLeft - 1]) & screen[fromLeft + 1]) == 0))
                                    {
                                        active[fromLeft - 2] = ((active[fromLeft] << 1) & (active[fromLeft] >> 2));
                                        updateColumn(fromLeft - 2);
                                        active[fromLeft - 1] = active[fromLeft - 2];
                                        updateColumn(fromLeft - 1);
                                        active[fromLeft] = active[fromLeft - 1];
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = active[fromLeft];
                                        updateColumn(fromLeft + 1);
                                        fromLeft -= 2;
                                        fromRight--;
                                        krok--;
                                        angle = 3;
                                    }
                                    break;

                                case 3:
                                    // . . . .        . o . .
                                    // . . . .  --->  . o . .
                                    // o o o o        . o . .
                                    // . . . .        . o . .
                                    if ((krok < (lcRows)) && (((active[fromLeft] >> 1) | (active[fromLeft] << 1) | (active[fromLeft] << 2)) & screen[fromLeft + 1]) == 0)
                                    {
                                        active[fromLeft] = 0;
                                        updateColumn(fromLeft);

                                        active[fromLeft + 1] = (active[fromLeft + 1] | (active[fromLeft + 1] >> 1) | (active[fromLeft + 1] << 1) | (active[fromLeft + 1] << 2));
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = 0;
                                        updateColumn(fromLeft + 2);
                                        active[fromLeft + 3] = 0;
                                        updateColumn(fromLeft + 3);
                                        fromLeft++;
                                        fromRight += 2;
                                        krok++;
                                        angle = 0;
                                    }
                                    break;
                                }
                                break;

                            case 6:
                                //"Z"
                                switch (angle)
                                {
                                case 0:
                                    // . . .        . o .
                                    // o o .  --->  o o .
                                    // . o o        o . .
                                    if (((active[fromLeft + 1] & screen[fromLeft]) == 0) && (((active[fromLeft + 1] << 1) & screen[fromLeft + 1]) == 0))
                                    {
                                        active[fromLeft] = active[fromLeft + 1];
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft + 1] << 1);
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = 0;
                                        updateColumn(fromLeft + 2);
                                        fromRight++;
                                        angle = 1;
                                    }
                                    break;

                                case 1:
                                    // . o .        o o .
                                    // o o .  --->  . o o
                                    // o . .        . . .
                                    if ((fromRight > 0) && ((((active[fromLeft] << 2) & (active[fromLeft] << 1)) & screen[fromLeft]) == 0) && (((active[fromLeft] & active[fromLeft + 1]) & screen[fromLeft + 2]) == 0))
                                    {
                                        active[fromLeft] = ((active[fromLeft] << 2) & (active[fromLeft] << 1));
                                        updateColumn(fromLeft);
                                        // active[fromLeft + 1]
                                        active[fromLeft + 2] = (active[fromLeft] >> 1);
                                        updateColumn(fromLeft + 2);
                                        fromRight--;
                                        krok--;
                                        angle = 2;
                                    }
                                    break;

                                case 2:
                                    // o o .        . . o
                                    // . o o  --->  . o o
                                    // . . .        . o .
                                    if ((krok < lcRows) && (((active[fromLeft + 1] >> 1) & screen[fromLeft + 1]) == 0) && (((active[fromLeft + 2] << 1) & screen[fromLeft + 2]) == 0))
                                    {
                                        active[fromLeft] = 0;
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft + 1] >> 1);
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = (active[fromLeft + 2] | (active[fromLeft + 2] << 1));
                                        updateColumn(fromLeft + 2);
                                        fromLeft++;
                                        krok++;
                                        angle = 3;
                                    }
                                    break;

                                case 3:
                                    // . . o        . . .
                                    // . o o  --->  o o .
                                    // . o .        . o o
                                    if ((fromLeft > 0) && (((active[fromLeft] & active[fromLeft + 1]) & screen[fromLeft - 1]) == 0) && (((active[fromLeft + 1] >> 1) & screen[fromLeft + 1]) == 0))
                                    {
                                        active[fromLeft - 1] = (active[fromLeft] & active[fromLeft + 1]);
                                        updateColumn(fromLeft - 1);
                                        // active[fromLeft]
                                        active[fromLeft + 1] = (active[fromLeft - 1] >> 1);
                                        updateColumn(fromLeft + 1);
                                        fromLeft--;
                                        angle = 0;
                                    }
                                    break;
                                }
                                break;

                            case 7:
                                //"S"
                                switch (angle)
                                {
                                case 0:
                                    // . . .        o . .
                                    // . o o  --->  o o .
                                    // o o .        . o .
                                    if (((active[fromLeft + 1] << 1) & screen[fromLeft]) == 0)
                                    {
                                        active[fromLeft] = (active[fromLeft + 1] << 1);
                                        updateColumn(fromLeft);
                                        // active[fromLeft + 1]
                                        active[fromLeft + 2] = 0;
                                        updateColumn(fromLeft + 2);
                                        fromRight++;
                                        angle = 1;
                                    }
                                    break;

                                case 1:
                                    // o . .        . o o
                                    // o o .  --->  o o .
                                    // . o .        . . .
                                    if ((fromRight > 0) && (((active[fromLeft + 1] << 1) & screen[fromLeft + 1]) == 0) && (((active[fromLeft] & (active[fromLeft] << 1)) & screen[fromLeft + 2]) == 0))
                                    {
                                        active[fromLeft] = (active[fromLeft] & active[fromLeft + 1]);
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft + 1] << 1);
                                        updateColumn(fromLeft + 1);
                                        active[fromLeft + 2] = (active[fromLeft] << 1);
                                        updateColumn(fromLeft + 2);
                                        fromRight--;
                                        krok--;
                                        angle = 2;
                                    }
                                    break;

                                case 2:
                                    // . o o        . o .
                                    // o o .  --->  . o o
                                    // . . .        . . o
                                    if ((krok < lcRows) && (((active[fromLeft + 1] >> 1) & screen[fromLeft + 2]) == 0))
                                    {
                                        active[fromLeft] = 0;
                                        updateColumn(fromLeft);
                                        // active[fromLeft + 1]
                                        active[fromLeft + 2] = (active[fromLeft + 1] >> 1);
                                        updateColumn(fromLeft + 2);
                                        fromLeft++;
                                        krok++;
                                        angle = 3;
                                    }
                                    break;

                                case 3:
                                    // . o .        . . .
                                    // . o o  --->  . o o
                                    // . . o        o o .
                                    if ((fromLeft > 0) && ((active[fromLeft + 1] & ((active[fromLeft + 1] >> 1)) & screen[fromLeft - 1]) == 0) && ((active[fromLeft + 1] & screen[fromLeft]) == 0))
                                    {
                                        active[fromLeft - 1] = (active[fromLeft + 1] & (active[fromLeft + 1] >> 1));
                                        updateColumn(fromLeft - 1);
                                        active[fromLeft] = active[fromLeft + 1];
                                        updateColumn(fromLeft);
                                        active[fromLeft + 1] = (active[fromLeft - 1] << 1);
                                        updateColumn(fromLeft + 1);
                                        fromLeft--;
                                        angle = 0;
                                    }
                                    break;
                                }
                                break;
                            }
                            playNote('E', 10);
                            // buttonDelay(200);
                            lastMoveTime = millis();
                        }
                    }
                }

                // Restart
                /*if(digitalRead(start_button) == LOW)
                {
                    memset(lc0, 0, sizeof(lc0));
                    memset(lc1, 0, sizeof(lc1));
                    memset(active, 0, sizeof(active));
                    memset(screen, 0, sizeof(screen));
                    score = 0;
                    allLines = 0;
                    figura = 0;
                    break;
                }*/

                // ... (collision detection and figure locking code)
                for (int colnum = 0; colnum < lcCols; colnum++)
                {

                    if ((screen[colnum] & (active[colnum] >> 1)) == 0)
                    {
                        colCheck++;
                        //Serial.println("columnCheck++");
                        //Serial.println(colCheck);
                    }
                    else
                    {
                        colCheck = 0;
                        if (krok == 0)
                        {
                            Serial.println("Endedendneendn");
                            started = 0;
                        }
                    }
                }

                // hier downward movement

                if ((colCheck == lcCols) && (krok < lcRows))
                {
                    if (checkii)
                    {
                        Serial.println("test1");
                        for (int colnum = 0; colnum < lcCols; colnum++)
                        {
                            //Serial.println("test2");
                            active[colnum] = active[colnum] >> 1;
                            updateColumn(colnum);
                        }
                        lastDownMoveTime = millis(); // Update the last move time
                        checkii = false;
                        krok++;
                        Serial.println("RealDown");
                    }
                    //Serial.println("inIF");
                }
                else
                {
                    Serial.println("inBREAK");
                    break;
                }
                // hier steht delay für alles
                // delay(brickDelay);
                // lastDownMoveTime = millis();
                if (millis() - lastDownMoveTime > downwardDelay)
                {

                    Serial.println("zeit");
                    checkii = true;
                }
                else
                {
                    // checkii = false;
                    // Serial.println("nope");
                }
                //Serial.println("End");
            }
            figura = 0;
            Serial.println("End11111111");
            //Serial.println(allLines);

            for (int colnum = 0; colnum < lcCols; colnum++)
            {
                screen[colnum] = screen[colnum] | (lc0[colnum] << (lcRows / 2));
                screen[colnum] = screen[colnum] | lc1[colnum];
                lc0[colnum] = 0;
                lc1[colnum] = 0;
                active[colnum] = 0;
            }

            currLines = 0;
            for (int rownum = 0; rownum < lcRows; rownum++)
            {
                colCheck = 0;
                for (int colnum = 0; colnum < lcCols; colnum++)
                {
                    if (((screen[colnum] >> rownum) & 1) == 1)
                    {
                        colCheck++;
                    }
                }
                if (colCheck == lcCols)
                {
                    // Animacja kasowania
                    for (int colnum = 0; colnum < lcCols; colnum++)
                    {
                        tmpCol = ~((int)round(pow(2, rownum)));
                        screen[colnum] = screen[colnum] & tmpCol;
                        updateColumn(colnum);

                        switch (currLines)
                        {
                        case 0:
                            playNote('b', 20);
                            break;
                        case 1:
                            playNote('D', 20);
                            break;
                        case 2:
                            playNote('F', 20);
                            break;
                        case 3:
                            playNote('A', 20);
                            break;
                        }
                        delay(30);

                        tmpCol = (int)(round(pow(2, rownum)) - 1);
                        tmpCol = screen[colnum] & tmpCol;
                        screen[colnum] = (screen[colnum] >> (rownum + 1));
                        screen[colnum] = (screen[colnum] << rownum);
                        screen[colnum] = screen[colnum] | tmpCol;
                    }

                    for (int colnum = 0; colnum < lcCols; colnum++)
                    {
                        updateColumn(colnum);
                    }
                    rownum--;
                    currLines++;
                    allLines++;
                }
            }

            if (currLines > 0)
            {
                //score += (int)round(pow(4, currLines - 1));
                updateScore(currLines*currLines);
                Serial.println(score);
                Serial.println(allLines);

            }
            
            
            /*if(score < 1){
              matrixFiguraBool = false;
            
            }
            */
                  

            
            //}
        }

        gameOver();
        // == Game Over ==
    }
}

void updateScore(int newScore){
    int oldScore = score;
    score += newScore;
    for (int i = oldScore + 1; i <= score; i++) {
        if (i % 5 == 0) {
            for(int k=0; k<4; k++){
              for(int l=0; l<4; l++){
                ownFigureArray[k][l] = 0;
              }
            } 
            matrixFiguraBool = true;
        }
    }
}
// Handles the game over sequence with display and sound.
void gameOver()
{
    playNote('F', 80);
    playNote('A', 60);
    playNote('F', 80);
    playNote('A', 60);

    int cima[] =
        {
            B11111111, //     o     o o o
            B11111111, //   o o o    o
            B11111111, //   o   o     o o
            B11111111, //     o     o
            B11111111, //     o     o o o
            B11111111, //
            B11111111, //   o o o   o o o
            B11111111  //     o     o   o
        };

    int baixo[] =
        {
            B11111111, //     o     o o
            B11111111, //     o     o   o
            B11111111, //     o     o     o
            B11111111, //
            B11111111, //     o       o o o
            B11111111, //     o       o
            B11111111, //     o         o
            B11111111  //     o     o o o
        };

    for (int rownum = 8; rownum >= 0; rownum--)
    {
        lc.setRow(1, rownum, baixo[rownum]);
        delay(100);
    }

    for (int rownum = 8; rownum >= 0; rownum--)
    {
        lc.setRow(0, rownum, cima[rownum]);
        delay(100);
    }

    delay(1800);
}
