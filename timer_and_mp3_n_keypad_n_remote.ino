// TIMER LIBRARIES
#include "SevenSegmentTM1637.h"

// SOUND LIBRARIES
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <JQ6500_Serial.h>

// LCD LIBRARIES
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// KEYPAD LIBRARIES
#include <Keypad.h>



// CONSTRUCTORS AND GLOBALS
//
//
// GLOBALS
int debug = 1;

//RX(10 Pin of Module),TX(9 Pin of Module)
JQ6500_Serial mp3(A2, A3);

//LCD Initiation
LiquidCrystal_I2C lcd(0x27, 20, 4);

// TIMER CONSTRUCT
const byte PIN_CLK = A1;   // define CLK pin (any digital pin)
const byte PIN_DIO = A0;   // define DIO pin (any digital pin)
SevenSegmentTM1637    display(PIN_CLK, PIN_DIO);

// KEYPAD CONSTRUCT
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );




void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Timer
  display.begin();            // initializes the display
  display.setBacklight(100);  // set the brightness to 100 %
  display.clear();

  //remote
  
  //128 1
  //129 2
  //130 3
  //131 4
  //148 mute
  //149 power

  
  // MP3
  mp3.begin(9600);
  mp3.reset();
  mp3.setVolume(20);
  mp3.setLoopMode(MP3_LOOP_NONE);


  // LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  // Remote
  pinMode(3, INPUT);
  digitalWrite(3, LOW);

  // FLASH 
  pinMode(0, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:


  //timer assign
  int ref, a, b, temp = 0;
  String disp;
  String password = "";
  char password_c[6] = "\0";
  int count = 0;
  if (debug = 1) {
    Serial.println("Code Initiated");
  }

  // LCD INITIALIZATON
  lcd.clear();
  line2(6, "C.R.E.W");
  line3(3, "Detonator Pro!");
  line4(7, "V 1.1");

  // SOUND INITIALIZATION
  if (debug = 1) {
    Serial.println("Sound Initiated");
  }
  mp3.playFileByIndexNumber(1);
  delay(4000);



  // TIMER CHECK
  display.print("CHEC");
  delay(2000);
  display.clear();
  if (debug = 1) {
    Serial.println("Timer Initiated");
  }

  lcd.clear();
  line1(7, "C.R.E.W");
  line2(4, "Enter Options");
  line3(5, "Test - Mute");
  line4(5, "Game - Power");

    if (debug = 1) {
    Serial.println("Remote Initiated");
  }

while(1)
{
  switch (remote())
  {
    case 149: {
        lcd.clear();
        Serial.print("Game initiated");
        line1(1, "Choose Duration");
        line2(6, "45 mins");
        line3(6, "30 mins");
        line4(6, "15 mins");
        while (1)
        {
          switch (remote())
          {
            case 128: {
                ref = 2700;
              } break;
            case 129: {
                ref = 1800;
              } break;
            case 130: {
                ref = 900;
              } break;
            case 131: {
                ref = 10;
              } break;
            default : break;
          }
          if(ref>0) break;
        }
      } break;
    case 148: {Serial.println("test");} break;
    default : break;
  }
  if(ref>0) break;
}


  lcd.clear();
  line2(2, "Enter Password :");
  line3(7, password_c);
  line4(1, "Time is of Essence");

  if (debug = 1) {
    Serial.print("password =  ");
    Serial.print(password_c);
    Serial.print("   ");
    Serial.println(password);
  }

  while (1)
  {

    a = ( millis() - 10000 ) / 1000;
    b = ref - a;

    temp = b / 60;        // min
    disp = temp;
    if (b < 1) boom();
    if (temp <= 9)
      disp = 0 + disp;


    temp = b % 60;        //Second
    if (temp <= 9)
      disp = disp + 0;
    disp = disp + temp;
    display.setColonOn(1);
    display.print(disp);

    char key;
    key = keypad.getKey();
    if (key != NO_KEY)
    {
      password_c[count] = key;
      password_c[count + 1] = '\0';
      password += key;
      count++;
      line3(4, "           ");
      line3(7, password_c);
    }
    

    if (debug = 1) {
      Serial.print("password =  ");
      Serial.print(password_c);
      Serial.print("   ");
      Serial.println(password);
    }

    if (count == 4)
    {
      if (password == "1234") {
        success();
      }
      else {
        count = 0;
        memset(password_c, 0, sizeof(password_c));
        password = "\0";
        ref -= 120;
        line3(5, "INCORRECT");
      }
    }
    if (disp == "0000")
    {
      boom();
    }

    //      mp3.playFileByIndexNumber(2);
    //      delay(1);
  }
}

void boom()
{
  display.clear();
  display.setColonOn(1);
  display.print("0000");
  mp3.playFileByIndexNumber(3);
  display.blink();
  lcd.clear();
  line2(8, "Y O U");
  line3(5, "F A I L E D");
  for(int i = 1; i < 10; i++)
  {
  digitalWrite(0, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(50);                       // wait for a second
  digitalWrite(0, LOW);    // turn the LED off by making the voltage LOW
  delay(50); 
  }// wait for a second
  delay(3000);
  while (1);
}

void success()
{
  display.blink();
  lcd.clear();
  line1(1, "Congratulations !!!");
  line2(1, "Your clue to escape");
  line3(6, "is helll");
  //line4(10, "CREW Labz");
  mp3.playFileByIndexNumber(4);
  delay(12000);
  while (1);
}

void line1(int offset, char str[ ])
{
  lcd.setCursor(offset, 0);
  lcd.print(str);
}

void line2(int offset, char str[ ])
{
  lcd.setCursor(offset, 1);
  lcd.print(str);
}

void line3(int offset, char str[ ])
{
  lcd.setCursor(offset, 2);
  lcd.print(str);
}

void line4(int offset, char str[ ])
{
  lcd.setCursor(offset, 3);
  lcd.print(str);
}

int remote()
{
  int flag = 0, time[12], val = 0, i;
  flag = 0; val = 0;
  if (pulseIn(3, LOW) > 2000)
    flag = 1;
  for (i = 0; (i < 11 && flag == 1); i++)
  {
    time[i] = pulseIn(3, LOW);
    if (time[i] > 1000)
    {
      val = val + (1 << i);
    }
  }
  delay(90);
  return val;
}

