#define ENC_A 14 // Pin 5 on the encoder
#define ENC_B 15 // Pin 3 on the encoder
#define ENC_PORT PINC
#define HISTORY_SIZE 100 // Where  

// Encoder variables
int8_t increment;
uint8_t oldValue = 0;
int8_t encoderStates[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0}; // Grey code lookup table

// Velocity variables
boolean bufferFilled = false;
int encoderTotal = 0, historyIndex = 0, velocity;
int history[HISTORY_SIZE];

//Pin Definitions
//Pin Definitions
//The 74HC595 uses a serial communication 
//link which has three pins
const int data  = 2; 
const int clock = 3;
const int latch = 4;

int LEDvalue = 0;

//Time to set up the Arduino pins (as defined above as the "ledAnalogOne" array) to outputs.
void setup(){
  pinMode(data, OUTPUT);
  pinMode(clock, OUTPUT);  
  pinMode(latch, OUTPUT);
  // Setup encoder pins as inputs
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);
}

void loop()
{
  // Get an encoder incremental reading (how far it has moved since we last checked)
  increment = readEncoder();
  if(increment != 0) {
    // We got a valid encoder incremental reading, so update the total encoder value
    encoderTotal += increment;
  }
  
  // Check to see if our buffer of stored encoder values has been filled - if it is filled, we can calculate velocity
  if(!bufferFilled) {
    // The buffer is not filled, so we will just store the current encoder total in the history and continue
    Serial.println("Filling buffer");
    history[historyIndex] = encoderTotal;
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    if(historyIndex == 0) {
      // We have looped back to the beginning of the history array, so it must be full
      bufferFilled = true;
    }
  } else {
    // The buffer is filled, so we will store the current encoder total in the history and calculate velocity
    // Velocity is just the difference in encoder total between the current encoder total and the total HISTORY_SIZE values ago
    velocity = encoderTotal - history[historyIndex];
    history[historyIndex] = encoderTotal;
    historyIndex = (historyIndex
    + 1) % HISTORY_SIZE;
  }
  LEDvalue = velocity > 0 ? 1.0*velocity/40*63 : -1.0*velocity/40*63;
  updateLEDs(LEDvalue);
}


/* 
 * This uses 4 bits to track grey code encoder status.
*/
int8_t readEncoder() {
  // Shift previous state two bits left into bits 3 and 4
  oldValue <<= 2;
  // Fill bits 1 and 2 with current state
  oldValue |= ( ENC_PORT & 0x03  );
  // Return grey code lookup value for transition from previous state to current state
  //  More info at http://en.wikipedia.org/wiki/Gray_code
  return encoderStates[( oldValue & 0x0f )];
}

void updateLEDs(int value)
{
    digitalWrite(latch, LOW);     //Pulls the chips latch low
    shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
    digitalWrite(latch, HIGH);   //Pulls the latch high displaying the data
}
