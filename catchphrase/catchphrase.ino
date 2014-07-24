#include <SD.h>
#include <SPI.h>
#include <TFTv2.h>
#include <SeeedTouchScreen.h>

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

const int PIN_SD_CS = 4;                        // pin of sd card
File textFile;
String currentWord;

void setup() {
  pinMode(PIN_SD_CS,OUTPUT);
  digitalWrite(PIN_SD_CS,HIGH);

  Tft.TFTinit();

  Sd2Card card;
  card.init(SPI_FULL_SPEED, PIN_SD_CS);
    
  if(!SD.begin(PIN_SD_CS))              
  { 
    Serial.println("failed!");
    while(1);       // init fail, die here
  }
    
  Serial.println("SD OK!");
  
  randomSeed(analogRead(0));

  // turn on the backlight
  TFT_BL_ON;
}

void loop() {
  // Get a point and see if anyone touched the screen
  Point p = ts.getPoint();
  if (p.z > __PRESSURE) {
    // Someone touched! Get a new word, clear the screen, and draw the new one!
    currentWord = getWord();
    Tft.drawRectangle(10,110,200,60,BLACK);
    Tft.drawString(currentWord,20,110,3,WHITE);
  }
}

char[] getWord(void) {
  textFile = SD.open("NAMES.txt");
  
  // Go to a random spot in the file
  textFile.seek(random(textFile.size()));
  char value = ' ';
  
  // Keep reading until you hit a newline or the end of the 
  while(value != "\n" && textFile.available()) {
    value = textFile.read();
  }
  
  // Read in the whole line as the word
  String newWord;
  int i = 0;
  while(value != "\n" && i < 20 && textFile.available())
  {
    value = textFile.read();
    newWord.concat(value);
    i++;
  }
  
  return newWord;
}