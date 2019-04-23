// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
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


#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

// defines for the bit fields of I/O devices
#define SWITCH                (*((volatile unsigned long *)0x40004020))
#define SOUND                 (*((volatile unsigned long *)0x40004010))

// state names
#define QUIET                 0
#define SWITCH_DOWN           1
#define SWITCH_UP             2
// sound mode
#define TOGGLE                1
// SWITCH low value
#define NOT_PRESSED           0
// SOUND output values
#define SOUND_ON              0x4
#define SOUND_OFF             0

// global declarations section
static volatile unsigned long prev_sw;
static volatile unsigned long state;
static volatile unsigned long input;

struct Sound_State {
  unsigned long Mode;
  unsigned long Next[4];
};

typedef const struct Sound_State S_State;

//                             |-------input-------|
// | # | State       | Mode    | 00 | 01 | 10 | 11 |
// |===============================================|
// | 0 | QUIET       | OFF     |  0 |  1 |  0 |  0 |
// | 1 | SWITCH_DOWN | TOGGLE  |  2 |  1 |  2 |  1 |
// | 2 | SWITCH_UP   | TOGGLE  |  2 |  0 |  2 |  0 |
//  
// note: input_bit1 = logical value of previous switch value
//       input_bit0 = logical value of current switch value
S_State FSM[3] = {
  {SOUND_OFF, {QUIET, SWITCH_DOWN, QUIET, QUIET}},
  {TOGGLE, {SWITCH_UP, SWITCH_DOWN, SWITCH_UP, SWITCH_DOWN}},
  {TOGGLE, {SWITCH_UP, QUIET, SWITCH_UP, QUIET}}
};

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ 
  unsigned long volatile delay;
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;   // enable clock
  delay = SYSCTL_RCGC2_R;
  GPIO_PORTA_DIR_R &= ~0x08;              // set PA3 input
  GPIO_PORTA_DIR_R |= 0x04;               // set PA2 output
  GPIO_PORTA_AFSEL_R &= ~0x0C;            // disable alternate function
  GPIO_PORTA_DR8R_R |= 0x04;              // set 8mA drive stregnth for PA2
  GPIO_PORTA_PDR_R |= 0x08;               // enable pull-down resistor for PA3
  GPIO_PORTA_DEN_R |= 0x0C;               // enable digital I/O
  NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // disable SysTick during setup
  NVIC_ST_RELOAD_R  = 90908;  // reload = int((1/(freq_interrupt*freq_bus))-1)
  NVIC_ST_CURRENT_R = 0;                  // write any value to clear current
  NVIC_ST_CTRL_R  = 0x07;     // enable system clk src, interrupts, systick
}

// called at 880 Hz()
// generates continous 440hz tone on intial switch press which is terminated
// by a following switch press
void SysTick_Handler(void){
  input = prev_sw >> 2 | SWITCH >> 3;
  state = FSM[state].Next[input];
  SOUND = (FSM[state].Mode == TOGGLE) ? SOUND^SOUND_ON : SOUND_OFF;
  prev_sw = SWITCH;
}

int main(void){// activate grader and set system clock to 80 MHz
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();         
  prev_sw = NOT_PRESSED;
  state = QUIET;
  EnableInterrupts();   // enable after all initialization are done
  while(1){
    // main program is free to perform other tasks
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash
  }
}
