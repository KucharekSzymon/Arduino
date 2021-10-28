#include <LiquidCrystal.h>
#include <TimerOne.h>
#include <ClickEncoder.h>

#define ENCODER_CLK A0
#define ENCODER_DT A1
#define ENCODER_SW A2

//const int ledPins[] = {A5, A4, A3, 7, 8, 9, 10, 13};
const int ledPins[] = {13, 10, 9, 8, 7, A3, A4, A5};
int losowa;
int tabA[8];
int tabB[8];

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int buttonOK = 6;
String pause = ":";
bool lup = false;
bool dclick = false;

int buttonPushCounter = 0;
int buttonState = 0;
int lastButtonState = 0;

ClickEncoder *encoder; // variable representing the rotary encoder
int16_t last, value;   // variables for current and last rotation value
int16_t mi = 0, se = 0;

void timerIsr()
{
  encoder->service();
}

void timerElapsing()
{
  while (!lup)
  {
    se--;
    if (se < 0)
    {
      if (mi != 0)
      {
        se = 59;
        mi--;
      }
      else
        Alarm();
    }
    lcd.clear();
    lcd.print("Timer: ");
    lcd.setCursor(0, 1);
    lcd.print(mi + pause + se);
    delay(1000);
  }
}
void Alarm()
{
  while (!dclick)
  {
    ClickEncoder::Button b = encoder->getButton();
    if (b != ClickEncoder::Open)
    {
      switch (b)
      {
      case ClickEncoder::DoubleClicked:
        adder();
        dclick = true;
        lup = true;
        break;
      }
    }
    se = 0;
    lcd.clear();
    lcd.print("Alarm!");
    lcd.setCursor(0, 1);
    lcd.print("Press twice!");
    delay(50);
  }
}

void adder(){
  int a = los();
  int b = los();
  lcd.clear();
  lcd.print("A: ");
  lcd.print(a);
  lcd.print("-");

  int temp = a;
  for (int i = 0; i < 8; i++)
  {
    if (temp % 2)
      tabA[i] = 1;
    else
      tabA[i] = 0;
    temp /= 2;
    lcd.print(tabA[i]);
  }
  
  lcd.setCursor(0,1);
  lcd.print("B: ");
  lcd.print(b);
  lcd.print("-");

  temp = b;
  for (int i = 0; i < 8; i++)
  {
    if (temp % 2)
      tabB[i] = 1;
    else
      tabB[i] = 0;
    temp /= 2;
    lcd.print(tabB[i]);
  }

  BinNumer(a);
  delay(15000);
  BinNumer(b);
  delay(15000);
  for(int i = 0; i < 8; i++)
    digitalWrite(ledPins[i],LOW);
}

int los()
{
  return random(0, 256);
}
void BinNumer(int x)
{
  for (int i = 0; i < 8; i++)
  {
    if (x % 2)
      digitalWrite(ledPins[i], HIGH);
    else
      digitalWrite(ledPins[i], LOW);
    x /= 2;
  }
}

void button(int button)
{
  buttonState = button;

  if (buttonState != lastButtonState)
  {
    if (buttonState == HIGH)
    {
      BinNumer(losowa);
          lcd.clear();
    lcd.print("Timer: |Rand:");
    lcd.print(losowa);
    lcd.setCursor(0, 1);
    lcd.print(mi + pause + se);
    }
  }
  lastButtonState = buttonState;
}

void setup()
{
  lcd.begin(16, 2);
  pinMode(buttonOK, INPUT);

  for (int i = 0; i < 8; i++)
    pinMode(ledPins[i], OUTPUT);

  randomSeed(analogRead(0));

  encoder = new ClickEncoder(ENCODER_DT, ENCODER_CLK, ENCODER_SW);

  Timer1.initialize(1000); // Initializes the timer, which the rotary encoder uses to detect rotation
  Timer1.attachInterrupt(timerIsr);
  last = -1;
  Serial.begin(9600);
}

void loop()
{
  losowa = los();
  button(digitalRead(buttonOK));
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open)
  {
    switch (b)
    {
    case ClickEncoder::Clicked:
      timerElapsing();
      dclick = false;
      lup = false;
      break;
    }
  }
  se += encoder->getValue();
  if (se >= 60)
  {
    se = 0;
    mi++;
  }
  if (se < 0)
  {
    if (mi != 0)
    {
      se = 59;
      mi--;
    }
    else
      se = 0;
  }

  if (se != last)
  {
    lcd.clear();
    lcd.print("Timer: ");
    lcd.setCursor(0, 1);
    lcd.print(mi + pause + se);
    last = se;
  }

  delay(50);
}
