#ifndef __LED_H 
#define __LED_H

#define LED0_State GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0)
#define LED1_State GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1)

void LED_Init(void);

void LED0_ON(void);

void LED0_OFF(void);

void LED0_Turn(void);

void LED1_ON(void);

void LED1_OFF(void);

void LED1_Turn(void);

#endif
