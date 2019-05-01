// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

// **************Waveform*********************
// 4-bit 32 element sine wave
const unsigned long N_WAVE_ELEM = 32;
const unsigned char SineWave[N_WAVE_ELEM] =
{8,9,10,11,12,13,14,15,15,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,1,1,2,3,4,5,6,7};
unsigned char Index;    // index varies from 0 to 31

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
  DAC_Init();
  Index = 0;
  NVIC_ST_CTRL_R &= ~0x1;   // disable SysTick during setup
  NVIC_ST_RELOAD_R = 0;     // initial reload value
  NVIC_ST_CURRENT_R = 0;    // write any value to clear current
  NVIC_ST_CTRL_R = 0x7;     // enable SysTick with system clk src and interrups
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period){
// this routine sets the RELOAD and starts SysTick
  NVIC_ST_CTRL_R &= ~0x1;       // disable SysTick
  NVIC_ST_RELOAD_R = (period-1)&0x00FFFFFF; // reload period for tone
  NVIC_ST_CURRENT_R = 0;        // clear current
  NVIC_ST_CTRL_R = 0x7;         // reenable SyStick
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
  NVIC_ST_CTRL_R &= ~0x1;   // disable SysTick
  DAC_Out(0);
}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
  Index = (Index+1)&0x1F;
  DAC_Out(SineWave[Index]);   // output one waveform segment each interrupt
}
