#include "Ai_WS2811.h"

#define NUM_PIXELS 12
#define DATA_PIN 4

#define sensor A0
int red = 3;  
int green = 2;
bool trigger_flag = 0;
bool color_flag = 0;


Ai_WS2811 ws2811;

struct CRGB {
  unsigned char g;
  unsigned char r;
  unsigned char b;
} *leds;

//some initial values


void setup()
{
  ws2811.init(DATA_PIN,NUM_PIXELS);
  leds = (struct CRGB*)ws2811.getRGBData();
  pinMode(sensor, INPUT_PULLUP);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  digitalWrite(red, LOW);
  digitalWrite(green, HIGH);
  Serial.begin(9600);
}

void loop()
{
  rainbow();
}

void rg_change()
{
  int a = 0;
  a = digitalRead(sensor);
  delay(100);
  Serial.println(a);
  if(a == HIGH && trigger_flag == 0 && color_flag == 0)
  {
    digitalWrite(red, HIGH);
    digitalWrite(green, LOW);
    trigger_flag = 1;
  }

  if(a == LOW && trigger_flag == 1 && color_flag == 0)
  {
    color_flag = 1;
    trigger_flag = 0;
  }

  if(a == HIGH && trigger_flag == 0 && color_flag == 1)
  {
    digitalWrite(red, LOW);
    digitalWrite(green, HIGH);
    trigger_flag = 1;
  }

  if(a == LOW && trigger_flag == 1 && color_flag == 1)
  {
    color_flag = 0;
    trigger_flag = 0;
  }
}

/**
 * Color climb function
 **/
void rainbow(){
  while(1) {
    for(int i = 255; i >= 0; i--)
    {
      int val = i;
      for (int led = 0; led < NUM_PIXELS; led++) {
          val = (val + 255/NUM_PIXELS) % 255;
          setHue(val, led);
      }
      ws2811.sendLedData();
      delay(100);
    }
  }
}

/**
 * HVS to RGB comversion (simplified to the range 0-255)
 **/
void setHue(int h, uint8_t n) {
  //this is the algorithm to convert from RGB to HSV
  double r=0; 
  double g=0; 
  double b=0;

  double hf=h/42.5; // Not /60 as range is _not_ 0-360

  int i=(int)floor(h/42.5);
  double f = h/42.5 - i;
  double qv = 1 - f;
  double tv = f;

  switch (i)
  {
  case 0: 
    r = 1;
    g = tv;
    break;
  case 1: 
    r = qv;
    g = 1;
    break;
  case 2: 
    g = 1;
    b = tv;
    break;
  case 3: 
    g = qv;
    b = 1;
    break;
  case 4:
    r = tv;
    b = 1;
    break;
  case 5: 
    r = 1;
    b = qv;
    break;
  }

  leds[n].r = constrain((int)255*r,0,255);
  leds[n].g = constrain((int)255*g,0,255);
  leds[n].b = constrain((int)255*b,0,255);
}
