#include <SoftwareSerial.h>

// Set up a new SoftwareSerial object with RX in digital pin 10 and TX in digital pin 11
SoftwareSerial portOne(10, 11);

// Set up a new SoftwareSerial object with RX in digital pin 8 and TX in digital pin 9
SoftwareSerial portTwo(8, 9);

void setup() {
    // Set the baud rate for the Serial object
    Serial.begin(9600);

    // Set the baud rate for the SerialSoftware objects
    portOne.begin(9600);
    portTwo.begin(9600);
}

void loop() {
    // Enable SoftwareSerial object to listen
    portOne.listen();
    
    if (portOne.isListening()) {
        Serial.println("portOne is listening!");
    } else {
        Serial.println("portOne is not listening!");
    }

    if (portTwo.isListening()) {
        Serial.println("portTwo is listening!");
    } else {
        Serial.println("portTwo is not listening!");
    }
    delay(1000);
}
