/*************************************************
* File Name          : main.c
* Author             : Tatarchenko S.
* Version            : v 1.0
* Description        : main file
*************************************************/
/*----------------------------------------------*/
#include "stm32f10x.h"
#include "usart.h"
#include "dma.h"
#include "OBJ_MODEL.h"
/*----------------------------------------------*/
/*defines*/
#define LED_OFF 		(GPIOC->BSRR |= GPIO_BSRR_BS13);
#define LED_ON 			(GPIOC->BSRR |= GPIO_BSRR_BR13);
/*prototypes*/
void CoreInit(void);
void led_invertor(void);
/*global variables*/

/*----------*/

void CoreInit(){
	
	/* config  LED on PC13*/
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	GPIOC->CRH &= ~(GPIO_CRH_MODE13|GPIO_CRH_CNF13);	
	GPIOC->CRH |= (GPIO_CRH_MODE13_0  | GPIO_CRH_MODE13_1);	
	
	DMA_USART1_Setup();
	/*usart init*/
	usart_init();
	usart_speed(56000);
	
}

void send_usart_message(uint8_t *message,uint32_t buf_size){
	
	if(buf_size <= USART1_DEFAULT_BUF_SIZE){
		memcpy(usart_data_transmit_array,message,buf_size);
		DMA_Ch4_Reload(USART_DATA_TYPE1,buf_size);
	}
	else {
		memcpy(usart_data_stream,message,buf_size);
		DMA_Ch4_Reload(USART_DATA_TYPE2,buf_size);}
}

void led_invertor(void)
{
	if(GPIOC->ODR &= GPIO_ODR_ODR13)
	{
		LED_ON;
	}
	else
	{
		LED_OFF;
	}
}
/*************************************************
					Main program 
*************************************************/
int main(void){
	/*peripherals init*/
	CoreInit();
	
	/*start obj model in boot mode*/
	OBJ_task_init(&task_priority,0);
	
	while(1){
	
	}

}













