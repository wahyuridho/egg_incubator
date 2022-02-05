#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define ONE_WIRE_BUS 2

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int upButton = 10;
int downButton = 11;
int selectButton = 12;
int lamp = 9;
int count = 0;

void setup()
{
  Serial.begin(9600);
  sensors.begin();
  lcd.begin();
  lcd.clear();
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(lamp, OUTPUT);
}


void loop()
{
  int up, down, selectbtn;
  sensors.requestTemperatures();
  up = digitalRead(upButton);
  down = digitalRead(downButton);
  selectbtn = digitalRead(selectButton);
  byte readEEPROM = EEPROM.read(0);
  float suhu = sensors.getTempCByIndex(0);
  byte degree = B11011111;
  
  if(readEEPROM){
    lcd.setCursor(0,0);  
    lcd.print("Suhu Saat Ini");
    lcd.setCursor(0,1);
    lcd.print(suhu, 1);
    lcd.write(degree);
    lcd.print("C");

    if(selectbtn == LOW){
      setSuhu(up, down, selectbtn);
    }

    if(suhu >= readEEPROM){
      digitalWrite(lamp, HIGH);  
    }else{
      digitalWrite(lamp, LOW);  
    }
    
  }else{
    setSuhu(up, down, selectbtn);
  }
  Serial.print(readEEPROM);
  delay(1000);
}

void setSuhu(int up, int down, int selectbtn){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Set Temperature");
  if(up == LOW){
    countUp();
    lcd.setCursor(0,1);
    lcd.print(count);
    delay(100);
  }else if(down == LOW){
    countDown();
    lcd.setCursor(0,1);
    lcd.print(count);
    delay(100);    
  }
  if(selectbtn == LOW){
    EEPROM.write(0,count);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperature Saving");
    delay(2000);
  } 
}

int countUp(){
  return count++;  
}

int countDown(){
  count --;
  if(count < 0){
    count = 0;  
  }
  return count;
}
