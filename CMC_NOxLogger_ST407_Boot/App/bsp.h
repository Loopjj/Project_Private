/******************** (C) COPYRIGHT 2014  ******************************
* File Name          : bsp.h
* Author             : Digital Development Team
* Version            : V1.0.0
* Date               : 08/24/2014
* Description        : board configuration
* Programer			 : MKH
*******************************************************************************/


/* Module --------------------------------------------------------------------*/
#ifndef	BSP_H
#define	BSP_H


/* Extern --------------------------------------------------------------------*/
#ifdef BSP_GLOBALS
#define BSP_EXT
#else
#define BSP_EXT extern
#endif

#define DATA_LED(val)		  GPIO_WriteBit(GPIOE,  GPIO_Pin_11, val ? Bit_RESET : Bit_SET)
#define NORMAL_LED(val)	  GPIO_WriteBit(GPIOE,  GPIO_Pin_12, val ? Bit_RESET : Bit_SET)
#define TEMP_LED(val)		  GPIO_WriteBit(GPIOE,  GPIO_Pin_13, val ? Bit_RESET : Bit_SET)
#define HPRESS_LED(val)	  GPIO_WriteBit(GPIOE,  GPIO_Pin_14, val ? Bit_RESET : Bit_SET)
#define LPRESS_LED(val)	  GPIO_WriteBit(GPIOE,  GPIO_Pin_15, val ? Bit_RESET : Bit_SET)
#define HPRESSToggle()    GPIO_ToggleBits(GPIOE,  GPIO_Pin_11)

/* Global functions ----------------------------------------------------------*/
BSP_EXT void BSP_Init(void);
BSP_EXT void BSP_Deinit(void);
BSP_EXT void BSP_md(int addr,int size);
BSP_EXT void System_reset(void);
BSP_EXT INT16U dbg_fsmount_test(void);




#endif

/******************* (C) COPYRIGHT 2014  ***************END OF FILE****/
