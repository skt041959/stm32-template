#include "stm32f10x.h"

vu32 TimeDelay;
void SysTick_Configuration(void)
{
	if (SysTick_Config(SystemCoreClock/1000))
	{
		while (1);
	}					  
}
void Delay(u32 nms)
{
	TimeDelay=nms;
	while (TimeDelay);
}
	