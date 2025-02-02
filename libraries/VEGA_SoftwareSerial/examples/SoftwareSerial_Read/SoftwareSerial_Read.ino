#include <SoftwareSerial.h>

// Set up a new SoftwareSerial object with RX in digital pin 10 and TX in digital pin 11
SoftwareSerial mySerial(10, 11);

void setup() {
    // Set the baud rate for the SerialSoftware object
    mySerial.begin(9600);
    Serial.begin(9600);
}

void loop() {
    char c = mySerial.read();
    Serial.print(c);
}
