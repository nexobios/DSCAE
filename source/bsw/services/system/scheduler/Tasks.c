/*
 * Tasks.c
 *
 * Created: 6/21/2016 7:25:43 PM
 *  Author: Francisco Martinez
 */ 

#include "Tasks.h"
#include "led.h"
#include "can.h"

void vfnTsk_1ms(void)
{
  vCANSendMsg();
}

void vfnTsk_2msA(void)
{
	
}

void vfnTsk_2msB(void)
{
	
}

void vfnTsk_10ms(void)
{
		static uint8_t u8500ms_Ctr=0;
		
		u8500ms_Ctr++;
		
		if (25 <= u8500ms_Ctr)
		{
			u8500ms_Ctr = 0;
			LED_Toggle( 1 );
		}   
}

void vfnTsk_50ms(void)
{

}

void vfnTsk_100ms(void)
{
  			
}

