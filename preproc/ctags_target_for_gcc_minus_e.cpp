# 1 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino"
# 2 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino" 2
# 3 "C:\\Users\\hungp\\OneDrive\\Documents\\Arduino\\test\\test.ino" 2


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

byte degree[] = {
  7,
  5,
  7,
  0,
  0,
  0,
  0,
  0
};

byte tempChar[] = {
  4,
  10,
  14,
  14,
  31,
  31,
  14,
  0
};

byte humiChar[] = {
  4,
  10,
  10,
  17,
  17,
  17,
  14,
  0
};

void setup() {
  //setup pin
  pinMode(btnLampPin,0x0);
  pinMode(lampPin,0x1);
  pinMode(btnFanPin,0x0);
  pinMode(fanPin,0x1);
  pinMode(autoModePin,0x0);

  //setup LCD
  lcd.begin(16, 2);
  lcd.createChar(1, degree);
  lcd.createChar(2, tempChar);
  lcd.createChar(3, humiChar);
  lcd.setCursor(0,0);
  lcd.write(2);
  lcd.print("TEMP : ");
  lcd.setCursor(12,0);
  lcd.print("AUTO");
  lcd.setCursor(0,1);
  lcd.write(3);
  lcd.print("HUMI : ");


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
  if(buttonState == 0x0 && autoMode == 0){
    onOffAutoMode();
    delay(500);
  } else if(buttonState == 0x0 && autoMode == 1){
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
  if(state == 0x0) {
    digitalWrite(pin,0x1);
    delay(500);
  }
}

void offDevice(int pin){
  int state = digitalRead(pin);
  if(state == 0x1) {
    digitalWrite(pin,0x0);
    delay(500);
  }
}

void changeDeviceState(int buttonPin,int pin){
  int buttonState = digitalRead(buttonPin);
  int state = digitalRead(pin);
  if(buttonState == 0x0 && state == 0x0) {
    digitalWrite(pin,0x1);
    delay(500);
  } else if(buttonState == 0x0 && state == 0x1){
    digitalWrite(pin,0x0);
    delay(500);
  }
}

void setDHT() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(t) || isnan(h)) {
  }
  else {
       lcd.setCursor(7,0);
       lcd.print(((t)>=0?(long)((t)+0.5):(long)((t)-0.5)));
       lcd.write(1);
       lcd.print("C");
       lcd.setCursor(7,1);
       lcd.print(((h)>=0?(long)((h)+0.5):(long)((h)-0.5)));
       lcd.print(" %");
     }
}
