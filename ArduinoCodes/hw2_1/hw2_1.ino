//Pin Definitions
//Pin Definitions
//The 74HC595 uses a serial communication 
//link which has three pins
const int data = 2; 
const int clock = 3;
const int latch = 4;

//Used for single LED manipulation
boolean ledState = 0;

//the number of milliseconds to delay between LED updates
int delayTime = 500;
        
//
byte bits[] = {B00000001, B00000010, B00000100, B00001000, 
                        B00010000, B00100000, B01000000, B10000000};
/*
 * setup() - this function runs once when you turn your Arduino on
 * We set the three control pins to outputs
 */
void setup()
{
    pinMode(data, OUTPUT);
    pinMode(clock, OUTPUT);  
    pinMode(latch, OUTPUT);  
}

/*
 * loop() - this function will start after setup finishes and then repeat
 * we set which LEDs we want on then call a routine which sends the states to the 74HC595
 */
void loop()
{
    for(int i = 0; i < 8; i++)
    {
        updateLEDs(bits[i]);
        delay(delayTime); 
    }
}



/*
 * updateLEDs() - sends the LED states set in ledStates to the 74HC595
 * sequence
 */
void updateLEDs(int value)
{
    digitalWrite(latch, LOW);     //Pulls the chips latch low
    shiftOut(data, clock, MSBFIRST, value); //Shifts out the 8 bits to the shift register
    digitalWrite(latch, HIGH);   //Pulls the latch high displaying the data
}
