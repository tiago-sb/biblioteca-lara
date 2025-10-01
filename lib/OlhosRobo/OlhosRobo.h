#ifndef OLHOSROBO_H
#define OLHOSROBO_H

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern const uint8_t image_data_DISTRESSED_EYESarray[] PROGMEM;
extern const uint8_t eye_lookleft [] PROGMEM;
extern const uint8_t eye_lookright [] PROGMEM; 
extern const uint8_t eye_nuetral [] PROGMEM;
extern const uint8_t eye_squint [] PROGMEM;

void look_nuetral(Adafruit_SSD1306 &display, int t);
void look_squint(Adafruit_SSD1306 &display, int t);
void look_right(Adafruit_SSD1306 &display, int t);
void look_left(Adafruit_SSD1306 &display, int t);
void blink_eyes(Adafruit_SSD1306 &display, int t, int numberOfblinks);
void sad_blink(Adafruit_SSD1306 &display, int t, int numberOfblinks);

#endif