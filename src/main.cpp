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

bool toggle = false;
bool toggle2 = false;
uint32_t atime = 0;
uint16_t debounce = 600;

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

  tft.fillScreen(TFT_BLACK);

  tft.println("test");
  ts.begin();

  Serial.println("Setup done22");
  
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
  tft.println("Hello World!");
}
