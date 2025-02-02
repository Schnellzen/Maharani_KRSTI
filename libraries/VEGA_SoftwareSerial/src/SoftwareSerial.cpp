#define _DEBUG 0
#define _DEBUG_PIN1 11
#define _DEBUG_PIN2 13
// 
// Includes
// 
// #include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <util/delay_basic.h>
#include <UARTClass.h>

//
// Statics
//
SoftwareSerial *SoftwareSerial::active_object = 0;
uint8_t SoftwareSerial::_receive_buffer[_SS_MAX_RX_BUFF]; 
volatile uint8_t SoftwareSerial::_receive_buffer_tail = 0;
volatile uint8_t SoftwareSerial::_receive_buffer_head = 0;


//
// Private methods
//

/* static */ 
__attribute__ ((noinline))
void SoftwareSerial::tunedDelay(unsigned long delay){
    unsigned long start_mcycle = read_csr(mcycle);
    do { } while (((read_csr(mcycle) - start_mcycle)*0.009) < delay);
} 
// This function sets the current object as the "listening"
// one and returns true if it replaces another 
bool SoftwareSerial::listen()
{
  // if (!_rx_delay_stopbit)
  //   return false;

  if (active_object != this)
  {
    if (active_object)
      active_object->stopListening();

    _buffer_overflow = false;
    _receive_buffer_head = _receive_buffer_tail = 0;
    active_object = this;

    // setRxIntMsk(true);
    return true;
  }

  return false;
}

// Stop listening. Returns true if we were actually listening.
bool SoftwareSerial::stopListening()
{
  if (active_object == this)
  {
    // setRxIntMsk(false);
    active_object = NULL;
    return true;
  }
  return false;
}

//
// The receive routine called by the interrupt handler
//
/*
void SoftwareSerial::recv()
{

#if GCC_VERSION < 40302
// Work-around for avr-gcc 4.3.0 OSX version bug
// Preserve the registers that the compiler misses
// (courtesy of Arduino forum user *etracer*)
  asm volatile(
    "push r18 \n\t"
    "push r19 \n\t"
    "push r20 \n\t"
    "push r21 \n\t"
    "push r22 \n\t"
    "push r23 \n\t"
    "push r26 \n\t"
    "push r27 \n\t"
    ::);
#endif  

  uint8_t d = 0;

  // If RX line is high, then we don't see any start bit
  // so interrupt is probably not for us
  if (_inverse_logic ? rx_pin_read() : !rx_pin_read())
  {
    // Disable further interrupts during reception, this prevents
    // triggering another interrupt directly after we return, which can
    // cause problems at higher baudrates.
    setRxIntMsk(false);

    // Wait approximately 1/2 of a bit width to "center" the sample
    tunedDelay(_rx_delay_centering);
    DebugPulse(_DEBUG_PIN2, 1);

    // Read each of the 8 bits
    for (uint8_t i=8; i > 0; --i)
    {
      tunedDelay(_rx_delay_intrabit);
      d >>= 1;
      DebugPulse(_DEBUG_PIN2, 1);
      if (rx_pin_read())
        d |= 0x80;
    }

    if (_inverse_logic)
      d = ~d;

    // if buffer full, set the overflow flag and return
    uint8_t next = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
    if (next != _receive_buffer_head)
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = d; // save new byte
      _receive_buffer_tail = next;
    } 
    else 
    {
      DebugPulse(_DEBUG_PIN1, 1);
      _buffer_overflow = true;
    }

    // skip the stop bit
    tunedDelay(_rx_delay_stopbit);
    DebugPulse(_DEBUG_PIN1, 1);

    // Re-enable interrupts when we're sure to be inside the stop bit
    setRxIntMsk(true);

  }


#if GCC_VERSION < 40302
// Work-around for avr-gcc 4.3.0 OSX version bug
// Restore the registers that the compiler misses
  asm volatile(
    "pop r27 \n\t"
    "pop r26 \n\t"
    "pop r23 \n\t"
    "pop r22 \n\t"
    "pop r21 \n\t"
    "pop r20 \n\t"
    "pop r19 \n\t"
    "pop r18 \n\t"
    ::);
#endif
}
  */
uint8_t SoftwareSerial::rx_pin_read()
{
  return *_receivePortRegister & _receiveBitMask;
}

//
// Interrupt handling
//

/* static */
inline void SoftwareSerial::handle_interrupt()
{
  if (active_object)
  {
    // active_object->recv();
  }
}

#if defined(PCINT0_vect)
ISR(PCINT0_vect)
{
  SoftwareSerial::handle_interrupt();
}
#endif

#if defined(PCINT1_vect)
ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT2_vect)
ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));
#endif

#if defined(PCINT3_vect)
ISR(PCINT3_vect, ISR_ALIASOF(PCINT0_vect));
#endif

//
// Constructor
//
SoftwareSerial::SoftwareSerial(uint8_t receivePin, uint8_t transmitPin, bool inverse_logic /* = false */)
{
  setTX(transmitPin);
  setRX(receivePin);
}

//
// Destructor
//
SoftwareSerial::~SoftwareSerial()
{
  // end();
}

void SoftwareSerial::setTX(uint8_t tx)
{
    //Set TX pin as gpio outpu
    pinMode(tx, OUTPUT);
    digitalWrite(tx, HIGH);
    _transmitPin = tx;
}

void SoftwareSerial::setRX(uint8_t rx)
{
    //Set TX pin as gpio input
    pinMode(rx, INPUT);
    _receivePin = rx;
}

uint16_t SoftwareSerial::subtract_cap(uint16_t num, uint16_t sub) {
  if (num > sub)
    return num - sub;
  else
    return 1;
}

//
// Public methods
//

void SoftwareSerial::begin(unsigned long speed)
{
  listen();
  bitWidth = (1000000/speed);
}

void SoftwareSerial::setRxIntMsk(bool enable)
{
    if (enable)
      *_pcint_maskreg |= _pcint_maskvalue;
    else
      *_pcint_maskreg &= ~_pcint_maskvalue;
}

void SoftwareSerial::end()
{
  stopListening();
}


// Read data from buffer
int SoftwareSerial::read()
{
  if (!isListening())
    return -1;

  char byte=0;
	while (1) {	
		while(digitalRead(_receivePin)); //start bit
		delayMicroseconds(bitWidth/2);
		if(digitalRead(_receivePin)!=0)
			continue;
		for (unsigned char i=0; i<8;i++) //Parse bits
		{
			delayMicroseconds(bitWidth);
			byte |= digitalRead(_receivePin)<<i;
		}
		delayMicroseconds(bitWidth);
		if(digitalRead(_receivePin)!=1)// detect stop bit
		 { 
			Serial.print("Error : No stop bit ");
      Serial.println(byte);
		 }
		else
		  break;

	}
  _receive_buffer[_receive_buffer_head] = byte; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return byte;
}

int SoftwareSerial::available()
{

  if (!isListening())
    return 0;

  // return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
  return 1;
}

size_t SoftwareSerial::write(uint8_t b)
{
  // setWriteError();
  digitalWrite(_transmitPin, LOW);
  delayMicroseconds(bitWidth);
  for (unsigned char mask = 0x01; mask; mask <<= 1)
  {
    if(b & mask) {// choose bit
        digitalWrite(_transmitPin,HIGH); // send 1
    }
    else{
        digitalWrite(_transmitPin,LOW); // send 0
    }

    delayMicroseconds(bitWidth);
  }
  digitalWrite(_transmitPin,HIGH); // restore pin to natural state
  delayMicroseconds(1000);
  return 1;
}

int SoftwareSerial::write(char *str)
{
  while(*str!='\0')
	{
		write((char)*str);
		str++;
	}
	return 1;
}


void SoftwareSerial::flush()
{
  // There is no tx buffering, simply return
}

int SoftwareSerial::peek()
{
  if (!isListening())
    return -1;

  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  return _receive_buffer[_receive_buffer_head];
}