#ifndef B72_H_
#define	B72_H_

#include "OBJ_MODEL.h"

#define	IND_obj_NULL			0x00
#define	IND_obj_END				0xFF
/*-------------------------------------------------*/
#define	ID_NETWORK			0x01
#define	ID_DEVICE			0xFF
#define ID_REMOTE_CNTRL		0x02
#define	FLAG_RX_ALL			0xFF

#define obj_STATUS			  (IND_obj_NULL + 1)	//[������]
#define obj_BOOT_OK			  (IND_obj_NULL + 2)	//[��������� ����� � ������]
#define obj_BOOT_ERROR		  (IND_obj_NULL + 3)	//[������ ����������]
#define obj_DATA_READY		  (IND_obj_NULL + 4)	//[��������� ����� � ������ �������� ��������]
#define obj_DATA_STREAM		  (IND_obj_NULL + 5)	//[������ ��� �������� ��������]
#define obj_FM_ERASE		  (IND_obj_NULL + 6)	//[�������� ������ ��������]
/*-----------------------------------------------------------------------------------------------------------------------\
           name               |      index          |   class     | type     | hw_snap   |    delay    |   Handler        | 
\-----------------------------------------------------------------------------------------------------------------------*/ 
#define _obj_STATUS_init       obj_STATUS           ,IND_obj_CAS  ,obj_soft  ,   NULL     ,NULL        ,NULL
/*--------------------------------------------------------------------------------------------------------------------*/
#define obj_BOOT_OK_init       obj_BOOT_OK          ,IND_obj_SWC  ,obj_soft  ,   NULL     ,NULL        ,NULL
#define obj_BOOT_ERROR_init    obj_BOOT_ERROR       ,IND_obj_SWC  ,obj_soft  ,   NULL     ,NULL        ,NULL
#define obj_DATA_READY_init    obj_DATA_READY       ,IND_obj_SWC  ,obj_soft  ,   NULL     ,NULL        ,NULL
#define obj_DATA_STREAM_init   obj_DATA_STREAM      ,IND_obj_COM  ,obj_soft  ,   NULL     ,NULL        ,NULL
#define obj_FM_ERASE_init      obj_FM_ERASE         ,IND_obj_CAS  ,obj_soft  ,   NULL     ,NULL        ,NULL

/*init struct for obj model setup*/
#define _obj_cofig_	\
{_obj_STATUS_init},{obj_BOOT_OK_init},{obj_BOOT_ERROR_init},{obj_DATA_READY_init},{obj_DATA_STREAM_init},{obj_FM_ERASE_init}
/*--------------------------------------------------------------------------------------*/
			
#endif
