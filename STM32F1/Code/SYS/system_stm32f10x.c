/*************************************************
* File Name          : system_stm32f10x.c
* Author             : Tatarchenko S.
* Version            : v 1.1
* Description        : SYSCLK and IWDG config for MCU
*************************************************/
#include "system_stm32f10x.h"
/*-----------local variables--------------------*/
uint32_t SystemCounter =0, HSEStatus=0; 
/*************************************************
System init whith 72Mhz PLL as SYSCLK
*************************************************/
void SystemInit(void) {

	
	RCC->CFGR &= (uint32_t)0xf0ff0000;
	RCC->CR   &= (uint32_t)0xfef6ffff;
	RCC->CR   &= (uint32_t)0xFFFBFFFF;
	RCC->CFGR &= (uint32_t)0xFF80FFFF;
	RCC->CIR   = (uint32_t)0x009F0000;
	
	/*Enable HSE*/
	RCC->CR |=(uint32_t)RCC_CR_HSEON; 
	do{
		HSEStatus=RCC->CR & RCC_CR_HSERDY;
		SystemCounter++;
	}
	while((HSEStatus==0) && (SystemCounter!= HSEStartUp_TimeOut));
	if((RCC->CR & RCC_CR_HSERDY)!= RESET){
			HSEStatus=0x01;
			SystemCounter = 0;
	}
	else{
		HSEStatus=0x00;
	}
	if(HSEStatus == (uint32_t)0x01) {
			/* Flash memory config*/
			FLASH->ACR |= (FLASH_ACR_PRFTBE|FLASH_ACR_LATENCY_2);
			
			/*clock config for APB1/2*/
			RCC->CFGR |= (RCC_CFGR_HPRE_DIV1|RCC_CFGR_PPRE1_DIV2|RCC_CFGR_PPRE2_DIV1);
			/*clock config for ADC1 12 MHz max
		      HSE oscillator clock selected as PLL input clock
	 	      72 Mhz PLL clock  when HSE is 12 MHz*/
			RCC->CFGR |= (RCC_CFGR_ADCPRE_DIV8|RCC_CFGR_PLLSRC|RCC_CFGR_PLLMULL6);		
			/*PLL On*/
			RCC->CR |= RCC_CR_PLLON;
			while((RCC->CR & RCC_CR_PLLRDY) == 0){};		
			/* use PLL  as a system clock*/
			RCC->CFGR |= RCC_CFGR_SW_PLL;    
			/* Wait till HSE is used as system clock source */
			while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL){}
	}
	else{
	/*HSE not working*/
	}
	watchdog_config();
}
void watchdog_config(){
	
	/* WD access enable */
	IWDG->KR = 0x5555;
	/*WD prescaler = 16, clock = 40 kHz/16 = 2,5 kHz
		1) 100 ms = 10 Hz, 2500/10 = 250
		2) 500 ms = 2 Hz   2500/2  = 1250
	*/
	IWDG->PR |= IWDG_PR_PR_1; /*010 - divider /16*/
//	IWDG->RLR = 250;		/*reset CPU after 100 ms*/
	IWDG->RLR = 1250;		/*reset CPU after 500 ms*/	
	
	IWDG_RELOAD;
	/*enable debug*/
	DBGMCU->CR |= DBGMCU_CR_DBG_IWDG_STOP;
	/*WD enable*/
	IWDG->KR = 0xCCCC;
	
}

