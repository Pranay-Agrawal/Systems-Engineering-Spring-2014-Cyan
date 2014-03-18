#define n_input 2

int pingPins[n_input];
int potentialPins[n_input];
int sensorData[n_input*2];

void setup() {
    int i = 0;
    for (i = 0; i < n_input; i++) {
        pingPins[i] = i;
        sonarPins[i] = 7 + i;
    }
}

void loop() {
    
}
