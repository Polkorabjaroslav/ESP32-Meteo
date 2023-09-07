#include <Wire.h>
#include <arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_BME280.h>


#define SCREEN_WIDTH 128
#define SCREEN_HIGHT 32
#define BUTTON_ADD  26
#define BUTTON_SUB  33

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HIGHT,&Wire,-1);
Adafruit_BME280 bme;

void perifcheck();
void buttonhandle();
void buttonlogic();
void menuhandle();

int buttonStateAdd = 0;
int lastStateAdd  = LOW;
int buttonStateSub = 0;
int lastStateSub  = LOW;
int buttonCount;
int menuStatus;

void setup() 
{
  Serial.begin(115200);
  
  pinMode(BUTTON_ADD, INPUT_PULLUP);
  pinMode(BUTTON_SUB, INPUT_PULLUP);

  perifcheck();

}

void loop() 
{

  buttonhandle();
  buttonlogic();
  menuhandle();
  
}
void perifcheck()
{

  bool status = bme.begin(0x76);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("Display nenalezen"));
    for(;;);
  }

  if(!status)
  {
    Serial.println("Senzor nenalezen!");
  }
  delay(2000);

  display.display();
  display.clearDisplay();
  display.setTextColor(WHITE);

}

void buttonhandle()
{

  int addState = digitalRead(BUTTON_ADD);
  int subState = digitalRead(BUTTON_SUB);

  if (buttonStateAdd != lastStateAdd)
  {
    if(addState == LOW)
    {

      buttonCount++;

    }
  }

  if (buttonStateSub != lastStateSub)
  {
    if(subState == LOW)
    {

      buttonCount--;

    }
  }
    
  lastStateSub = subState;
  lastStateAdd = addState;
}

void buttonlogic()
{
  if(buttonCount > 2)
  {

    buttonCount = 0;

  }
  if(buttonCount < 0)
  {

    buttonCount = 2;

  }
}

void menuhandle()
{
  display.clearDisplay();
  switch (buttonCount)
  {
  case 0:
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("Teplota:");
    display.setCursor(0,16);
    display.print(String(bme.readTemperature()));
    display.print(" ");
    display.cp437(true);
    display.write(167);
    display.print("C");
    break;

  case 1:
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("Tlak:");
    display.setCursor(0,16);
    display.print(String(bme.readPressure()/100.0F));
    display.print(" hPa");
    break;

  case 2:
    display.setTextSize(2);
    display.setCursor(0,0);
    display.print("Vlhkost:");
    display.setCursor(0,16);
    display.print(String(bme.readHumidity()));
    display.print(" %");
    break;
  }

  display.display(); 
  Serial.println(buttonCount);

}