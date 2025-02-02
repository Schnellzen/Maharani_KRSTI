## __VEGA_SoftwareSerial__ by _VEGA-Processor_
 
### About the library

The VEGA_SoftwareSerial library has been developed to allow serial communication on any digital pin of the VEGA ARIES Boards, 
using software to replicate the functionality of the hardware UART. It is possible to have multiple software serial ports with speeds up to 115200 bps.


### More about ARIES Boards

ARIES Board has 2 UART Peripheral which can be use by including "UARTClass.h" file in your application.

Below two lines are required to do so:
```
#include<UARTClass.h>
UARTClass mySerial(1); // where '1' means UART-1 and '2' means UART-2 
```
_Note_: All other fuction is same as SoftwareSerial library.

### Useful links:

 * [ARIES Pinmap](https://vegaprocessors.in/files/PINOUT_ARIES%20V2.0_.pdf)
 * Official site [VEGA Processor](https://vegaprocessors.in/)
 * YouTube channel link [VEGA Processor Youtube](https://www.youtube.com/@VEGAProcessors)
