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

/* ScreenModes */
#define MainScreen 0
#define MenuScreen 1
#define TimeSetScreen 2
#define HeaterSetScreen 3

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
byte systemHour, systemMinute;
const int centerX = 120;
const int centerY = 120;
const int COLOR1 = Orange;
const int COLOR2 = Black;
const int COLOR3 = GreenYellow;
const unsigned char myBitmap [] PROGMEM = {
// 'Roja', 40x40px
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x80, 
0x10, 0x00, 0x00, 0x00, 0x80, 0x30, 0x08, 0x00, 0x00, 0x40, 0x20, 0x18, 0x00, 0x00, 0x40, 0x20, 
0x30, 0x00, 0x00, 0x40, 0x7c, 0x40, 0x00, 0x00, 0x67, 0xc7, 0x80, 0x00, 0x00, 0x2c, 0x01, 0x80, 
0x00, 0x00, 0x30, 0x00, 0xe0, 0x00, 0x00, 0xc0, 0x00, 0x32, 0x30, 0x03, 0x00, 0x00, 0x1f, 0xe0, 
0x03, 0x00, 0x00, 0x0e, 0x00, 0x02, 0x00, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x01, 0x80, 0x04, 
0x08, 0x00, 0x00, 0xc0, 0x04, 0x0c, 0x41, 0xd0, 0x60, 0x04, 0x03, 0xc0, 0x70, 0x20, 0x04, 0x00, 
0x00, 0x00, 0x20, 0x04, 0x00, 0x00, 0x00, 0x20, 0x04, 0x00, 0x0c, 0x00, 0x30, 0x04, 0x00, 0x30, 
0x00, 0x10, 0x04, 0x00, 0x60, 0x00, 0x10, 0x04, 0x00, 0x3e, 0x00, 0x10, 0x04, 0x02, 0x00, 0x00, 
0x18, 0x04, 0x03, 0x80, 0x02, 0x18, 0x04, 0x00, 0xe0, 0x04, 0x18, 0x06, 0x00, 0x3f, 0x9c, 0x18, 
0x02, 0x00, 0x01, 0xf0, 0x10, 0x01, 0x80, 0x00, 0x00, 0x30, 0x00, 0xc0, 0x00, 0x00, 0x60, 0x00, 
0x70, 0x00, 0x01, 0x80, 0x00, 0x1c, 0x00, 0x07, 0x00, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x00, 0x00, 
0xff, 0xf0, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00

};
byte buttonPressed = 0;
byte screenMode = 0;

void setup() {
  const char SystemTime [] = __TIME__;
  systemHour = (SystemTime[0] - '0') * 10 + SystemTime[1] - '0';
  systemHour %= 12;
  systemMinute = (SystemTime[3] - '0') * 10  +SystemTime[4] - '0';
  // Serial.begin(9600);
  // Serial.println(systemHour);
  // Serial.println(systemMinute);
  uint16_t ID = tft.readID(); //
  tft.begin(ID);
  tft.setRotation(1);    //Landscape
  tft.fillScreen(COLOR2);
  drawScreen();  
}

/* BUTTONS */

void drawMenuButton() {
  tft.fillRoundRect(235, 190, 85, 50, 10, COLOR1);
  tft.setCursor(244, 204);
  tft.setTextColor(COLOR2);
  tft.setTextSize(3);
  tft.print("Menu");
}

void drawMoon() {
  tft.drawBitmap(240, 0, myBitmap, 40, 40, COLOR3);
}

void buttonHandle() {
  
}

/* END BUTTONS */


/* DRAW SCREEN */

void drawScreen() {
  switch (screenMode)
  {
  case MainScreen:
    drawMainScreen();
    break;
  case MenuScreen:
    drawMenuScreen();
    break;  
  case TimeSetScreen:
    drawTimeSetScreen();
    break;
  case HeaterSetScreen:
    drawHeaterSetScreen();
    break;
  default:
      break;
    }
  }
 
void drawMainScreen() {
  drawClockShape();
  clockRefresh();
  drawMenuButton();
  drawMoon();
}

void drawMenuScreen() {

}

void drawTimeSetScreen() {

}

void drawHeaterSetScreen() {

}

/* END DRAW SCREEN */

/* CLOCK */

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
  tft.fillCircle(centerX, centerY, 95, COLOR2);  
  drawHourHand(systemHour, systemMinute);  
  drawMinuteHand(systemMinute);
  tft.fillCircle(centerX, centerY, 8, COLOR1);
  tft.fillCircle(centerX, centerY, 2, COLOR2);
}

void drawClockShape() {
  tft.fillCircle(centerX, centerY, 119, COLOR1);
  tft.fillCircle(centerX, centerY, 114, COLOR2);
  for (int i=0; i<12; i++) {    
    float angle = calculateAngle(i, 12);
    int finalX = calculateFinalXPoint(angle, 105);
    int finalY = calculateFinalYPoint(angle, 105);      
    tft.drawLine(centerX, centerY, finalX, finalY, COLOR1);
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
    tft.fillTriangle(centerX-3, centerY-3, centerX+3, centerY+3, finalX, finalY, COLOR1);
  } else {
    tft.fillTriangle(centerX+3, centerY-3, centerX-3, centerY+3, finalX, finalY, COLOR1);
  }
}

void drawMinuteHand(byte minute) {
  float angle = calculateAngle(minute, 60);
  int finalX = calculateFinalXPoint(angle, 85);
  int finalY = calculateFinalYPoint(angle, 85);  
  if(finalX>centerX && finalY<centerY || finalX<centerX && finalY>centerY) {
    tft.fillTriangle(centerX-2, centerY-2, centerX+2, centerY+2, finalX, finalY, COLOR1);
  } else {
    tft.fillTriangle(centerX+2, centerY-2, centerX-2, centerY+2, finalX, finalY, COLOR1);
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

/* END CLOCK */

void loop() {
  clockRun();
}
