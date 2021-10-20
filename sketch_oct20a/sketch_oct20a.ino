#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <ClickEncoder.h>

#define ENCODER_CLK A0
#define ENCODER_DT A1
#define ENCODER_SW A2

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int  buttonOK = 6;    
const int  buttonS = 7;    
const int  buttonM = 8;
const int  buttonH = 9; 

int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

ClickEncoder *encoder; // variable representing the rotary encoder
int16_t last, value; // variables for current and last rotation value

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
      buttonPushCounter++;
      lcd.print(buttonPushCounter);
    } else {
    }
    lastButtonState = buttonState;
}
}

void loop() {
  button(digitalRead(buttonOK));
  

  value += encoder->getValue();

  // This part of the code is responsible for the actions when you rotate the encoder
  if (value != last) { // New value is different than the last one, that means to encoder was rotated
    if(last<value) // Detecting the direction of rotation
       lcd.print(value);//Consumer.write(MEDIA_VOLUME_UP); Replace this line to have a different function when rotating counter-clockwise
      else
       lcd.print(value);//Consumer.write(MEDIA_VOLUME_DOWN); Replace this line to have a different function when rotating clockwise
    last = value; // Refreshing the "last" varible for the next loop with the current value
   // Serial.print("Encoder Value: "); // Text output of the rotation value used manily for debugging (open Tools - Serial Monitor to see it)
    //Serial.println(value);
  }

  // This next part handles the rotary encoder BUTTON
  ClickEncoder::Button b = encoder->getButton(); // Asking the button for it's current state
  if (b != ClickEncoder::Open) { // If the button is unpressed, we'll skip to the end of this if block
    //Serial.print("Button: "); 
    //#define VERBOSECASE(label) case label: Serial.println(#label); break;
    switch (b) {
      case ClickEncoder::Clicked: // Button was clicked once
        digitalWrite(LED_BUILTIN, HIGH); // Replace this line to have a different function when clicking button once
      break;  
      case ClickEncoder::DoubleClicked: // Button was double clicked
         digitalWrite(LED_BUILTIN, LOW); // Replace this line to have a different function when double-clicking
      break;        
    }
  }
  lcd.setCursor(0, 1);
  delay(50);
  }
