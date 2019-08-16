/*************************************************
* File Name          : can.c
* Author             : Tatarchenko S.
* Version            : v 1.0
* Description        : bxCAN  bus module for stm32f1x
*************************************************/
#include "can.h"

/*************************************************
                   Init CAN
*************************************************/
void CAN_init(){
	
	/*enable clock for CAN */
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN;
	
	/*enable clock for AFIO and GPIOA*/	
	RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN|RCC_APB2ENR_AFIOEN);	
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	
	GPIOA->CRH &= ~(GPIO_CRH_MODE8|GPIO_CRH_CNF8);
	GPIOA->CRH |= (GPIO_CRH_MODE8);	//PA8
	
	/*
	  CAN RX on PA11 ; CAN TX on PA12
	  CAN TX - Alternate function push-pull;
	  CAN RX - Input floating / Input pull-up;
	*/
	#if	CAN1_REMAP == 0
		GPIOA->CRH &= ~(GPIO_CRH_MODE11|GPIO_CRH_CNF11);
		GPIOA->CRH |= GPIO_CRH_CNF11_1;					 //PA11
		GPIOA->CRH &= ~(GPIO_CRH_MODE12|GPIO_CRH_CNF12);
		GPIOA->CRH |= (GPIO_CRH_MODE12|GPIO_CRH_CNF12_1);//PA12
	#elif CAN1_REMAP == 1
		GPIOB->CRH &= ~(GPIO_CRH_MODE8|GPIO_CRH_CNF8);
		GPIOB->CRH |= GPIO_CRH_CNF8_1;					 //PB8
		GPIOB->CRH &= ~(GPIO_CRH_MODE9|GPIO_CRH_CNF9);
		GPIOB->CRH |= (GPIO_CRH_MODE9|GPIO_CRH_CNF9_1);	 //PB9
		AFIO->MAPR &= ~AFIO_MAPR_CAN_REMAP_REMAP2;
		AFIO->MAPR |=  AFIO_MAPR_CAN_REMAP_REMAP2;
	#endif	
	
	/*init can module*/
	can_set_mode(can_mode_normal);
	can_id_filter_config();
}

/*************************************************
 global CAN and baudrate config 
*************************************************/
static void can_set_mode(int mode){
	
	/*enter init mode*/
	CAN1->MCR |= CAN_MCR_INRQ;
	while((CAN1->MSR & CAN_MSR_INAK)==0);
	/*
	Time Triggered Communication Mode disable;
	Set the no automatic retransmission disable;
	Set the receive FIFO locked mode disable;
	*/
	CAN1->MCR &=~(CAN_MCR_TTCM|CAN_MCR_NART|CAN_MCR_RFLM);
	/*
	Set the automatic bus-off management enable;
	Set the automatic wake-up mode enable;
	Set the transmit FIFO priority enable;
	*/
	CAN1->MCR |=(CAN_MCR_AWUM|CAN_MCR_TXFP);
	/*set can mode*/
	CAN1->BTR &= ~(CAN_BTR_BRP|CAN_BTR_TS1|CAN_BTR_TS2|CAN_BTR_SJW);
	
	CAN1->BTR |= ((uint32_t)mode<<30)|
				 ((uint32_t)(can_TS1-1)<<16)|
				 ((uint32_t)(can_TS2-1)<<20)|
				  (uint32_t)(can_BRR-1);
	CAN1->MCR &= ~CAN_MCR_INRQ;
}

/*************************************************
 global CAN message filter config
*************************************************/
static void can_id_filter_config(){
	int CAN_FilterNumber = 0;
	uint32_t CAN_FilterIdHigh = 0,CAN_FilterIdLow = 0;
	uint32_t CAN_FilterMaskIdHigh = 0,CAN_FilterMaskIdLow = 0;
	
	/*select filter for config*/
	int filter_number_bit_pos  = (1<<CAN_FilterNumber);
	
	/*srart init*/
	CAN1->FMR |= CAN_FMR_FINIT;
	
	CAN1->FA1R &= ~(uint32_t)filter_number_bit_pos;
	
	/* 32-bit scale for the filter */
	CAN1->FS1R |= filter_number_bit_pos;
	/*id mode*/
	CAN1->FM1R &= ~(uint32_t)filter_number_bit_pos;
	 /*clean filter regs!!!*/
	CAN1->sFilterRegister[CAN_FilterNumber].FR1 = 0;
	CAN1->sFilterRegister[CAN_FilterNumber].FR2 = 0;
	
	
	CAN1->sFilterRegister[CAN_FilterNumber].FR1 = 
    ((0x0000FFFF & CAN_FilterIdHigh) << 16) |
        (0x0000FFFF & CAN_FilterIdLow);
    /* 32-bit mask or Second 32-bit identifier */
    CAN1->sFilterRegister[CAN_FilterNumber].FR2 = 
    ((0x0000FFFF & CAN_FilterMaskIdHigh) << 16) |
        (0x0000FFFF & CAN_FilterMaskIdLow);	
	
	/*assign to FIFO0*/
	CAN1->FFA1R &= ~(uint32_t)filter_number_bit_pos;
	/*filter active*/
	CAN1->FA1R |= (uint32_t)filter_number_bit_pos;
	/*finish init*/ 
	CAN1->FMR &= ~CAN_FMR_FINIT;
	
	#ifndef CAN_IRQ	
		NVIC_EnableIRQ (USB_LP_CAN1_RX0_IRQn);
		__enable_irq ();
	#endif
		CAN1->IER |= (CAN_IER_FMPIE0);
	
}
/*************************************************
 CAN RX handler
*************************************************/
void USB_LP_CAN1_RX0_IRQHandler(){
	CanRxMsg Mes;
	GPIOA->BSRR |=GPIO_BSRR_BS8;
	
	if((CAN1->RF0R & CAN_RF0R_FMP0)!=0){
		CAN_ReceiveMailBox(&Mes,CAN_FIFO0);
		
	}
	/*test*/	
//	if(Mes.Data[0] == 1){
//		GPIOA->BSRR |=GPIO_BSRR_BS8;
//	}
//	if(Mes.Data[0] == 2){
//		GPIOA->BSRR |=GPIO_BSRR_BR8;
//	}
}

//void CAN1_RX1_IRQHandler(){
//CanRxMsg Mes;
//	GPIOA->BSRR |=GPIO_BSRR_BS8;
//	
//	if((CAN1->RF0R & CAN_RF1R_FMP1)!=0){
//		CAN_ReceiveMailBox(&Mes,CAN_FIFO1);
//		
//	}
//}
/*************************************************
 CAN send message function 
*************************************************/
extern uint8_t CAN_Send(CanTxMsg *TX_message){
	
	uint8_t transmit_mailbox = 0;
	/* Select one empty transmit mailbox */
  if ((CAN1->TSR&CAN_TSR_TME0) == CAN_TSR_TME0){
    transmit_mailbox = 0;
  }
  else if ((CAN1->TSR&CAN_TSR_TME1) == CAN_TSR_TME1){
    transmit_mailbox = 1;
  }
  else if ((CAN1->TSR&CAN_TSR_TME2) == CAN_TSR_TME2){
    transmit_mailbox = 2;
  }
  else{
	/*error - no empty mailbox*/  
	return 0;
  }
	/* Set up the Id */
    CAN1->sTxMailBox[transmit_mailbox].TIR &= CAN_TI0R_TXRQ;
    if (TX_message->IDE == CAN_Id_Standard){
      CAN1->sTxMailBox[transmit_mailbox].TIR |= ((TX_message->StdId << 21) | \
                                                  TX_message->RTR);
    }
    else{
      CAN1->sTxMailBox[transmit_mailbox].TIR |= ((TX_message->ExtId << 3) | \
                                                  TX_message->IDE | \
                                                  TX_message->RTR);
    }
	CAN_fillMailBox(TX_message,transmit_mailbox);
    /* Request transmission */
    CAN1->sTxMailBox[transmit_mailbox].TIR |= CAN_TI0R_TXRQ;
	return 1;
}

/*************************************************
 fill CAN TxMailBox routine
*************************************************/
static uint8_t CAN_fillMailBox(CanTxMsg *TX_message,int mailbox){
	
	/* Set up the DLC */
    CAN1->sTxMailBox[mailbox].TDTR &= (uint32_t)0xFFFFFFF0;
    CAN1->sTxMailBox[mailbox].TDTR |= TX_message->DLC;
	
	/* Set up the data field */
	CAN1->sTxMailBox[mailbox].TDLR = 0;
	CAN1->sTxMailBox[mailbox].TDHR = 0;
	CAN1->sTxMailBox[mailbox].TDLR = *((uint32_t*)&TX_message->Data[0]);
	CAN1->sTxMailBox[mailbox].TDHR = *((uint32_t*)&TX_message->Data[4]);
	
	/*add MailBox control variable */
	return 0;
}

/*************************************************
 receive CAN message
*************************************************/
static uint8_t CAN_ReceiveMailBox(CanRxMsg *RX_message,int mailbox){
	
	/* Get the Id */
	RX_message->IDE = (uint8_t)0x04 & CAN1->sFIFOMailBox[mailbox].RIR;
	if (RX_message->IDE == CAN_Id_Standard){
		RX_message->StdId = (uint32_t)0x000007FF & (CAN1->sFIFOMailBox[mailbox].RIR >> 21);
	}
	else{
		RX_message->ExtId = (uint32_t)0x1FFFFFFF & (CAN1->sFIFOMailBox[mailbox].RIR >> 3);
	}
	RX_message->RTR = (uint8_t)0x02 & CAN1->sFIFOMailBox[mailbox].RIR;
	/* Get the DLC */
	RX_message->DLC = (uint8_t)0x0F & CAN1->sFIFOMailBox[mailbox].RDTR;
	/* Get the FMI */
	RX_message->FMI = (uint8_t)0xFF & (CAN1->sFIFOMailBox[mailbox].RDTR >> 8);
  
	/* Get the data field */
	*((uint32_t*)&RX_message->Data[0])= CAN1->sFIFOMailBox[mailbox].RDLR;
	*((uint32_t*)&RX_message->Data[4])= CAN1->sFIFOMailBox[mailbox].RDHR;
	
	/* Release the FIFO */
	switch(mailbox){
		case CAN_FIFO0:
			   CAN1->RF0R |= CAN_RF0R_RFOM0;
			break;
		case CAN_FIFO1:
			 CAN1->RF1R |= CAN_RF1R_RFOM1;
			break;
	}
	return 0;
}
