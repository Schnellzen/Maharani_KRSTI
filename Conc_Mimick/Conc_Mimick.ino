#include <Dynamixel2Arduino.h>
#include "MPU9250.h"

#define DXL_SERIAL   Serial1 //OpenCM9.04 EXP Board's DXL port Serial. (Serial1 for the DXL port on the OpenCM 9.04 board)
#define DEBUG_SERIAL Serial
const int DXL_DIR_PIN = 28; //OpenCM9.04 EXP Board's DIR PIN. (28 for the DXL port on the OpenCM 9.04 board)


const uint8_t DXL_ID_X = 22;
const uint8_t DXL_ID_Y = 21;

int offset_X = 0;
int offset_Y = 0;

const float DXL_PROTOCOL_VERSION = 1.0;

Dynamixel2Arduino dxl(DXL_SERIAL, DXL_DIR_PIN);
MPU9250 mpu;

//This namespace is required to use Control table item names

//using namespace ControlTableItem;

using ControlTableItem::writeControlTableItem;

  
int xx=270;
int yy=350;

void setup() {
  // put your setup code here, to run once:
  
  // Use UART port of DYNAMIXEL Shield to debug.
  DEBUG_SERIAL.begin(115200);
  Wire.begin();
  while(!DEBUG_SERIAL);

  // Set Port baudrate to 57600bps. This has to match with DYNAMIXEL baudrate.
  dxl.begin(1000000);
  // Set Port Protocol Version. This has to match with DYNAMIXEL protocol version.
  dxl.setPortProtocolVersion(DXL_PROTOCOL_VERSION);
  // Get DYNAMIXEL information
  dxl.ping(DXL_ID_X);
  dxl.ping(DXL_ID_Y);  

  // Turn off torque when configuring items in EEPROM area
  dxl.torqueOff(DXL_ID_X);
  dxl.torqueOff(DXL_ID_Y);
  dxl.setOperatingMode(DXL_ID_X, OP_POSITION);
  dxl.setOperatingMode(DXL_ID_Y, OP_POSITION);  
  dxl.torqueOn(DXL_ID_X);


  dxl.setGoalPosition(DXL_ID_X, xx, UNIT_DEGREE);

  // Limit the maximum velocity in Position Control Mode. Use 0 for Max speed
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID_X, 0); 
  dxl.writeControlTableItem(PROFILE_VELOCITY, DXL_ID_Y, 0);

  //For MPU
  if (!mpu.setup(0x68)) {  // change to your own address
      while (1) {
          Serial.println("MPU connection failed. Please check your connection with `connection_check` example.");
          delay(5000);
      }
  }

}

void loop() {

  mpu.update();

  DEBUG_SERIAL.print("X");
//  DEBUG_SERIAL.print(mpu.getRoll(), 2);


  DEBUG_SERIAL.print(map((mpu.getRoll()),110,-110,325,200));
  xx = (map((mpu.getRoll()),110,-110,325,200));

  dxl.setGoalPosition(DXL_ID_X, xx, UNIT_DEGREE);

  DEBUG_SERIAL.print(", ");
  DEBUG_SERIAL.print(dxl.getPresentPosition(DXL_ID_X, UNIT_DEGREE));
  
  DEBUG_SERIAL.print("   ");

  DEBUG_SERIAL.print("Y");
//  DEBUG_SERIAL.print(mpu.getPitch(), 2);

  DEBUG_SERIAL.print(map((mpu.getPitch()),-60,60,360,267));

  yy=(map((mpu.getPitch()),-60,60,360,267));
  dxl.setGoalPosition(DXL_ID_Y, yy, UNIT_DEGREE);


  DEBUG_SERIAL.print(", ");
//  DEBUG_SERIAL.print(dxl.getPresentPosition(DXL_ID_Y, UNIT_DEGREE));
    
  DEBUG_SERIAL.println();

    // Serial.print("Yaw, Pitch, Roll: ");
    // Serial.print(mpu.getYaw(), 2);
    // Serial.print(", ");
    // Serial.print(mpu.getPitch(), 2);
    // Serial.print(", ");
    // Serial.println(mpu.getRoll(), 2);

//  delay(200);

}