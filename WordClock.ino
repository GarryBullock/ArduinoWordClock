#include <Adafruit_NeoPixel.h>
#include <avr/power.h> // Comment out this line for non-AVR boards (Arduino Due, etc.)
#include <Wire.h>
#include "RTClib.h"

#define PIN_MINUTES 2
#define NUMPIX_MINUTES 4

#define PIN_HOUR 4
#define NUMPIX_HOUR 51

#define PIN_DESIGNATION 6
#define NUMPIX_DESIGNATION 35

int IT[] = {0,1};
int IS[] = {2,3};
int QUARTER[] = {4,5,6,7,8,9,10};
int A[] = {11};
int TWENTY[] = {12,13,14,15,16,17};
int FIVE[] = {18,19,20,21};
int HALF[] = {22,23,24,25};
int TEN[] = {26,27,28};
int TO[] = {29,30};
int PAST[] = {31,32,33,34};

int HOUR_TWELVE[] = {0,1,2,3,4,5};
int HOUR_THREE[]= {6,7,8,9,10};
int HOUR_EIGHT[] = {11,12,13,14,15};
int HOUR_FIVE[] = {16,17,18,19};
int HOUR_ONE[] = {20,21,22};
int HOUR_SIX[] = {23,24,25};
int HOUR_SEVEN[] = {26,27,28,29,30};
int HOUR_TEN[] = {31,32,33};
int HOUR_NINE[] = {34,35,36,37};
int HOUR_TWO[] = {38,39};
int HOUR_ELEVEN[] = {40,41,42,43,45,46};
int HOUR_FOUR[] = {47,48,49,50};

int grid_brightness = 128;

RTC_DS1307 rtc;
Adafruit_NeoPixel designationStrip = Adafruit_NeoPixel(NUMPIX_DESIGNATION, PIN_DESIGNATION, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel minutesStrip = Adafruit_NeoPixel(NUMPIX_MINUTES, PIN_MINUTES, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel hourStrip = Adafruit_NeoPixel(NUMPIX_HOUR, PIN_HOUR, NEO_GRB + NEO_KHZ800);

unsigned long letterColor = minutesStrip.Color(0,150,0);

void setup() {
  // put your setup code here, to run once:
  minutesStrip.begin();
  minutesStrip.show();
  minutesStrip.setBrightness(grid_brightness);
  
  hourStrip.begin();
  hourStrip.show();
  hourStrip.setBrightness(grid_brightness);
  
  designationStrip.begin();
  designationStrip.show();
  designationStrip.setBrightness(grid_brightness);
  
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();

  
  //rtc.adjust(DateTime(__DATE__,__TIME__));

  unsigned long testColor = minutesStrip.Color(0,0,150);

  gridBrightness(grid_brightness);
  testStrip(35, &designationStrip, testColor);
  testStrip(51, &hourStrip, testColor);
  testStrip(4, &minutesStrip, testColor);
}

void loop() {
  // put your main code here, to run repeatedly:
  DateTime now = rtc.now();  //retrieve current time from clock
  
  Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  
  //gridBrightness(grid_brightness);
  //letterColor = randomColor();
  drawMinutes(now, letterColor);
  drawDesignation(now, letterColor);
  drawHour(now, letterColor);
  delay(10000);
}

void drawDesignation(DateTime currentTime, unsigned long color){
  unsigned int minutes = currentTime.minute();
  
  //setup minutes for our 5 minute intervals
  minutes = minutes / 5;

  clearStrip(&designationStrip);
  
  writeWord(IT, 2, &designationStrip, letterColor);
  writeWord(IS, 2, &designationStrip, letterColor);
  
  switch(minutes){
    case 0: //it is x
      break;
    case 1: //it is 5 past x
      writeWord(FIVE, 4, &designationStrip, letterColor);
      writeWord(PAST, 4, &designationStrip, letterColor);
      break;
    case 2: //it is 10 past x
      writeWord(TEN, 3, &designationStrip, letterColor);
      writeWord(PAST, 4, &designationStrip, letterColor);
      break;
    case 3: //it is quarter past x
      writeWord(QUARTER, 7, &designationStrip, letterColor);
      writeWord(PAST, 4, &designationStrip, letterColor);
      break;
    case 4: //it is twenty past x
      writeWord(TWENTY, 6, &designationStrip, letterColor);
      writeWord(PAST, 4, &designationStrip, letterColor);
      break;
    case 5: //it is twenty five past x
      writeWord(TWENTY, 6, &designationStrip, letterColor);
      writeWord(FIVE, 4, &designationStrip, letterColor);
      writeWord(PAST, 4, &designationStrip, letterColor);
      break;
    case 6: //it is half past x
      writeWord(HALF, 4, &designationStrip, letterColor);
      writeWord(PAST, 4, &designationStrip, letterColor);
      break;
    case 7: //it is twenty five to x
      writeWord(TWENTY, 6, &designationStrip, letterColor);
      writeWord(FIVE, 4, &designationStrip, letterColor);
      writeWord(TO, 2, &designationStrip, letterColor);
      break;
    case 8: //it is twenty to x
      writeWord(TWENTY, 6, &designationStrip, letterColor);
      writeWord(TO, 2, &designationStrip, letterColor);
      break;
    case 9: //it is quarter to x
      writeWord(QUARTER, 7, &designationStrip, letterColor);
      writeWord(TO, 2, &designationStrip, letterColor);
      break;
    case 10: //it is ten to x
      writeWord(TEN, 3, &designationStrip, letterColor);
      writeWord(TO, 2, &designationStrip, letterColor);
      break;
    case 11: //it is five to x
      writeWord(FIVE, 4, &designationStrip, letterColor);
      writeWord(TO, 2, &designationStrip, letterColor);
      break;
  }
}
void drawHour(DateTime currentTime, unsigned long color){
  //draws the current hour to the hour strip in the given color
  unsigned int hour = currentTime.hour();
  
  //map the current hour to 12 hour time, creates less cases for switch
  if(hour > 12){
    hour = hour - 12;
  }
  clearStrip(&hourStrip);
    
  switch(hour){
    case 1:
      //print one
      writeWord(HOUR_ONE, sizeof(HOUR_ONE)/sizeof(int), &hourStrip, color);
      break;
    case 2:
      //print two
      writeWord(HOUR_TWO, sizeof(HOUR_TWO)/sizeof(int), &hourStrip, color);
      break;
    case 3:
      writeWord(HOUR_THREE, sizeof(HOUR_THREE)/sizeof(int), &hourStrip, color);
      break;
    case 4:
      writeWord(HOUR_FOUR, sizeof(HOUR_FOUR)/sizeof(int), &hourStrip, color);
      break;
    case 5:
      writeWord(HOUR_FIVE, sizeof(HOUR_FIVE)/sizeof(int), &hourStrip, color);
      break;
    case 6:
      writeWord(HOUR_SIX, sizeof(HOUR_SIX)/sizeof(int), &hourStrip, color);
      break;
    case 7:
      writeWord(HOUR_SEVEN, sizeof(HOUR_SEVEN)/sizeof(int), &hourStrip, color);
      break;
    case 8:
      writeWord(HOUR_EIGHT, sizeof(HOUR_EIGHT)/sizeof(int), &hourStrip, color);
      break;
    case 9:
      writeWord(HOUR_NINE, sizeof(HOUR_NINE)/sizeof(int), &hourStrip, color);
      break;
    case 10:
      writeWord(HOUR_TEN, sizeof(HOUR_TEN)/sizeof(int), &hourStrip, color);
      break;
    case 11:
      writeWord(HOUR_ELEVEN, sizeof(HOUR_ELEVEN)/sizeof(int), &hourStrip, color);
      break;
    case 12:
      writeWord(HOUR_TWELVE, sizeof(HOUR_TWELVE)/sizeof(int), &hourStrip, color);
      break;
    default:
      break;
  }
  return;
}

void drawMinutes(DateTime currentTime, unsigned long color){
  //checks the current minutes and prints the corresponding value to the minutes strip.
  switch(currentTime.minute()%5){
    case 0:
      minutesStrip.clear();
      break;
    case 1:
      minutesStrip.setPixelColor(0, color);
      break;     
    case 2:
      for(int i = 0; i < 2; i++){
        minutesStrip.setPixelColor(i, color);
      }
      break;    
    case 3:
      for(int i = 0; i < 3; i++){
        minutesStrip.setPixelColor(i, color);
      }
      break; 
    case 4:
       for(int i = 0; i < 4; i++){
        minutesStrip.setPixelColor(i, color);
      }
      break;
  }
  minutesStrip.show();
}//printMinutes  

void writeWord(int arr[], int arrSize, Adafruit_NeoPixel* strip, unsigned long color){
  //Writes a given word to a strip based on pixel locations
  for(int i = 0; i < arrSize; i++){
      strip->setPixelColor(arr[i], color);
  }
  strip->show();
}//writeWord


//test all led's in a strip that has been initialized.
void testStrip(int numPix, Adafruit_NeoPixel* strip, unsigned long color){
  for(int i = 0; i < numPix; i++){
    strip->clear();
    strip->setPixelColor(i, color);
    strip->show();
    delay(300);
  }

  strip->clear();
  strip->show();
}
void clearStrip(Adafruit_NeoPixel* strip){
  strip->clear();
  strip->show();
}

unsigned long randomColor(){
  return minutesStrip.Color(random(255),random(255),random(255));
}

void gridBrightness(int brightness){
  //Sets the brightness for each of the three strips to argument brightness
  minutesStrip.setBrightness(brightness);
  designationStrip.setBrightness(brightness);
  hourStrip.setBrightness(brightness);
}//gridBrightness

void wordTest(){
  writeWord(IT, 2, &designationStrip, letterColor);
  delay(2000);
  clearStrip(&designationStrip);
  delay(2000);
  writeWord(IS, 2, &designationStrip, letterColor);
  delay(2000);
  clearStrip(&designationStrip);
  return;
}

