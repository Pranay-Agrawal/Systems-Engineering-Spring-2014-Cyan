#include <Servo.h>
Servo myservo;  // create servo object to control a servo
int pos = 0;    // variable to store the servo position
const int PMpin = 0;
int PMvalue = 0;

void setup()
{
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
  PMvalue = analogRead(PMpin);
  // PMvalue is ranging from 0 to 1023.
  // Servo position is ranging from 0 to 180.
  // Convert the analog signal to servo's position.
  pos = 1.0*PMvalue/1024*180;
  myservo.write(pos);
}

