#include <SoftwareSerial.h>

// Set up a new SoftwareSerial object with RX in digital pin 10 and TX in digital pin 11
SoftwareSerial mySerial(10, 11);

void setup() {
    // Set the baud rate for the SerialSoftware object
    mySerial.begin(9600);
}

void loop() {
    // Send a byte with the value 45
    mySerial.write(65);

    //Send the string “hello” and return the length of the string.
    int bytesSent = mySerial.write("hello");
    delay(1000);
}