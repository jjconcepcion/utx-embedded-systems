// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// January 15, 2016

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Ports_Init(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long);

// ***** 3. Subroutines Section *****

int main(void){ 
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
  Ports_Init();
  SysTick_Init();
  EnableInterrupts();
  while(1){
  }
}

// Subroutine to configure PB5-0, PF3,F1 as output pins, and PE2-0 as input pins
// Inputs: None
// Outputs: None
void Ports_Init(void) {
  volatile long delay;
  SYSCTL_RCGC2_R |= 0x32;             // activate clock for Ports B,E,F
  delay = SYSCTL_RCGC2_R;             // wait for clock
  GPIO_PORTB_CR_R |= 0x3F;            // allow writes to PB5-0
  GPIO_PORTB_AMSEL_R &= ~0x3F;        // disable analog PB5-0
  GPIO_PORTB_AFSEL_R &= ~0x3F;        // regular function PB5-0
  GPIO_PORTB_PCTL_R &= ~0x00FFFFFF;   // GPIO
  GPIO_PORTB_DIR_R |= 0x3F;           // outputs on PB5-0
  GPIO_PORTB_DEN_R |= 0x3F;           // enable digital PB5-0
  GPIO_PORTE_CR_R |= 0x07;            // allow writes to PE2-0
  GPIO_PORTE_AMSEL_R &= ~0x07;        // disable analog PE2-0
  GPIO_PORTE_AFSEL_R &= ~0x07;        // regular function PE2-0
  GPIO_PORTE_PCTL_R &= ~0x00000FFF;   // GPIO
  GPIO_PORTE_DIR_R |= 0x07;           // inputs on PE2-0
  GPIO_PORTE_DEN_R |= 0x07;           // enable digital PE2-0
  GPIO_PORTF_CR_R |= 0x0A;            // allow writes to PF3, PF1
  GPIO_PORTF_AMSEL_R &= ~0x0A;        // disable analog PF3, PF1
  GPIO_PORTF_AFSEL_R &= ~0x0A;        // regular function PF3, PF1
  GPIO_PORTF_PCTL_R &= ~0x0000F0F0;   // GPIO
  GPIO_PORTF_DIR_R |= 0x0A;           // inputs on PF3, PF1
  GPIO_PORTF_DEN_R |= 0x0A;           // enable digital PF3, PF1
}

// Subroutine to configure SysTick system timer
// Inputs: None
// Outputs: None
void SysTick_Init(void) {
  NVIC_ST_CTRL_R = 0;             // disable during setup
  NVIC_ST_RELOAD_R = 0x00FFFFFF;  // max reload value
  NVIC_ST_CURRENT_R = 0;          // writing any value to clear
  NVIC_ST_CTRL_R = 0x00000005;    // enable with core clock
}

// Subroutine to perform busy waiting
// Inputs: delay, unsigned 24-bit integer SysTick counter value
// Outputs: None
void SysTick_Wait(unsigned long delay) {
  NVIC_ST_RELOAD_R = 0x00FFFFFF&(delay-1);
  NVIC_ST_CURRENT_R = 0;                      //  reload current
  while ((NVIC_ST_CTRL_R&0x00010000) == 0) {  // check COUNT flag
  }
}
