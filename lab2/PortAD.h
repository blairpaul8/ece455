#ifndef _PortAD_h
#define _PortAD_h

#include "stdint.h"

enum PushButton { SW1, SW2 };

void PortA_Init(void);
void PortD_Init(void);
uint32_t pushbuttons(enum PushButton button);

#endif
