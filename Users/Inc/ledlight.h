
#ifndef  __LEDLIGHT_H
#define  __LEDLIGHT_H

#include "main.h"

#define  LEDLIGHT_PORT   GPIOC
#define  LEDLIGHT_PIN    GPIO_PIN_7   


void ledLightInit_LL(void);
void ledLightOn_LL(void);
void ledLightOff_LL(void);
void ledLightToggle_LL(void);
void ledLightDisplay(void);

#endif

