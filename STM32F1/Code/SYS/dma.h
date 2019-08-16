/*************************************************
* File Name          : dma.h
* Author             : Tatarchenko S.
* Version            : v 1.0
* Description        : dma.c header
*************************************************/
#ifndef DMA_H
#define DMA_H

#include "stm32f10x.h"
#include "OBJ_MODEL.h"
/*-----------local define-----------------------*/
/**/
#define ADC1_DR_ADDR	((uint32_t)(ADC1_BASE + 0x4C))
#define USART1_DR_ADDR	((uint32_t)(USART1_BASE + 0x04))
#define DMA1_USART_ON	(DMA1_Channel4->CCR |= DMA_CCR1_EN)
#define DMA1_USART_OFF	(DMA1_Channel4->CCR &= ~DMA_CCR1_EN)
/*-----------global function prototypes---------*/
extern void DMA_ADC1_Setup(void);
extern void DMA_USART1_Setup(void);
extern void DMA_Ch4_Reload(int type,int new_buf_size);
/*-----------global variables-------------------*/
#endif
