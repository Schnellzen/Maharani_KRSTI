#include <Dynamixel2Arduino.h>
#include <SD.h>
#include "MPU9250.h"

#define DXL_SERIAL   Serial1 //OpenCM9.04 EXP Board's DXL port Serial. (Serial1 for the DXL port on the OpenCM 9.04 board)
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = -1; //OpenCM9.04 EXP Board's DIR PIN. (28 for the DXL port on the OpenCM 9.04 board)

//declare every ID of Dynamixel
const uint8_t DXL_KAKI_TALLY = 6;
const uint16_t DXL_KAKI[DXL_KAKI_TALLY] = {22,21,20,23,2,14}; 

int offset_X = 0;

const float DXL_PROTOCOL_VERSION = 1.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);

const uint8_t chipSelect = 7; //CS pin for SD CARD
File Files;
const uint32_t maxFrame = 300; //set max frame, since we used 1 fps, 300 frame == 5 min(s)

MPU9250 mpu;

//This namespace is required to use Control table item names
using namespace ControlTableItem;

void setup() {
  // put your setup code here, to run once:
  
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  Wire.begin(); //sometimes doesn't need to be stated, but just in case when using mpu through I2C connection (dunno why tho)
  while(!DEBUG_SERIAL);

  // Set Port baudrate to 1000000bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);

  uint8_t fi = 0;  
  uint8_t DXL_ID = 0;
  for(fi = 0; fi < DXL_KAKI_TALLY; fi++){
    DXL_ID = DXL_KAKI[fi];
    
    // Get DYNAMIXEL information
    dxl.ping(DXL_ID);
    
    // Turn off torque when configuring items in EEPROM area
    dxl.torqueOff(DXL_ID);
    dxl.setOperatingMode(DXL_ID, OP_POSITION); 
    dxl.torqueOn(DXL_ID);

    // Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
    dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID, 0);
    //DEBUG_SERIAL.println(DXL_ID);
  }

  //// Frame
  if (!SD.begin(chipSelect)) {
    DEBUG_SERIAL.println("SD card initialization failed");
    while (true); //program will stuck here
  }

  bool check = 1;
  uint16_t ii = 0;
  String txt = ".txt";
  while (check==1) { //Checking if the file "frame" is already exist
    txt = ".txt";
    txt = ii + txt;
    check = SD.exists(txt);
    ii++;
    if (ii >= maxFrame) {  // force exiting loop
      check = 0; 
      DEBUG_SERIAL.println("Frame Exceeded Max_Value");
      }
    DEBUG_SERIAL.println(txt);
  }


  fi = 0;  
  for(fi = 0; fi < DXL_KAKI_TALLY; fi++){
    DXL_ID = DXL_KAKI[fi];

    Serial.println(DXL_ID);

    Files = SD.open(txt, FILE_WRITE);
    if (Files) {
      Serial.print("Writing to ");
      Serial.println(txt);
      Files.println(dxl.getPresentPosition(DXL_ID,UNIT_DEGREE));
      // close the file:
      Files.close();
      Serial.println("done.");
    }
    else { // if the file didn't open, print an error:
    Serial.print("error opening");
    Serial.println(txt);
    }

    //checking, by printing whatever is writed on the file
    Files = SD.open(txt);
    if (Files) {
      Serial.println(txt);

      // read from the file until there's nothing else in it:
      while (Files.available()) {
        Serial.write(Files.read());
      }
      // close the file:
      Files.close();
    } 
    else {
      Serial.print("error opening");
      Serial.println(txt);
      }


   }
  
// Serial.println("hehe");
// int i = 0;
// for(i = 0; i < 10; i++) {
// String txt = ".txt";
//  txt = i + txt;
// DEBUG_SERIAL.println (txt);
// File Files = SD.open(txt, FILE_WRITE);
// Files.println(i);
// Files.close();
// }


////////////
// SD.begin(7);
// int Target = 100;
// String txt = ".txt";
// txt = Target + txt;
// File Files = SD.open(txt);
// String buffer = "";
// buffer = Files.readStringUntil('\n');
// DEBUG_SERIAL.println(buffer);
// dxl.setGoalPosition(DXL_ID, buffer.toFloat(), UNIT_DEGREE);

// String Buffer="";
// int no = 1;
// String is = "RECORD.txt";
// String iss = is + no + no;
// DEBUG_SERIAL.println(iss);
//   File Files = SD.open(is, FILE_WRITE); //trust me, no matter if it's camel case or anything, it will be converted to uppercase
//   if (Files) {
//     Serial.print("Writing to test.txt...");
//     Files.println(dxl.getPresentPosition(DXL_ID,UNIT_DEGREE));
//     // close the file:
//     Files.close();
//     Serial.println("done.");
//   }
//   else {
//     // if the file didn't open, print an error:
//     Serial.println("error opening RECORD.txt");
//   }

//   Files = SD.open("RECORD.txt");
//   if (Files) {
//     Serial.println("RECORD.txt:");

//     // read from the file until there's nothing else in it:
//     while (Files.available()) {
//       Serial.write(Files.read());
//     }
//     // close the file:
//     Files.close();
//   } else {
//     // if the file didn't open, print an error:
//     Serial.println("error opening RECORD.txt");
//   }

//   SD.remove("RECORD.txt");
/////

}

void loop() {

  // DEBUG_SERIAL.print(dxl.getPresentPosition(DXL_ID, UNIT_DEGREE));
  // dxl.setGoalPosition(DXL_ID, 180, UNIT_DEGREE);
  // DEBUG_SERIAL.println();

}