#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <ClickEncoder.h>

#define ENCODER_CLK A0
#define ENCODER_DT A1
#define ENCODER_SW A2

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int  buttonOK = 6;
String pause = ":";
bool lup = false;
bool dclick = false;

int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

ClickEncoder *encoder; // variable representing the rotary encoder
int16_t last, value; // variables for current and last rotation value
int16_t mi = 0,se = 0;

void timerIsr() {
  encoder->service();
}

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonOK, INPUT);
  
  pinMode(LED_BUILTIN, OUTPUT);
  encoder = new ClickEncoder(ENCODER_DT, ENCODER_CLK, ENCODER_SW);

  Timer1.initialize(1000); // Initializes the timer, which the rotary encoder uses to detect rotation
  Timer1.attachInterrupt(timerIsr); 
  last = -1;
}
void button(int button){
  buttonState = button;

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      se++;  
    } 
    lastButtonState = buttonState;
  }
}

void timerElapsing(){
  while(!lup){
    se--;
  if(se < 0){
    if(mi != 0){
      se = 59;
      mi--;
    }
    else
      Alarm();
    }
    lcd.clear();
    lcd.print("Timer:");
    lcd.setCursor(0,1);
    lcd.print(mi+pause+se);
      delay(1000);
  }

}
void Alarm(){
  while(!dclick){
      ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::DoubleClicked:
         dclick = true;
         lup = true;
      break;        
    }
  }
    se = 0;
    lcd.clear();
    lcd.print("Alarm!");
    lcd.setCursor(0,1);
    lcd.print("Press twice!");
    delay(50);
  }
  }

void loop() {
  button(digitalRead(buttonOK));
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Clicked:
        timerElapsing();
        dclick = false;
        lup = false;
      break;    
    }
  }
  se += encoder->getValue();
  if(se >= 60){
    se = 0;
    mi++;
  }
  if(se < 0){
    if(mi != 0){
      se = 59;
      mi--;
    }
    else
    se= 0;
    }
    

  if (se != last) {
    lcd.clear();
    lcd.print("Timer:");
    lcd.setCursor(0,1);
    lcd.print(mi+pause+se);
    last = se;
  }


  delay(50);
  }
