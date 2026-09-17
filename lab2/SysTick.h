#ifndef _SysTick_h
#define _SysTick_h

#include <stdint.h>

void SysTick_Init(void);
void SysTick_Init_Interrupts(void);
void SysTick_Delay1s_16MHz(void);
void SysTick_Delay1s_25MHz(void);
void part_two_b_work(void);

void change_interrupt(uint8_t overlap);

#endif
