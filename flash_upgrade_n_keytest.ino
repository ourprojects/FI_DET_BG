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

  // FLASH
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
  Serial.println("FLASHS");

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
  //463 blue

  // MP3
  mp3.begin(9600);
  mp3.reset();
  mp3.setVolume(30);
  mp3.setLoopMode(MP3_LOOP_NONE);


  // LCD
  lcd.begin();
  lcd.backlight();
  lcd.clear();

  // Remote
  pinMode(3, INPUT);
  digitalWrite(3, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:


  //timer assign
  int ref, a, b, i, temp = 0;
  String disp;
  String password = "";
  char password_c[] = "\0";
  char pw2[] = "\0";
  char pw1[] = "\0";
  char c = '\0';
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
  //  mp3.playFileByIndexNumber(1);
  //  delay(4000);



  // TIMER CHECK
  display.print("CHEC");
  delay(2000);
  display.clear();
  if (debug = 1) {
    Serial.println("Timer Initiated");
  }


  digitalWrite(0, HIGH);
  delay(500);
  digitalWrite(0, LOW);
  if (debug = 1)
    Serial.println("FLASHS");

  if (debug = 1) {
    Serial.println("Remote Initiated");
  }


  lcd.clear();
//  line1(7, "C.R.E.W");
//  line2(4, "Game     - Power");
//  line3(4, "Pwd rst  - Blue");
//  line4(4, "Kpad tst - Red");


  while (1)
  { ref = 0;
    line1(7, "C.R.E.W");
    line2(2, "Game     - Power");
    line3(2, "Pwd rst  - Blue");
    line4(2, "Kpad tst - Red");
    switch (remote())
    {
      case 149: {
          lcd.clear();
          Serial.print("Game initiated");
          line1(1, "Choose Duration");
          line2(1, "1-40 mins");
          line3(1, "2-30 mins");
          line4(1, "3-20 mins");
          line2(11, "4-10 mins");
          while (1)
          {
            switch (remote())
            {
              case 128: {
                  ref = 2400;
                } break;
              case 129: {
                  ref = 1800;
                } break;
              case 130: {
                  ref = 1200;
                } break;
              case 131: {
                  ref = 600;
                } break;
              default : break;
            }
            if (ref > 0) break;
          }
        } break;
      case 463: {
          i = 0;
          Serial.println("enter new password");
          //          delay(2000);
          while (1)
          {
            Serial.println("while");
            if (Serial.available() > 0)
            {
              Serial.println("check");
              //              whi
              c = Serial.read();
              pw1[i] = c;
              i++;

              break;
            }
            delay(500);
          }
          Serial.println(" new password is ");
          Serial.println(pw1);
          //          ref = 60;
        } break;

      case 460: {

          Serial.println("Keypad test");
          lcd.clear();
          line1(1, "Keypad test");
          line2(1, "Press keys 1 by 1");
          char key = '\0';
          int count1 = 0;
          char cc[13] = "\0";
          while (count1 < 12)
          {
            line2(1, "Press keys 1 by 1!");
            key = keypad.getKey();
            if (key != NO_KEY)
            {
              cc[count1] = key;
              // cc[count1 + 1] = '\0';
              count1++;
              line3(4, "            ");
              line3(5, cc);
            }
          }
          lcd.clear();
        } break;
      default : break;
    }
    if (ref > 0) break;
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
  mp3.setLoopMode(MP3_LOOP_ONE);                  //Looping inbuilt funct
  mp3.playFileByIndexNumber(2);                  // play file 2

  double x = millis();
  while (1)
  {

    a = ( millis() - x  ) / 1000;
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
      line3(9, password_c);
    }


    if (debug = 1) {
      Serial.print("password =  ");
      Serial.print(password_c);
      Serial.print("   ");
      Serial.println(password);
    }

    if (count == 4)
    {
      if (password == pw1) {
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
  mp3.setLoopMode(MP3_LOOP_ONE_STOP);
  display.clear();
  display.setColonOn(1);
  display.print("0000");
  display.blink();
  lcd.clear();
  line1(4, "You let your");
  line2(5, " city get");
  line3(5, "B O O M E D");
  mp3.playFileByIndexNumber(3);
  for (int i = 1; i < 40; i++)
  {
    digitalWrite(10, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                       // wait for a second
    //    display.blink();
    digitalWrite(10, LOW);    // turn the LED off by making the voltage LOW
    delay(50);
  }// wait for a second
  while (1);
}

void success()
{
  display.blink();
  mp3.setLoopMode(MP3_LOOP_ONE_STOP);
  lcd.clear();
  line1(1, "Congratulations !!!");
  line2(1, "Your final clue to");
  line3(4, " escape is");
  line4(6, " hELL");
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

