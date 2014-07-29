#include <SD.h>
#include <SPI.h>
#include <TFTv2.h>
#include <MemoryFree.h>
#include <SeeedTouchScreen.h>
#include "categoryMenu.h"

// Boilerplate touchscreen defs
#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) // mega
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3 

#elif defined(__AVR_ATmega32U4__) // leonardo
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 18   // can be a digital pin, this is A0
#define XP 21   // can be a digital pin, this is A3 

#else //168, 328, something else
#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3 

#endif

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2

const int PIN_SD_CS = 4;   // pin of sd card
File textFile;
String currentWord;
CategoryMenu category;

TouchScreen ts = TouchScreen(XP, YP, XM, YM);

void setup() {
  // Enable serial for debugging
  Serial.begin(9600);

  // Init SD Card
  pinMode(53,OUTPUT);
  digitalWrite(10,HIGH);
  Sd2Card card;
  
  card.init(SPI_FULL_SPEED, PIN_SD_CS);

  if(!SD.begin(PIN_SD_CS))              
  { 
    Serial.println("failed!");
    while(1);       // init fail, die here
  }
  Serial.println("SD OK!");
  
  //category = CategoryMenu();

  // For better random number generator,
  // seed noise from unconnected pin
  randomSeed(analogRead(0));

  // init screen and enable backlight
  Tft.TFTinit();
  TFT_BL_ON;
}

void loop() {
  // Get a point and see if anyone touched the screen
  Point p = ts.getPoint();
  if (p.z > 20) {
    Serial.print("freeMemory()=");
    Serial.println(freeMemory());
    drawNewWord();
    delay(50);
    
  }
}

void drawNewWord(void)
{
  Serial.println("Ooooh! That tickles!");
  // Someone touched! Get a new word, clear the screen, and draw the new one!
  currentWord = getWord();
  char drawWord[22];
  int length = currentWord.length();
  currentWord.toCharArray(drawWord, length);
  Serial.print("The word to draw:");
  Serial.println(currentWord);

  // Calculate the draw width from the midpoint of the screen
  int width = length * 11;
  // Calculate draw starting point
  int startPoint = 120 - (width / 2);

  // drawRectangle(INT16U poX, INT16U poY, INT16U length,INT16U width,INT16U color);
  Tft.fillRectangle(0,110,240,14,BLACK);
  Tft.fillRectangle(startPoint,110,(width -2),16,BLACK);
  Tft.drawString(drawWord,startPoint,110,2,RED);
}

String getWord(void) {
  Serial.println("Opening text file for input...");

  // Keep reading until you hit a newline or the end of the file
  // Repeat if we hit the end of the file
  boolean hitEOF = true;
  char value = ' ';
  while (hitEOF) {
    textFile = SD.open("NAMES.txt");
    textFile.seek(random(textFile.size()));
    while(textFile.available()) {
      value = textFile.read();
      if (value == ',') {
        hitEOF = false;
        break;
      }
      // 
      textFile.close();
    }
  }

  Serial.println("Found the start of a new word! Reading that word...");
  // Read in the whole line as the word
  String newWord;
  int i = 0;
  value = ' ';
  while(value != ',' && i < 20 && textFile.available())
  {
    value = textFile.read();
    newWord.concat(value);
    i++;
  }
  textFile.close();
  return newWord;
}

