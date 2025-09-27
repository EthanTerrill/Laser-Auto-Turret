////////////////////////////////////////////////////////////////////////////
//  FileName:     GPIO.h
//  Author:       Ethan Terrill
//  Date:         27.07.2025
//
//  Description:  GPIO.h is a simple header file containg
//                a handfull of useful inline functions
//                for intefacing directly with GPIO pins
//////////////////////////////////////////////////////////////////////


// ============================ function prototypes ============================ //
inline void enable_pin(int pin);




// ============================ Definition ============================ //
inline void enable_pin(int pin) {

  // This predefined macro sets the functionality of a specified pin (to UART, SPI, 
  // GPIO, etc). Here we are setting pin 5 to be a general purpose input/output 
  // pin.
  PIN_FUNC_SELECT(GPIO_PIN_MUX_REG[pin], PIN_FUNC_GPIO);
 
  // Write code here (1-3 lines) to mark pin 5 as output
  // using the GPIO_ENABLE_REG macro
  *((volatile uint32_t *)GPIO_ENABLE_REG) |= (1 << pin);

}
