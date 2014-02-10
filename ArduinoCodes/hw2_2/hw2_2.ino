// define pin numbers
const int button1pin = 2;
const int button2pin = 3;
const int button3pin = 4;
const int LEDpin = 13;

// define state variables
boolean LEDstate = LOW;
boolean button1state;
boolean button2state;
boolean button3state;

void setup()
{
    pinMode(button1pin, INPUT);
    pinMode(button2pin, INPUT);
    pinMode(button3pin, INPUT);
    pinMode(LEDpin, OUTPUT);
}

void loop()
{
    // when button is pressed, the input is shorted to the ground, and gets zero
    if(digitalRead(button1pin) == LOW)
    {
        LEDstate = HIGH;
    }
    else if(digitalRead(button2pin) == LOW)
    {
        LEDstate = LOW;
    }
    else if(digitalRead(button3pin) == LOW)
    {
        // use a loop to flash the LED
        // use if() to break the loop
        while(true)
        {
            digitalWrite(LEDpin, HIGH);
            delay(500);
            digitalWrite(LEDpin, LOW);
            if(digitalRead(button1pin) == LOW) break;
            if(digitalRead(button2pin) == LOW) break;
            delay(500);
        }
    }
    else
    {
    }
    digitalWrite(LEDpin, LEDstate);
}
