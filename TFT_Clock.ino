#include "Adafruit_GFX.h"
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;

// ***TouchScreen***
#include <TouchScreen.h>
// калибровка тачскрина
#define TS_MINX 84
#define TS_MINY 62
#define TS_MAXX 953
#define TS_MAXY 904
// Сила нажатия на дисплей
#define MINPRESSURE 10
#define MAXPRESSURE 1000
// Последним параметром указывается сопротивление между X+ и X-
// Для данных дисплеев 300 подойдет
uint8_t YP = A1;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 7;   // can be a digital pin
uint8_t XP = 6;   // can be a digital pin

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
// ***TouchScreen***


/* some RGB color definitions            */
#define Black           0x0000      /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255,   0 */
#define White           0xFFFF      /* 255, 255, 255 */
#define Orange          0xFD20      /* 255, 165,   0 */
#define GreenYellow     0xAFE5      /* 173, 255,  47 */
uint32_t initTime = millis();
byte systemHour;
byte systemMinute;
const int centerX = 120;
const int centerY = 120;


void setup() {
  String SystemTime = __TIME__;
  systemHour = SystemTime.substring(0,2).toInt();
  systemHour%=12;
  systemMinute = SystemTime.substring(3,5).toInt();
//  Serial.begin(9600);
//  Serial.println(systemHour);
//  Serial.println(systemMinute);
  uint16_t ID = tft.readID(); //
  tft.begin(ID);
  tft.setRotation(1);    //Landscape
  tft.fillScreen(Black);
  drawClockShape();
  drawHourHand(systemHour, systemMinute);  
  drawMinuteHand(systemMinute);
}

void clockRun(){
  if(millis() - initTime > 60000) {
    initTime = millis();
    systemMinute++;
    if(systemMinute == 60) {
      systemMinute = 0;
      systemHour++;
      systemHour%=12;
    }
    clockRefresh();
  }
}

void clockRefresh() {  
  tft.fillCircle(centerX, centerY, 95, Black);  
  drawHourHand(systemHour, systemMinute);  
  drawMinuteHand(systemMinute);
  tft.fillCircle(centerX, centerY, 8, Orange);
  tft.fillCircle(centerX, centerY, 2, Black);
}

void drawClockShape() {
  tft.fillCircle(centerX, centerY, 119, Orange);
  tft.fillCircle(centerX, centerY, 114, Black);
  for (int i=0; i<12; i++) {    
    float angle = calculateAngle(i, 12);
    int finalX = calculateFinalXPoint(angle, 105);
    int finalY = calculateFinalYPoint(angle, 105);      
    tft.drawLine(centerX, centerY, finalX, finalY, Orange);
  }
  clockRefresh();
}

void drawHourHand(byte hour, byte minute) {
  if(hour>=12) hour-=12;
  float exactHour = hour + (float)minute/60.0;
  Serial.print("exactHour: "); Serial.println(exactHour);
  float angle = calculateAngle(exactHour, 12);
  int finalX = calculateFinalXPoint(angle, 70);
  int finalY = calculateFinalYPoint(angle, 70);  
  if(finalX>centerX && finalY<centerY || finalX<centerX && finalY>centerY) {
    tft.fillTriangle(centerX-3, centerY-3, centerX+3, centerY+3, finalX, finalY, Orange);
  } else {
    tft.fillTriangle(centerX+3, centerY-3, centerX-3, centerY+3, finalX, finalY, Orange);
  }
}

void drawMinuteHand(byte minute) {
  float angle = calculateAngle(minute, 60);
  int finalX = calculateFinalXPoint(angle, 85);
  int finalY = calculateFinalYPoint(angle, 85);  
  if(finalX>centerX && finalY<centerY || finalX<centerX && finalY>centerY) {
    tft.fillTriangle(centerX-2, centerY-2, centerX+2, centerY+2, finalX, finalY, Orange);
  } else {
    tft.fillTriangle(centerX+2, centerY-2, centerX-2, centerY+2, finalX, finalY, Orange);
  }
}

float calculateAngle(float counter, byte divisor) {
  float angle = 2*M_PI/divisor*counter-M_PI/2;
  return angle;
}

int calculateFinalXPoint(float angle, byte large) {
  int finalX = centerX+(int)large*cos(angle);  
  return finalX;
}

int calculateFinalYPoint(float angle, byte large) {
  int finalY = centerY+(int)large*sin(angle);
  return finalY;
}


void loop() {
  clockRun();
}
