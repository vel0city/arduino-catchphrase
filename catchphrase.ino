#include <SD.h>
#include <SPI.h>
#include <TFTv2.h>

const int PIN_SD_CS = 4;                        // pin of sd card
File textFile;

void setup() {
  pinMode(PIN_SD_CS,OUTPUT);
  digitalWrite(PIN_SD_CS,HIGH);

  Tft.TFTinit();

  Sd2Card card;
  card.init(SPI_FULL_SPEED, PIN_SD_CS); 
    
  if(!SD.begin(PIN_SD_CS))              
  { 
    Serial.println("failed!");
    while(1);                               // init fail, die here
  }
    
  Serial.println("SD OK!");
  
  randomSeed(analogRead(0));

  TFT_BL_ON;
}

char[] getWord(void) {
  textFile = SD.open("NAMES.txt");
  
  // Go to a random spot in the file
  textFile.seek(random(textFile.size()));
  char value = ' ';
  
  // Keep reading until you hit a newline
  while(value != "\n") {
    value = textFile.read();
  }
  
  // Read in the whole line as the word
  char[20] textFile;
  int i = 0;
  while(value != "\n")
  {
    value = textFile.read();
    textFile[i] = value;
  }
}
