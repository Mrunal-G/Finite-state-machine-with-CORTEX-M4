#include "stm32L476xx.h"
/* 
//====================================================================
// LED pattern for amount of money collected.
//====================================================================
 50p --> |*| | | | 
100p --> | |*| | |
150p --> |*|*| | |
200p --> | | |*| |
250p --> |*|*|*|*|
//====================================================================
// 250p collected means parking machine vends ticket... demontrated by ligting up all leds.
//====================================================================
*/
#define SW0 1<<1
#define SW1 1<<9
#define SW4 1<<10
int main(void){
int i;
int count = 0; // for sw1 (50 p) increment
int time = 0x5FFFF; // 393215 decimal
int time1 = 100000; // 1 sec delay
uint16_t switchRead;
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // switch on GPIOA clock
//===============================================
// configure outputs 
//===============================================
GPIOA->MODER &= ~(0xFF<<(10)); // clear both bit 5-8 mode bits
GPIOA->MODER |= 0x55<<(10); // Output (01) set bit 5-8 for output
GPIOA-> ODR &= ~(0xF<<5); // reset b8:5
// NOTE : configure pin 1, 3, 9 and 10 
//===============================================
// configure inputs 
//===============================================
GPIOA->MODER &= ~(0x3C000C); // reset bits for pin PA1, PA9 and PA10 
(00=input)
GPIOA->PUPDR &= ~(0x3C000C); 
GPIOA->PUPDR |= 0x140004; // pull-up for bits 1, 9 and 10 (01= pull up)
char state = 0; // check state == 0, 1 and 2 that is check for 3 states.
while(1)
{
//===============================================
// Idle state 
//===============================================Student name: Mrunal Prakash Gavali
if(state == 0)
 {
 count = 0;
 //GPIOA->ODR &= ~(0xF<<5); // reset count value in ODR
 state = 1; // Go to money counting state
 }
//========================================================
// Active state
//========================================================
if (state == 1)
{
 //========================================================
 // Money Counting for 50p with SW1
 //========================================================
//count for 50p controlled by SW1
switchRead = GPIOA->IDR; //// Assign GPIOA state to switchRead
if(!(switchRead & SW1)) // check for SW1 ON condition 
{
for(i=0; i<time; i++); //delay for debouncing
switchRead = GPIOA->IDR;
if(!(switchRead & SW1))
for(i=0; i<time; i++); //delay for debouncing
switchRead = GPIOA->IDR;
if(switchRead & SW1) // counter increment by 1 when SW1 return to 
'off' from 'on'. This is to 
 // ensure the counter increment by 1 once each time 
when SW1 is 'on'. 
{
for(i=0; i<time; i++); // delay for debouncing
switchRead = GPIOA->IDR; 
if(switchRead & SW1)
{
count = count + 1; // increment the counbt by 1 to indicate 50p is inserted
}
}
}
GPIOA->ODR &= ~(0xF<<5); // reset count value in ODR
GPIOA->ODR |= count<<5; // update ODR
//========================================================
 // Money Counting for 100p with SW4
 //========================================================
//count for 100p controlled by SW4
switchRead = GPIOA->IDR;
if(!(switchRead & SW4)) // check for SW4 ON condition 
{
for(i=0; i<time; i++); //delay for debouncing
switchRead = GPIOA->IDR;
if(!(switchRead & SW4))
for(i=0; i<time; i++); //delay for debouncing
switchRead = GPIOA->IDR;
if(switchRead & SW4) // counter increment by 2 when SW4 return to 
'off' from 'on'. This is to 
 // ensure the counter increment by 1 once each time 
when SW4 is 'on'. 
{
for(i=0; i<time; i++); // delay for debouncing
switchRead = GPIOA->IDR; 
if(switchRead & SW4)
{
count = count + 2; // increment the count by 2 to indicate 
100p is inserted
}
}
}
GPIOA->ODR &= ~(0xF<<5); // reset count value in ODR
GPIOA->ODR |= count<<5; // update ODR
//==============================================================
 // Money Counting for wrong coin with push button
 //==============================================================
switchRead = GPIOA->IDR;
if(!(switchRead & SW0)) // check for SW4 ON condition 
{
for(i=0; i<time; i++); //delay for debouncing
switchRead = GPIOA->IDR;
if(!(switchRead & SW0))
for(i=0; i<time; i++); //delay for debouncing
switchRead = GPIOA->IDR;
if(switchRead & SW0) 
{
for(i=0; i<time; i++); // delay for debouncing
switchRead = GPIOA->IDR; 
if(switchRead & SW0)
{
count = count + 0; // do nothing when push button is 
pressed by incrementing count by 0.
}
}
}
GPIOA->ODR &= ~(0xF<<5); // reset count value in ODR
GPIOA->ODR |= count<<5; // update ODR
//============================================================
if(count >= 5) state = 2; // Go to Vend state
}
//===============================================
// Vending state 
//===============================================
if (state == 2)
{
switchRead = GPIOA->IDR;
GPIOA->ODR |= (0xF<<5); // Turn on LEDs
for(i=0; i<time1; i++); //delay for 1 second
GPIOA->ODR &= ~(0xF<<5); // Turn off LEDs
state = 0; // go to Idle state to reset the count value along with value in 
ODR and update ODR
}
} // while loop end
} // main loop end
