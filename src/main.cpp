#include <Arduino.h>
#include <bits/stdc++.h>
#include <hardware/gpio.h>
#include "hardware/pio.h"
#include <strings.h>
#include <EEPROM.h>

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#include "globals.h"
//#include "lv_helper.h"

TFT_eSPI tft = TFT_eSPI();

XPT2046_Touchscreen ts(TOUCH_CS, TFT_IRQ);

/*  Adafruit_ST7796S_kbv(int8_t _CS, int8_t _DC, int8_t _MOSI, int8_t _SCLK,
                   int8_t _RST = -1, int8_t _MISO = -1);*/
//Adafruit_ST7796S_kbv tft = Adafruit_ST7796S_kbv(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST, TFT_MISO);

bool toggle = false;
bool toggle2 = false;
uint32_t atime = 0;
uint16_t debounce = 600;
/*
// Code to run a screen calibration, not needed when calibration values set in setup()
void touch_calibrate()
{
  uint16_t calData[6];

  // Calibrate
 
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(20, 0);
  tft.setTextFont(2);
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.println("Touch corners as indicated");

  tft.setTextFont(1);
  tft.println();

  tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);


  EEPROM.begin(sizeof(calData));
  EEPROM.put(EEPROM_ADDR, calData);  
  EEPROM.commit();
  EEPROM.end(); 
 
  tft.fillScreen(TFT_BLACK);
  
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.println("Calibration complete!");
  tft.println("Calibration code sent to Serial port.");

  delay(3000);
}
*/

void callInterrupt(uint gpio, uint32_t event)
{
  digitalWrite(25, toggle2 ? LOW : HIGH);
  toggle2 = !toggle2;
  Serial.println("Interrupt ");
  delay(50);
}
void callInterrupt2()
{
  digitalWrite(25, toggle2 ? LOW : HIGH);
  toggle2 = !toggle2;
  if ( (millis() - debounce) > atime){
    
    Serial.println("Interrupt2 ");
    atime = millis();
  }
  delay(50);
}

void setup()
{
  uint16_t calData[6];
  EEPROM.begin(sizeof(calData));
  delay(1000);
  Serial.begin(57600);
  EEPROM.get(EEPROM_ADDR, calData);
  EEPROM.end();

  gpio_init(15);
  gpio_set_drive_strength(15, GPIO_DRIVE_STRENGTH_8MA);
  gpio_set_dir(15, GPIO_OUT);
  delay(2000);

  if (calData[5] != 4)
  {
    //touch_calibrate();
  }

  tft.init();
  //tft.begin();
  //tft.fillScreen(ST7796S_BLACK);

  tft.fillScreen(TFT_BLACK);

  tft.println("test");
  ts.begin();
  //tft.println("test");

  Serial.println("Setup done22");

  //pinMode(TFT_RST, INPUT_PULLUP);
  /* gpio_init(TFT_IRQ);
  gpio_set_dir(TFT_IRQ, GPIO_IN);
  gpio_pull_up(TFT_IRQ);
  gpio_set_irq_enabled_with_callback(TFT_IRQ, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &callInterrupt);
     */
  
  SPI.usingInterrupt(digitalPinToInterrupt(TFT_IRQ));
  attachInterrupt(digitalPinToInterrupt(TFT_IRQ), callInterrupt2, LOW);
}

void loop()
{
  //lv_task_handler();
  gpio_put(15, toggle);
  toggle = !toggle;
  sleep_ms(50);
  tft.setCursor(0, 0);
  //tft.setTextColor(ST7796S_WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  //if (ts.touched()) {
    //Serial.println("meine");
  //}
}
