#include <TFT_eSPI.h>                   // Include the graphics library (this includes the sprite functions)
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RAND_MAX 200

TFT_eSPI    tft = TFT_eSPI();           // Create object "tft"
TFT_eSprite arc = TFT_eSprite(&tft);
TFT_eSprite spd = TFT_eSprite(&tft);
TFT_eSprite bat = TFT_eSprite(&tft);


// Jpeg image array attached to this sketch
#include "fond.h"
#include "logo.h"

uint8_t       vitesse = 0;
uint16_t      old_angle = 89;
uint16_t      batterie = 100;
uint16_t      angle = 1;
uint32_t      dist = 10000;
bool          dec = false;


void setup(void) {
  //Serial.begin(250000);

  tft.init();
  tft.setRotation(1);
  tft.setSwapBytes(true);
  tft.pushImage(0,0,320,240,logo);
  while (angle!=360)
  {
    tft.drawArc(320/2,200,20,15,0,angle,TFT_WHITE,TFT_WHITE,true);
    angle++;
    delay(9);
  }
  tft.pushImage(0,0,320,240,fond);
  tft.setTextFont(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("100%",60+(320-240)/2,72);
  tft.drawString("0%",60+(320-240)/2,194);
  

  arc.setColorDepth(8);

  spd.setColorDepth(8);
  spd.setTextDatum(MC_DATUM);
  spd.setTextColor(TFT_WHITE,TFT_BLACK,true);
  spd.setTextSize(1);
  spd.setTextFont(8);

  bat.setColorDepth(8);
  bat.setTextDatum(MC_DATUM);

  tft.setTextDatum(MC_DATUM);
}






void drawarc(uint8_t vitesse)
{
  uint16_t angle = vitesse*3+90;
  
  arc.createSprite(320, 240);
  arc.fillSprite(TFT_TRANSPARENT);

  if (angle < 90) angle = 90; // Limit angle to emulate needle end stops
  if (angle > 270) angle = 270;


  
    // Move the needle until new angle reached
  while (angle != old_angle) {
    if (abs(old_angle-angle)>40) old_angle=angle;

    if (old_angle < angle) old_angle++;
    else old_angle--;

    // Only plot needle at even values to improve plotting performance
    if ( (old_angle & 3) == 0)
    {
      if (old_angle<=180)
      {
        arc.drawArc(120+(320-240)/2,120,120,100,90,old_angle,TFT_DARKGREEN,TFT_BLACK,true);
        arc.drawArc(120+(320-240)/2,120,120,100,old_angle,270,TFT_BLACK,TFT_BLACK,true);
        arc.pushSprite(0,0,TFT_TRANSPARENT);
      }else if (old_angle<=225 && old_angle>181)
      {
        arc.drawArc(120+(320-240)/2,120,120,100,90,180,TFT_DARKGREEN,TFT_BLACK,true);
        arc.drawArc(120+(320-240)/2,120,120,100,181,old_angle,TFT_ORANGE,TFT_BLACK,true);
        arc.drawArc(120+(320-240)/2,120,120,100,old_angle,270,TFT_BLACK,TFT_BLACK,true);
        arc.pushSprite(0,0,TFT_TRANSPARENT);
      }else if (old_angle>226)
      {
        arc.drawArc(120+(320-240)/2,120,120,100,90,180,TFT_DARKGREEN,TFT_BLACK,true);
        arc.drawArc(120+(320-240)/2,120,120,100,181,225,TFT_ORANGE,TFT_BLACK,true);
        arc.drawArc(120+(320-240)/2,120,120,100,226,old_angle,TFT_RED,TFT_BLACK,true);
        arc.drawArc(120+(320-240)/2,120,120,100,old_angle,270,TFT_BLACK,TFT_BLACK,true);
        arc.pushSprite(0,0,TFT_TRANSPARENT);
      }
      
        
      delay(3);
    }
  }
  arc.deleteSprite();
}

void drawspeed(uint8_t vitesse)
{
  uint16_t sizeSprite = 140;
  uint16_t locateX = 80+(320-240)/2;
  uint16_t locateY = 50;

  spd.createSprite(sizeSprite, sizeSprite);
  spd.fillSprite(TFT_TRANSPARENT);
  spd.fillCircle(sizeSprite/2,sizeSprite/2,sizeSprite/2,TFT_BLACK);
  spd.drawNumber(vitesse, sizeSprite/2,sizeSprite/2);
  spd.setTextFont(2);
  spd.drawString("km/h",sizeSprite/2,sizeSprite-25);
  spd.setTextFont(8);
  spd.pushSprite(locateX,locateY,TFT_TRANSPARENT);
  spd.deleteSprite();
  
}

void drawbatt(uint16_t batt){
  bat.createSprite(20,106);
  bat.fillRect(0,0,20,106,TFT_WHITE);

  if (batterie>20)
  {
    bat.fillRect(3,3,14,100,TFT_NAVY);
    bat.fillRect(3,103-batt,14,batt,TFT_BLUE);
  }else{
    bat.fillRect(3,3,14,100,TFT_NAVY);
    bat.fillRect(3,103-batt,14,batt,TFT_RED);
  }
  
  bat.pushSprite(50+(320-240)/2,80);
  bat.deleteSprite();
}

void drawkm()
{
  tft.setTextFont(2);
  tft.drawString(String(dist)+" km",150+(320-240)/2,200);
  dist++;
  if (dist==999999)
  {
    dist = 1664;
  }
  tft.setTextFont(1);
}

void loop() {
  
  
  if (vitesse>60){
    dec = true;
  }else if (dec==false)
  {
    vitesse++;
  }

  if (vitesse<1){
    dec = false;
  }else if (dec==true)
  {
    vitesse--;
  }


  if (batterie==0) batterie = 100;
  else batterie--;
 
  drawkm();
  drawspeed(vitesse);
  drawarc(vitesse);
  drawbatt(batterie);
  delay(rand()%200);
}

