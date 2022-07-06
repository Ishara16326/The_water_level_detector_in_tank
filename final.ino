#include <LiquidCrystal_I2C.h>

#include <Wire.h>
  
LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

#define trigpin 3
#define echopin 4
float duration, waterHeight, currentLevel;
const int Relay = 13;
const int bulbON = 11;
const int bulbOFF = 12;

//Buttons Pins
const int LEVEL_UP = 9;
const int LEVEL_DOWN = 8;
const int OK = 7;
const int RESET = 10;

int bottomLevel = 0;
int topLevel = 100;
int count = 0; 

boolean relayon = false;
boolean pushRESET = false;



void setup() {

  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(Relay, OUTPUT);
  pinMode(bulbON,OUTPUT);
  pinMode(bulbOFF,OUTPUT);
  pinMode(LEVEL_UP, INPUT);
  pinMode(LEVEL_DOWN, INPUT);
  pinMode(OK, INPUT);
  pinMode(RESET, INPUT);


  lcd.begin();
  lcd.setBacklight(HIGH);

  lcd.setCursor(1, 0);
  lcd.print("Hello, Welcome");
  delay (1000);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Water Level");
  lcd.setCursor(3, 1);
  lcd.print("Indicator");
  delay (2000);

}

void loop() {

  //check reset press or not.................................................................................................................................
  if ((digitalRead(RESET) == HIGH) || (pushRESET == true)) {
    while (digitalRead(RESET) == HIGH);
    pushRESET = true;
  }
  else {
    pushRESET = false;
  }



  // reset...........................................................................................................................
  if (pushRESET) {
    //begin message...............................

    
    if (count==0){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Set Your Levels.");
      delay(2000);
      count=1;
    }
    // for bottomLevel .............................................................................................................
    
    if (count==1) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("WHAT IS BOTTOM");
      lcd.setCursor(0,1);
      lcd.print("LEVEL?: ");     

      if (digitalRead(LEVEL_UP) == HIGH) {
        while (digitalRead(LEVEL_UP) == HIGH);
        bottomLevel=bottomLevel+10;
      }

      if (digitalRead(LEVEL_DOWN) == HIGH) {
        while (digitalRead(LEVEL_DOWN) == HIGH);
        bottomLevel = bottomLevel-10;
      }

      if (bottomLevel <= 0) {
        bottomLevel = 0;
      }
      if (bottomLevel >= 90) {
        bottomLevel = 90;
      }

      lcd.setCursor(8,1);
      lcd.print(bottomLevel);
      lcd.setCursor(12,1);
      lcd.print("%");
      
      
      if(digitalRead(OK)==HIGH){
        while (digitalRead(OK) == HIGH);
        count=2;
       }
    }
        
    // for topLevel .............................................................................................................
    if (count==2) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("WHAT IS TOP");
      lcd.setCursor(0,1);
      lcd.print("LEVEL?: "); 
      

      if (digitalRead(LEVEL_UP) == HIGH) {
        while (digitalRead(LEVEL_UP) == HIGH);
        topLevel=topLevel+10;
      }

      if (digitalRead(LEVEL_DOWN) == HIGH) {
        while (digitalRead(LEVEL_DOWN) == HIGH);
        topLevel=topLevel-10;
      }

      if (topLevel >= 100) {
        topLevel = 100;
      }
      if (bottomLevel >= topLevel) {
        topLevel = bottomLevel + 10;
      }

      lcd.setCursor(8,1);
      lcd.print(topLevel);
      lcd.setCursor(12,1);
      lcd.print("%");

      
      if(digitalRead(OK)==HIGH){
        while (digitalRead(OK) == HIGH);
        count=3;
       }             
    }
    
    if((count==3)){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("BOTTOM LEVEL:");
      lcd.setCursor(13,0);
      lcd.print(bottomLevel);
      lcd.setCursor(15,0);
      lcd.print("%");

      lcd.setCursor(0,1);
      lcd.print("TOP LEVEL:");
      lcd.setCursor(12,1);
      lcd.print(topLevel);
      lcd.setCursor(15,1);
      lcd.print("%");
      
     
      
      delay(3000);
      pushRESET=false;
      count=0;
      }
  }


  //home..............................................................................................................................
  else {

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WATER LEVEL:");
    lcd.setCursor(0,1);
    lcd.print("WATER PUMP:");
        
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);
    delayMicroseconds(2);

    duration = pulseIn(echopin, HIGH);
    waterHeight = 13 - ((duration*0.034/2));
    currentLevel = waterHeight*10;

    lcd.setCursor(12,0);
    lcd.print(currentLevel);
    lcd.setCursor(15,0);
    lcd.print("%");
    delay(1000);

    if (currentLevel <= bottomLevel || relayon == true) {
      digitalWrite(bulbON,HIGH);
      digitalWrite(bulbOFF,LOW);
      lcd.setCursor(12,1);
      lcd.print("ON");
      delay(500);
      relayon = true;
      digitalWrite(Relay, LOW);
    }
    if (currentLevel >= topLevel) {
      relayon = false;
      digitalWrite(Relay, HIGH);
      digitalWrite(bulbOFF,HIGH);      
      digitalWrite(bulbON,LOW);
    }
    if((currentLevel > bottomLevel)&&(currentLevel < topLevel)&&(relayon == false)){
      lcd.setCursor(12,1);
      lcd.print("OFF");
      delay(500); 
      digitalWrite(bulbOFF,HIGH);      
      digitalWrite(bulbON,LOW);
       
      }

    
  }
} 
