#include <Arduino.h>
#line 1 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
#include<LiquidCrystal.h>
#include<DHT.h>


//define pin
const int dhtPin = 3;
const int btnLampPin = 6;
const int lampPin = 4;
const int btnFanPin = 5;
const int fanPin = 2;
const int autoModePin = 7;

//define type of sensor
const int DHTTYPE = DHT11;
//init variable
float h;
float t;

float tempFan = 38;
float tempLamp = 20;

int autoMode = 0;


DHT dht(dhtPin, DHTTYPE);
LiquidCrystal lcd(13,12,11,10,9,8);      

byte degree[8] = {
0B01110,
0B01010,
0B01110,
0B00000,
0B00000,
0B00000,
0B00000,
0B00000
};

#line 39 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void setup();
#line 61 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void loop();
#line 78 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void displayAutoMode();
#line 86 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void changeStateAutoMode();
#line 97 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void onOffAutoMode();
#line 105 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void sensorDHTchangeState();
#line 118 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void onDevice(int pin);
#line 126 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void offDevice(int pin);
#line 134 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void changeDeviceState(int buttonPin,int pin);
#line 146 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void setDHT();
#line 39 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
void setup() {
  //setup pin
  pinMode(btnLampPin,INPUT);
  pinMode(lampPin,OUTPUT);
  pinMode(btnFanPin,INPUT);
  pinMode(fanPin,OUTPUT);
  pinMode(autoModePin,INPUT);

  //setup LCD
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("TEMP: ");
  lcd.setCursor(12,0);
  lcd.print("AUTO");
  lcd.setCursor(0,1);
  lcd.print("HUMI: ");
  lcd.createChar(1, degree);

  //setup DHT
  dht.begin();
}

void loop() {
  //config DHT sensor
  setDHT();
  displayAutoMode();
  
  changeStateAutoMode();
  
  if(autoMode == 1){
    //handle event sensor
    sensorDHTchangeState();
  } else {
    //handle button event
    changeDeviceState(btnLampPin,lampPin);
    changeDeviceState(btnFanPin,fanPin);
  }
}

void displayAutoMode(){
  lcd.setCursor(12,1);
  if(autoMode ==1){
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
}
void changeStateAutoMode(){
  int buttonState = digitalRead(autoModePin);
  if(buttonState == LOW && autoMode == 0){
    onOffAutoMode();
    delay(500);
  } else if(buttonState == LOW && autoMode == 1){
    onOffAutoMode();
    delay(500);
  }
}

void onOffAutoMode(){
  if(autoMode == 1){
    autoMode = 0;
  } else {
    autoMode = 1;
  }
}

void sensorDHTchangeState(){
  if(t>=tempFan){
    onDevice(fanPin);
    offDevice(lampPin);
  } else if(t<=tempLamp) {
    onDevice(lampPin);
    offDevice(fanPin);
  } else {
    offDevice(fanPin);
    offDevice(lampPin);
  }
}

void onDevice(int pin){
  int state = digitalRead(pin);
  if(state == LOW) {
    digitalWrite(pin,HIGH);
    delay(500);
  }
}

void offDevice(int pin){
  int state = digitalRead(pin);
  if(state == HIGH) {
    digitalWrite(pin,LOW);
    delay(500);
  }
}

void changeDeviceState(int buttonPin,int pin){
  int buttonState = digitalRead(buttonPin);
  int state = digitalRead(pin);
  if(buttonState == LOW && state == LOW) {
    digitalWrite(pin,HIGH);
    delay(500);
  } else if(buttonState == LOW && state == HIGH){
    digitalWrite(pin,LOW);
    delay(500);
  }
}
  
void setDHT() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(t) || isnan(h)) { 
  }
  else {
       lcd.setCursor(6,0);
       lcd.print(round(t));
       lcd.write(1);
       lcd.print("C");
       lcd.setCursor(6,1);
       lcd.print(round(h));
       lcd.print(" %");
     }
}

