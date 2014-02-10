// define pin numbers
const int button1pin = 2;
const int LEDpin = 13;

// define state variables
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
  Serial.begin(9600);
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
    Serial.println(n);  
    case 0:
      LEDstate = HIGH;
      break;
    case 1:
      LEDstate = !LEDstate;
      break;
    case 2:
      LEDstate = LOW;
      break;
  }
  digitalWrite(LEDpin, LEDstate);
  delay(200);
}
