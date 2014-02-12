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

//RGB LED pins
int ledAnalogOne[] = {9, 10, 11};

//Defined Colors (different RGB (red, green, blue) values for colors
//(to add your own ie. fuscia experiment and then add to the list)
//Remember, 0 = OFF, and 255 is full-ON
const byte RED[] = {255, 0, 0}; 
const byte ORANGE[] = {83, 4, 0}; 
const byte YELLOW[] = {255, 255, 0}; 
const byte GREEN[] = {0, 255, 0}; 
const byte BLUE[] = {0, 0, 255}; 
const byte INDIGO[] = {4, 0, 19}; 
const byte VIOLET[] = {23, 0, 22}; 
const byte CYAN[] = {0, 255, 255}; 
const byte MAGENTA[] = {255, 0, 255}; 
const byte WHITE[] = {255, 255, 255}; 
const byte BLACK[] = {0, 0, 0}; 
const byte PINK[] = {158, 4, 79}; 
const byte* COLOR[] = {RED, ORANGE, YELLOW, GREEN, BLUE, INDIGO, VIOLET, CYAN, MAGENTA, WHITE, BLACK, PINK};
int colorIdx = 0;

//Time to set up the Arduino pins (as defined above as the "ledAnalogOne" array) to outputs.
void setup(){
  for(int i = 0; i < 3; i++){
   pinMode(ledAnalogOne[i], OUTPUT);   //Set the three LED pins as outputs
  }
  setColor(ledAnalogOne, BLACK);       //Turn off led 1
  
  // Setup encoder pins as inputs
  pinMode(ENC_A, INPUT);
  digitalWrite(ENC_A, HIGH);
  pinMode(ENC_B, INPUT);
  digitalWrite(ENC_B, HIGH);
  // For debugging output, start the Serial Monitor (CTRL + SHIFT + m) with 115200 baud
  Serial.begin (115200);
  Serial.println("Start");
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
    // Print value to Serial Monitor 
    Serial.print("Counter value: ");
    Serial.print(encoderTotal, DEC); 
    Serial.print("\tVelocity: ");
    Serial.println(velocity, DEC);
  }
  // Assume the maximum speed is 40.
  // The index ranges from 0 to 11.
  colorIdx = velocity > 0 ? 1.0*velocity/40*11 : -1.0*velocity/40*11;
  setColor(ledAnalogOne, (byte*)COLOR[colorIdx]);
}

void setColor(int* led, const byte* color){
 for(int i = 0; i < 3; i++){             //iterate through each of the three pins (red green blue)
   analogWrite(led[i], color[i]);        //set the analog output value of each pin to the input value (ie led[0] (red pin) to 255 (red input color)
 }
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
