#define N_INPUT 2
#define APIN_START 0
#define DPIN_START 7

#include <SoftwareSerial.h>

int potential_pins[N_INPUT];
int ping_pins[N_INPUT];
int potential_read[N_INPUT];
int ping_read[N_INPUT];
int volumns[N_INPUT];
int notes[N_INPUT];

int i = 0;

// ping sensor variables
int duration, cm;

// Music shield variables
SoftwareSerial mySerial(2, 3); // RX, TX
byte note = 0; //The MIDI note value to be played
byte resetMIDI = 4; //Tied to VS1053 Reset line
byte ledPin = 13; //MIDI traffic inidicator
int  instrument = 0;

void setup() {
    for (i = 0; i < N_INPUT; i++) {
        potential_pins[i] = APIN_START + i;
        ping_pins[i] = DPIN_START + i;
    }

    //Setup soft serial for MIDI control
    mySerial.begin(31250);

    //Reset the VS1053
    pinMode(resetMIDI, OUTPUT);
    digitalWrite(resetMIDI, LOW);
    delay(100);
    digitalWrite(resetMIDI, HIGH);
    delay(100);
    talkMIDI(0xB0, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)
}

void loop() {
    getVolumns();
    getNotes();
    playMusic();
}

void getVolumns() {
    for (i = 0; i < N_INPUT; i++) {
        potential_read[i] = analogRead(potential_pins[i]);
        volumns = map(potential_read[i], 0, 1024, 0, 128);
    }
}

void getNotes() {
    for (i = 0; i < N_INPUT; i++) {
        ping_read[i] = getRange(ping_pins[i]);
        notes[i] = map(ping_read[i], 10, 60, 60, 90);
    }
}

int getRange(int pingPin) {
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);
    return duration / 29 / 2;


void playMusic() {
    talkMIDI(0xB0, 0, 0x79); //Bank select melodies

    //For this bank 0x78, the instrument does not matter, only the note
    for(i = 0 ; i < N_INPUT ; i++) {
        talkMIDI(0xB0, 0x07, volumns[i]);  // Set channel volumn

        talkMIDI(0xC0, 27 + 5*i, 0); //Set instrument number. 0xC0 is a 1 data byte command
        //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
        noteOn(0, notes[i], 60);
        delay(50);

        //Turn off the note with a given off/release velocity
        noteOff(0, notes[i], 60);
        delay(50);
    }
}

//Send a MIDI note-on message.  Like pressing a piano key
//channel ranges from 0-15
void noteOn(byte channel, byte note, byte attack_velocity) {
    talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
    talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
    digitalWrite(ledPin, HIGH);
    mySerial.write(cmd);
    mySerial.write(data1);

    //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
    //(sort of: http://253.ccarh.org/handout/midiprotocol/)
    if( (cmd & 0xF0) <= 0xB0)
        mySerial.write(data2);

    digitalWrite(ledPin, LOW);
}
