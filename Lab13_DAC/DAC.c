// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

// **************DAC_Init*********************
// Initialize 4-bit DAC (Port B bits 3-0)
// Input: none
// Output: none
void DAC_Init(void){
  unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB; // enable clock
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTB_DIR_R |= 0x0F;         // set PE3-0 as output
  GPIO_PORTB_AFSEL_R &= ~0x0F;      // disable alt func
  GPIO_PORTB_DR8R_R |= 0x0F;        // set 8mA drive strength PB3-0
  GPIO_PORTB_DEN_R |= 0x0F;         // enable digital I/O
  GPIO_PORTB_AMSEL_R &= ~0x0F;      // disable analog mode
  GPIO_PORTB_PCTL_R &= ~0x0000FFFF; // GPIO PB3-0
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
  
}
