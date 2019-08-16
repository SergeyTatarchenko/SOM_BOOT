/*************************************************
* File Name          : usart.c
* Author             : Kukha M.
* Version            : v 1.0
* Date				 : 11.08.2018
* Description        : 
*************************************************/

#include "usart.h"

void usart_init(void)
{
	/*enable clock for USART1 and GPIOA*/
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;	
	
	/* config PA9 for TX and PA10 for RX */
	/* Clean register */
	GPIOA->CRH &= ~GPIO_CRH_MODE9 ;	//PA9
	GPIOA->CRH &= ~GPIO_CRH_MODE10 ;//PA10
	GPIOA->CRH &= ~GPIO_CRH_CNF9 ;//PA9
	GPIOA->CRH &= ~GPIO_CRH_CNF10 ;//PA10
	/* config register */
	GPIOA->CRH |= GPIO_CRH_CNF9_1| GPIO_CRH_MODE9 ;	//PA9/	
	GPIOA->CRH |= GPIO_CRH_CNF10_1;	//PA10
	
	/* config div USART for clock 9600 kbit/s */
	/* USARTDIV = 9600 */
	/* 
	* baud = f_ck / (16*USARTDIV)
	*
	* f_ck = 72MHz
	* baud = 468,75 
	* 468 -> 1D4
	* 0.75 * 16 = 12 -> C
	* BRR = 0x00001D4C
	*/
	//USART1->BRR = 0x1D4C;		// f_ck = 72MHz, baud 9600
	USART1->BRR = 0x186A;		// f_ck = 60MHz, baud 9600
//	USART_BRR_DIV_Mantissa ;				//  DIV_Mantissa[11:0]
//	USART_BRR_DIV_Fraction ;				//  DIV_Fraction[3:0]
	//USART_BRR |= USART_BRR_DIV_Mantissa;
	
	USART1->CR1 |= USART_CR1_UE				// bit_13	Usart Enable
//				| USART_CR1_M				// bit_12	Word Length 0: 1 Start bit, 8 Data bits, n Stop bit; 	1: 1 Start bit, 9 Data bits, n Stop bit
//				| USART_CR1_WAKE			// bit_11	Wakeup method - 0: Idle Line;	1: Address Mark
//				| USART_CR1_PCE				// bit_10	Parity control enable
//				| USART_CR1_PS				// bit_9	Parity selection - 0: Even parity	1: Odd parity 
//				| USART_CR1_PEIE			// bit_8	PE interrupt enable - 1: A USART interrupt is generated whenever PE=1 in the USART_SR register
//				| USART_CR1_TXEIE			// bit_7	TXE interrupt enable - 1: A USART interrupt is generated whenever TXE=1 in the USART_SR register
//				| USART_CR1_TCIE			// bit_6	Transmission complete interrupt enable - 1: A USART interrupt is generated whenever TC=1 in the USART_SR register
				| USART_CR1_RXNEIE			// bit_5	RXNE interrupt enable - 1: A USART interrupt is generated whenever ORE=1 or RXNE=1 in the USART_SR register
//				| USART_CR1_IDLEIE			// bit_4	IDLE interrupt enable - 1: A USART interrupt is generated whenever IDLE=1 in the USART_SR register
				| USART_CR1_TE				// bit_3	Transmitter enable
				| USART_CR1_RE				// bit_2	Receiver enable	 
//				| USART_CR1_RWU				// bit_1	Receiver wakeup - 0: Receiver in active mode	1: Receiver in mute mode
//				| USART_CR1_SBK				// bit_0	Send break - 0: No break character is transmitted	1: Break character will be transmitted
	;
	
////	USART1->CR2 |= USART_CR2_LINEN			// bit_14	LIN mode enable 0: LIN mode disabled	1: LIN mode enabled
//////				| USART_CR2_STOP_0			// bit_13	Stop bit 00: 1 Stop bit		01: 0.5 Stop bit	10: 2 Stop bits		11: 1.5 Stop bits
//////				| USART_CR2_STOP_1
//				| USART_CR2_CLKEN			// bit_11	Clock enable
//				| USART_CR2_CPOL			// bit_10	Clock polarity	0: Steady low value on CK pin outside transmission window	1: Steady high value on CK pin outside transmission window
//				| USART_CR2_CPHA			// bit_9	Clock phase		0: The first clock transition is the first data capture edge	1: The second clock transition is the first data capture edge.
//				| USART_CR2_LBCL			// bit_8	Last bit clock pulse	0: The clock pulse of the last data bit is not output to the CK pin		1: The clock pulse of the last data bit is output to the CK pin
//				| USART_CR2_LBDIE			// LIN break detection interrupt enable	1: An interrupt is generated whenever LBD=1 in the USART_SR register
//				| USART_CR2_LBDL			// lin break detection length	0: 10 bit break detection	1: 11 bit break detection
				
//				| (USART_CR2_ADD & 0x0001)	// Address of the USART node
	;


	USART1->CR3 |= /*USART_CR3_CTSIE			// bit_10	CTS interrupt enable	1: An interrupt is generated whenever CTS=1 in the USART_SR register
////				|*/ //USART_CR3_CTSE			// bit_9	CTS	enable 	0: CTS hardware flow control disabled
//				| USART_CR3_RTSE			// bit_8	RTS enable	0: RTS hardware flow control disabled
				 USART_CR3_DMAT			// bit_7	DMA enable transmitter
//				| USART_CR3_DMAR			// bit_6	DMA enable receiver
//				| USART_CR3_SCEN			// bit_5	Smartcard mode enable
//				| USART_CR3_NACK			// bit_4	Smartcard NACK enable	0: NACK transmission in case of parity error is disabled	1: NACK transmission during parity error is enabled
//				| USART_CR3_HDSEL			// bit_3	Half-duplex selection
//				| USART_CR3_IRLP			// bit_2	IrDA low-power	0: Normal mode
//				| USART_CR3_IREN			// bit_1	IrDA mode enable
//				| USART_CR3_EIE				// bit_0	Error interrupt enable	1: An interrupt is generated whenever DMAR=1 in the USART_CR3 register and FE=1 or ORE=1 or NE=1 in the USART_SR register.
	;
	
//	USART1->GTPR |= (USART_GTPR_GT & 0x0000) // bit_15 - bit_8 Guard time value
//				| (USART_GTPR_PSC & 0x0000)	// bit_7 - bit_0	Prescaler value
	;

}

void usart_speed(uint32_t usartdiv)
{
	float baud = 0.0;
	uint32_t div_fraction = 0;
	uint32_t div_mantissa = 0;
	uint32_t brr = 0;
	
	baud = (float)PLL_FREQ / (16.0 * (float)usartdiv);	
	
	div_fraction = (int)(baud * 100); 		
	div_fraction = div_fraction % 100;	
	div_fraction = (float)(div_fraction/100.0)*16.0;
	
	div_mantissa = baud * 16;				
	
	brr = div_mantissa | div_fraction;
	
	USART1->BRR = brr;
}

uint8_t	usart_rx(uint8_t *st)
{
//	USART1->CR1 &= ~USART_CR1_TE;	// disable TX
	//USART1->CR1 |= USART_CR1_RE;	// enable RX
	if(USART1->SR & USART_SR_RXNE)	// Read data register not empty
	{
		*st = 1;
		return 	USART1->DR;
	}else{
		*st = 0;
		
	}
//	if(USART1->SR & USART_SR_PE)			// Parity error
//	{
//		return 0;
//	}
	return 	0; 
}

void usart_tx(uint8_t *bytes, uint8_t length)
{
	uint8_t i = 0;
	
//	USART1->CR1 &= ~USART_CR1_RE;	// disable RX
	USART1->CR1 |= USART_CR1_RE;	// enable RX
	USART1->CR1 |= USART_CR1_TE;	// enable TX
	
//	//USART1->CR1 &= ~USART_CR1_PCE;				// bit_10	Parity control enable
//	
//	USART1->CR1 |= USART_CR1_TE;	// enable TX
//	while(!(USART1->SR & USART_SR_TC));	// while flag "transmission complete"
//	USART1->DR = i;
	
	for(i=0 ;i < length; i++)
	{
		while(!(USART1->SR & USART_SR_TC));	 // while flag "transmission complete"
		while(!(USART1->SR & USART_SR_TXE)); // if flag set "Transmit data register empty"
		USART1->DR = *(bytes + i);
//		USART1->SR = USART_SR_TC;			//clear flag "transmission complete"
	}
	
	
}
