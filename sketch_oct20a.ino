#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int  buttonOK = 6;    
const int  buttonS = 12;    
const int  buttonM = 11;
const int  buttonH = 10; 

int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

void setup() {
  lcd.begin(16, 2);
  pinMode(buttonOK, INPUT);
}
void button(int button){
  buttonState = button;

  if (buttonState != lastButtonState) {
    if (buttonState == HIGH) {
      buttonPushCounter++;
      lcd.print(buttonPushCounter);
    } else {
    }
    lastButtonState = buttonState;
}
}

void loop() {
  button(digitalRead(buttonOK));
  lcd.setCursor(0, 1);
  delay(50);
  }
