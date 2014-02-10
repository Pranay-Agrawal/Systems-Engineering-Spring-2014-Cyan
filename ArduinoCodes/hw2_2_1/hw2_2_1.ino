// Pin definitions
const int button1pin = 2;
const int LEDpin = 13;

// State definitions
boolean LEDstate = LOW;
int buttonCount = 0;
int buttonState;

// when button is pressed, the input is shorted to the ground,
// and gets zero(LOW)
int lastButtonState = HIGH;

void setup()
{
  pinMode(button1pin, INPUT);
  pinMode(LEDpin, OUTPUT);
}

void loop()
{
  buttonState = digitalRead(button1pin);
  if(buttonState != lastButtonState)
  {
    if(buttonState == LOW)
    {
      buttonCount++;
    }
  }
  lastButtonState = buttonState;
  int n = buttonCount % 3;
  switch(n)
  {
    case 0:
      LEDstate = HIGH;
      break;
    case 1:
       // Change the LEDstate in each loop so that it looks like flashing.
      LEDstate = !LEDstate;
      break;
    case 2:
      LEDstate = LOW;
      break;
  }
  digitalWrite(LEDpin, LEDstate);
  // Delay 200ms to show the flashing effect.
  delay(200);
}
