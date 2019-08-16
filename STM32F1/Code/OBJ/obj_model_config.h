/*-------------------------------------------------
                SOM global config 
-------------------------------------------------*/
#ifndef OBJ_CONFIG_H_
#define	OBJ_CONFIG_H_

#define RTOS_USAGE	FALSE
#define SOM_MODE	BOOT_MODE
/*-----------------------------------------------
************communication channels***************
-----------------------------------------------*/
#define USART_COM_ENABLE	TRUE
#define CAN_COM_ENABLE    	FALSE
/*-----------------------------------------------
************SOM memory usage config**************
-----------------------------------------------*/

#define HARDWARE_OBJECT FALSE
#define OBJECT_TIMER	FALSE

#define	num_of_all_obj		10

#if RTOS_USAGE == TRUE
	#include "RTOS.h"
#endif
#if RTOS_USAGE == TRUE
	#include "RTOS.h"
#endif

#define USART_DATA_FAST FALSE

#include "BOOT.h"


#endif

