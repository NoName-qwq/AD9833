#ifndef __AD9833_H__
#define __AD9833_H__ 

#include "gpio.h"
#include <stdint.h>


static uint16_t Control_REG = 0x0000;

//时钟速率为25 MHz时， 可以实现0.1 Hz的分辨率；而时钟速率为1 MHz时，则可以实现0.004 Hz的分辨率。
//调整参考时钟修改此处即可。
#define FCLK 25000000	//设置参考时钟25MHz
#define RealFreDat    268435456.0/FCLK//总的公式为 Fout=（Fclk/2的28次方）*28位寄存器的值


#define PI 3.1415926f


#define AD9833_FSYNC_SET()        HAL_GPIO_WritePin(AD_FSYNC_GPIO_Port, AD_FSYNC_Pin, GPIO_PIN_SET)
#define AD9833_FSYNC_RESET()      HAL_GPIO_WritePin(AD_FSYNC_GPIO_Port, AD_FSYNC_Pin, GPIO_PIN_RESET)
#define AD9833_SCLK_SET() 	      HAL_GPIO_WritePin(AD_SCLK_GPIO_Port, AD_SCLK_Pin, GPIO_PIN_SET)
#define AD9833_SCLK_RESET()       HAL_GPIO_WritePin(AD_SCLK_GPIO_Port, AD_SCLK_Pin, GPIO_PIN_RESET)
#define AD9833_SDATA_SET() 	      HAL_GPIO_WritePin(AD_SDATA_GPIO_Port, AD_SDATA_Pin, GPIO_PIN_SET)
#define AD9833_SDATA_RESET()      HAL_GPIO_WritePin(AD_SDATA_GPIO_Port, AD_SDATA_Pin, GPIO_PIN_RESET)


/******************************************************************************/
/* AD9833                                                                    */
/******************************************************************************/
/* 寄存器 */

#define AD9833_REG_CMD		(0 << 14)
#define AD9833_REG_FREQ0	(1 << 14)
#define AD9833_REG_FREQ1	(2 << 14)
#define AD9833_REG_PHASE0	(6 << 13)
#define AD9833_REG_PHASE1	(7 << 13)

/* 命令控制位 */

#define AD9833_B28				(1 << 13)
#define AD9833_HLB				(1 << 12)
#define AD9833_FSEL0			(0 << 11)
#define AD9833_FSEL1			(1 << 11)
#define AD9833_PSEL0			(0 << 10)
#define AD9833_PSEL1			(1 << 10)
#define AD9833_PIN_SW			(1 << 9)
#define AD9833_RESET			(1 << 8)
#define AD9833_SLEEP1			(1 << 7)
#define AD9833_SLEEP12		    (1 << 6)
#define AD9833_OPBITEN		    (1 << 5)
#define AD9833_SIGN_PIB		    (1 << 4)
#define AD9833_DIV2				(1 << 3)
#define AD9833_MODE				(1 << 1)

#define AD9833_OUT_SINUS		0//正弦波 
#define AD9833_OUT_TRIANGLE	    1//三角波
#define AD9833_OUT_MSB			2 //方波
#define AD9833_OUT_MSB2			3



void AD9833_Write_2Bytes(uint8_t* data);
void AD9833_SetControlValue(uint16_t ControlValue);

void AD9833_Reset(void);			//置位AD9833的复位位
void AD9833_ClearReset(void);	    //清除AD9833的复位位

void AD9833_SetFrequency(uint16_t reg, float fout);
void AD9833_SetPhaseRad(uint16_t reg, float rad);

void AD9833_Init(float freq_reg0_out,float freq_reg1_out,float phase_reg0_out,float phase_reg1_out);
void AD9833_Select_OutPut_REGandMODE(uint16_t FSELECT,uint16_t PSELECT,uint16_t OutPut_Mode);

void AD9833_FSELECT(uint16_t FSELECT);
void AD9833_PSELECT(uint16_t PSELECT); 
void AD9833_OutPut_Mode(uint16_t OutPut_Mode);

#endif