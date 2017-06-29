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

// EEPROM INITIALIZE
#include <EEPROM.h>


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

// EEPROM CONSTRUCT
int address = 0;// the variable which holds the address in the eeprom
char EEPROM1 = 0;// the variable which holds the data which is read from the eeprom
char EEPROM2 = 0;// the variable which holds the data which is read from the eeprom
char serial_in_data;// the variable which holds serial input data
String diffuse_key = "";
String door_key = "";
int dkeylen = 0; // door key length
int diffkeylen = 0; // diff key length

//  RESET FUNCTION
void(* resetFunc) (void) = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // FLASH
  pinMode(11, OUTPUT);
  digitalWrite(11, LOW);

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

  // EEPROM READ
  for (address = 0; address < 15; address ++)
  {
    EEPROM1 = EEPROM.read(address);
    if (EEPROM1 != '*')
    {
      diffuse_key += EEPROM1 ;
    }
  }
  diffuse_key.trim();
  Serial.println("diffuse_key at 0 or old diffuse_key is : ");
  Serial.print(diffuse_key);
  Serial.println("   length : ");
  diffkeylen = diffuse_key.length();
  Serial.println(diffkeylen);

  for (address = 16; address < 30; address ++)
  {
    EEPROM2 = EEPROM.read(address);
    if (EEPROM2 != '*')
    {
      door_key += EEPROM2 ;
    }
  }
  door_key.trim();
  Serial.println("door_key at 0 or old door_key is : ");
  Serial.print(door_key);
  Serial.println("   length : ");
  dkeylen = door_key.length();
  Serial.println(dkeylen);

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
  mp3.playFileByIndexNumber(1);
  delay(3000);



  // TIMER CHECK
  display.print("CHEC");
  delay(1000);
  display.clear();
  if (debug = 1) {
    Serial.println("Timer Initiated");
  }


  digitalWrite(11, HIGH);
  delay(500);
  digitalWrite(11, LOW);
  if (debug = 1)
    Serial.println("Flash Initiated");

  if (debug = 1) {
    Serial.println("Remote Initiated");
  }


  lcd.clear();

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
          line2(1, "1-45 mins");
          line3(1, "2-30 mins");
          line4(1, "3-20 mins");
          line2(11, "4-10 mins");
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
                  ref = 1200;
                } break;
              case 131: {
                  ref = 600;
                } break;
              case 135: {
                  ref = 5;
                } break;
              default : break;
            }
            if (ref > 0) break;
          }
        } break;
      case 463: {
          int done = 0;
          lcd.clear();
          while (1)
          {

            line1(7, "C.R.E.W");
            line2(2, "Diff key -Green");
            line3(2, "Door key -Yellow");
            int arr = remote();

            switch (arr)
            {

              case 461:
                {
                  lcd.clear();
                  line1(7, "C.R.E.W");
                  line2(2, "Changing Pwd...");
                  Serial.println("diffuse_key Change Mode..");
                  delay(1000);
                  Serial.println("Clearing Old diffuse_key..");
                  delay(1000);
                  for (address = 0; address < 15; address ++)
                  {
                    EEPROM.write(address, ' ');
                  }
                  Serial.println("Enter diffuse_key and Click Send..");
                  for (address = 0; address < 15;)
                    if (Serial.available())
                    {
                      serial_in_data = Serial.read();
                      if (serial_in_data != '*')
                      {
                        EEPROM.write(address, serial_in_data);
                        address ++;
                      }
                      if (serial_in_data == '*')
                      {
                        address = 16;
                      }
                    }
                  lcd.clear();
                  Serial.println("diffuse_key Changed...");
                  done = 1;
                } break;

              case 462:
                {
                  lcd.clear();
                  line1(7, "C.R.E.W");
                  line2(2, "Changing Pwd...");
                  Serial.println("door_key Change Mode..");
                  delay(1000);
                  Serial.println("Clearing Old door_key..");
                  delay(1000);
                  for (address = 16; address < 30; address ++)
                  {
                    EEPROM.write(address, ' ');
                  }
                  Serial.println("Enter door_key and Click Send..");
                  for (address = 16; address < 30;)
                    if (Serial.available())
                    {
                      serial_in_data = Serial.read();
                      if (serial_in_data != '*')
                      {
                        EEPROM.write(address, serial_in_data);
                        address ++;
                      }
                      if (serial_in_data == '*')
                      {
                        address = 31;
                      }
                    }
                  Serial.println("door_key Change Successful...");
                  lcd.clear();
                  done = 1;
                } break;
              default: done = 0; break;
            }
            if (done == 1) break;
          }
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
      case 132: {
          resetFunc();
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
  display.setColonOn(1);
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
    display.print(disp);
    if (temp == 0)
    {
      display.blink();
    }

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

    if (count == diffkeylen)
    {
      if (password == diffuse_key) {
        success();
      }
      else {
        count = 0;
        memset(password_c, 0, sizeof(password_c));
        password = "\0";
        ref -= 120;
        line3(5, "INCORRECT");
        display.blink();
      }
    }
    
    if (disp == "0000")
    {
      boom();
    }

    
  }
}

void boom()
{
  mp3.setLoopMode(MP3_LOOP_ONE_STOP);
  display.clear();
  display.setColonOn(1);
  display.print("0000");
//  display.blink();
  lcd.clear();
  line1(4, "You let your");
  line2(5, " city get");
  line3(5, "B O O M E D");
  mp3.playFileByIndexNumber(3);
  for (int i = 1; i < 100; i++)
  {
    digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(35);                       // wait for a second
    //    display.blink();
    digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
    delay(35);
  }// wait for a second
  int b = 0;
  while (1)
  {
    b = remote();
    if (b == 132)
      resetFunc(); //call reset
  }
}

void success()
{
  char dkey[] = "";
  display.blink();
  mp3.setLoopMode(MP3_LOOP_ONE_STOP);
  lcd.clear();
  line1(1, "Congratulations !!!");
  line2(1, "Your final clue to");
  line3(4, " escape is");
  door_key.toCharArray(dkey, dkeylen + 1 );
  line4(6, dkey);
  mp3.playFileByIndexNumber(4);
  delay(12000);
  int b = 0;
  while (1)
  {
    b = remote();
    if (b == 132)
      resetFunc(); //call reset
  }
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

