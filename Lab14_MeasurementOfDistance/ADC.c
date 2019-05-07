// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 SS3 to be triggered by
// software and trigger a conversion, wait for it to finish,
// and return the result. 
// Daniel Valvano
// January 15, 2016

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2015

 Copyright 2016 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */

#include "ADC.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

// This initialization function sets up the ADC 
// Max sample rate: <=125,000 samples/second
// SS3 triggering event: software trigger
// SS3 1st sample source:  channel 1
// SS3 interrupts: enabled but not promoted to controller
void ADC0_Init(void){ 
  volatile unsigned long delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE; // enable port E clock
  delay = SYSCTL_RCGC2_R;               // allow time for clock to stabilize
  GPIO_PORTE_DIR_R &= ~0x04;            // set PE2 as input
  GPIO_PORTE_AFSEL_R |= 0x04;           // enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;            // disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;           // enable analog mode on PE2
  SYSCTL_RCGC0_R |= SYSCTL_RCGC0_ADC0;  // enable ADC module 0 clock
  delay = SYSCTL_RCGC2_R;
  SYSCTL_RCGC0_R &= ~0x00000300;        // set ADC0 sample speed 125K
  ADC0_SSPRI_R = 0x0123;                // set highest priority for SS3
  ADC0_ACTSS_R &= ~0x00000008;          // disable SS3 for configuration
  ADC0_EMUX_R &= ~0xF000;               // set SS3 software trigger event
  ADC0_SSMUX3_R += 1;                   // set AIN1(PE2) as SS3 input channel
  ADC0_SSCTL3_R = 0x6;                  // clear TS0 and D0; set IE0 and END0
  ADC0_ACTSS_R |= 0x00000008;           // enable SS3
}


//------------ADC0_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
unsigned long ADC0_In(void){
  unsigned long result;
  ADC0_PSSI_R |= ADC_PSSI_SS3;            // initiate sample sequencer 3
  while ((ADC0_RIS_R&ADC_RIS_INR3) == 0); // wait for sample conversion
  result = ADC0_SSFIFO3_R&0xFFF;          // read result
  ADC0_ISC_R = ADC_ISC_IN3;               // acknowledge (write to clear)
  return result;
}
