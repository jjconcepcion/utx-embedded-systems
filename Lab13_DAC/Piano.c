// Piano.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano
// Daniel Valvano
// December 29, 2014

// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>


// **************Piano_Init*********************
// Initialize piano key inputs (Port E bits 3-0)
// Input: none
// Output: none
void Piano_Init(void){ 
  unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; // enable clock
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTE_DIR_R &= ~0x0F;          // set PE3-0 as input
  GPIO_PORTE_AFSEL_R &= ~0x0F;        // disable alt func
  GPIO_PORTE_DEN_R |= 0x0F;           // enable digital I/O
  GPIO_PORTE_AMSEL_R &= ~0x0F;        // disable analog mode
  GPIO_PORTE_PCTL_R &= ~0x0000FFFF;   // GPIO PE3-0
}
// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void){
  
  return 0; // remove this, replace with input
}
