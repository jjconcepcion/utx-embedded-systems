// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// January 15, 2016
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"

// Register Definitions
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOE      0x00000010  // port E Clock Gating Control
// Bit-specific Addresses
#define LED                     (*((volatile unsigned long *)0x40024008))
#define SW                      (*((volatile unsigned long *)0x40024004))

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortE_Init(void);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).
int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1, ScopeOn);  // activate grader and set system clock to 80 MHz
  PortE_Init();
  EnableInterrupts();           // enable interrupts for the grader
  while(1){
    if (SW) {                   // toggle LED if switch pressed, else turn LED on
      LED ^= 0x02;
    } else {
      LED = 0x02;
    }
  }
}

// Subroutine to initialize Port E:
// PE0 pin as input, and PE1 pin as output
// Inputs: None
// Outputs: None
void PortE_Init(void) {
  unsigned long volatile delay;

  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE;   // Turn on clock for Port E
  delay = SYSCTL_RCGC2_R;                 // Allow time for clock to start
  GPIO_PORTE_AMSEL_R &= ~0x03;            // Disable analog mode
  GPIO_PORTE_AFSEL_R &= ~0x03;            // No alternate function
  GPIO_PORTE_PCTL_R &= ~0x03;             // Configure as GPIO
  GPIO_PORTE_DIR_R &= ~0x01;              // PE0 input
  GPIO_PORTE_DIR_R |= 0x02;               // PE1 output
  GPIO_PORTE_DEN_R |= 0x03;               // Enable digital
  LED = 0x02;                             // Initialize LED ON
}
