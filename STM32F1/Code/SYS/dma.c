/*************************************************
* File Name          : dma.c
* Author             : Tatarchenko S.
* Version            : v 1.0
* Description        : DMA configuration
*************************************************/
#include "dma.h"

/*----------- global variables-------------------*/
/*************************************************
Init DMA Channel for USART1 
*************************************************/
void DMA_USART1_Setup(){
	/*DMA1 clock*/
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	/*init DMA for USART1 transmit*/
	
	/*memory increment mode enabled,
	memory\peripheral size size 8 bit
	single mode,memory to peripheral direction*/
	DMA1_Channel4->CCR &= 0;
	DMA1_Channel4->CCR |= DMA_CCR1_MINC|DMA_CCR1_DIR;
	/*peripheral address*/
	DMA1_Channel4->CPAR |= (uint32_t)&(USART1->DR);
	/*pointer to memory address*/
	DMA1_Channel4->CMAR |= (uint32_t)usart_data_transmit_array;
	/*number of data to transfer*/
	DMA1_Channel4->CNDTR = (uint32_t)USART1_DEFAULT_BUF_SIZE;
	/*Transfer complete interrupt enable */
	DMA1_Channel4->CCR |= DMA_CCR1_TCIE;
	
	/*medium priority level */
	DMA1_Channel4->CCR |= DMA_CCR1_PL_1;
	
	/*init DMA for USART1 receive*/
	/*memory increment mode enabled,
	memory\peripheral size size 8 bit
	single mode,peripheral to memory direction*/
	DMA1_Channel5->CCR &= 0;
	DMA1_Channel5->CCR |= DMA_CCR1_MINC;
	/*peripheral address*/
	DMA1_Channel5->CPAR |= (uint32_t)&(USART1->DR);
	/*pointer to memory address*/
	DMA1_Channel5->CMAR |= (uint32_t)&usart_data_receive_array[0];
	/*number of data to transfer*/
	DMA1_Channel5->CNDTR = (uint32_t)USART1_DEFAULT_BUF_SIZE;
	/*medium priority level */
	DMA1_Channel5->CCR |= DMA_CCR1_PL_1;
	/*Transfer complete interrupt enable */
	DMA1_Channel5->CCR |= DMA_CCR1_TCIE;
	
	/*DMA ch5 on*/
//	DMA1_Channel5->CCR |= DMA_CCR1_EN;
	
}
void DMA1_Channel4_IRQHandler(){
	
	#if RTOS_USAGE == TRUE
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*interrupt on transfer complete */
	
		/*return mutex for other tasks to usart transmit*/
	xSemaphoreGiveFromISR(xMutex_USART_BUSY,&xTaskWoken);
	if(xTaskWoken == pdTRUE){
		taskYIELD();
	}
	#endif
	if(DMA1->ISR &= DMA_ISR_TCIF4){
		DMA1->IFCR |= DMA_IFCR_CTCIF4;
	}
	
}
#ifdef TARGET
void DMA1_Channel1_IRQHandler(){
	
	static uint32_t buffer_counter = 0;
	static portBASE_TYPE xTaskWoken = pdFALSE;
	/*DMA channel 1 transfer complete*/
	if(DMA1->ISR &= DMA_ISR_TCIF1){
		DMA1->IFCR |= DMA_IFCR_CTCIF1;
		
		if(buffer_counter < adc_filter_size){
		
		adc_ch1_buffer[buffer_counter] = ADC1_DataArray[0];
		adc_ch2_buffer[buffer_counter] = ADC1_DataArray[1];
		adc_ch3_buffer[buffer_counter] = ADC1_DataArray[2];
		adc_ch4_buffer[buffer_counter] = ADC1_DataArray[3];
		adc_ch5_buffer[buffer_counter] = ADC1_DataArray[4];
		adc_ch6_buffer[buffer_counter] = ADC1_DataArray[5];
		
		buffer_counter ++;	
		}else{
			/*выдача семафора на вызов функции фильтра для каждого канала*/
			buffer_counter = 0;
			xSemaphoreGiveFromISR(FilterReady,&xTaskWoken);
			if(xTaskWoken == pdTRUE){
				taskYIELD();
			}
		}
	}
}
#endif
/*************************************************
Reload DMA Channel 4 
*************************************************/
void DMA_Ch4_Reload(int type,int new_buf_size){
	
	DMA1_Channel4->CCR &= ~DMA_CCR1_EN;
	DMA1_Channel4->CMAR &= ~(uint32_t)0xffffffff;
	
	switch(type){
		case USART_DATA_TYPE1:
			DMA1_Channel4->CMAR |= (uint32_t)usart_data_transmit_array;
			break;
		case USART_DATA_TYPE2:
			DMA1_Channel4->CMAR |= (uint32_t)usart_data_stream;
			break;
		default:
			DMA1_Channel4->CMAR |= (uint32_t)usart_data_transmit_array;
			break;
	}
	DMA1_Channel4->CNDTR = new_buf_size;
	DMA1_Channel4->CCR |= DMA_CCR1_EN;
}


