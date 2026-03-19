#include "ad9833.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>


// ==========================================类软件SPI底层通信==========================================
void AD9833_Write_2Bytes(uint8_t* data){
    AD9833_SCLK_SET();
    AD9833_FSYNC_SET();
    AD9833_FSYNC_RESET();
    for (uint8_t i = 0; i < 2; i++) {
        for (uint8_t j = 0; j < 8; j++) {
            if (data[i] & (0x80 >> j)) {
                AD9833_SDATA_SET();
            } else {
                AD9833_SDATA_RESET();
            }
            AD9833_SCLK_RESET();
            AD9833_SCLK_SET();
        }
    }
    AD9833_SDATA_SET();
    AD9833_FSYNC_SET();
}

void AD9833_SetControlValue(uint16_t ControlValue){
    uint8_t data[2];
    data[0] = (ControlValue >> 8) & 0xFF;
    data[1] = ControlValue & 0xFF;
    AD9833_Write_2Bytes(data);
}


// ==========================================AD9833底层函数==========================================
void AD9833_Reset(void){
    Control_REG |= AD9833_RESET; // RESET=1
	AD9833_SetControlValue(Control_REG);
}
void AD9833_ClearReset(void)
{
    Control_REG &= ~AD9833_RESET; // RESET=0
	AD9833_SetControlValue(Control_REG);
}

void AD9833_SetFrequency(uint16_t reg, float fout)
{
    if(reg != AD9833_REG_FREQ0 && reg != AD9833_REG_FREQ1)return;
	uint16_t freqHi = reg;
	uint16_t freqLo = reg;
    uint32_t val = (uint32_t)((fout * (1ULL << 28)) / FCLK);
    freqHi |= (val >> 14) & 0x3FFF;
	freqLo |= (val & 0x3FFF);

	AD9833_SetControlValue(freqLo);
	AD9833_SetControlValue(freqHi);
}

void AD9833_SetPhaseRad(uint16_t reg, float rad)
{
    uint16_t val;
    
    while(rad < 0) rad += 2 * PI;
    while(rad >= 2 * PI) rad -= 2 * PI;
    
    uint16_t phase_val = (uint16_t)(rad * 4096.0f / (2 * PI));
    
    phase_val &= 0x0FFF;
    
    val = reg | phase_val;
    
    AD9833_SetControlValue(val);
}


// ==========================================AD9833操作函数==========================================
void AD9833_Init(float freq_reg0_out,float freq_reg1_out,float phase_reg0_out,float phase_reg1_out)  
{
    AD9833_Reset();

    Control_REG |= AD9833_B28;
	AD9833_SetControlValue(Control_REG);

    AD9833_SetFrequency(AD9833_REG_FREQ0,freq_reg0_out);
    AD9833_SetFrequency(AD9833_REG_FREQ1,freq_reg1_out);
    AD9833_SetPhaseRad(AD9833_REG_PHASE0, phase_reg0_out);
    AD9833_SetPhaseRad(AD9833_REG_PHASE1, phase_reg1_out);

    AD9833_ClearReset();
}

void AD9833_Select_OutPut_REGandMODE(uint16_t FSELECT,uint16_t PSELECT,uint16_t OutPut_Mode){
    if (FSELECT == AD9833_FSEL0) {
        Control_REG &= ~AD9833_FSEL1;
    } else {
        Control_REG |= AD9833_FSEL1;
    }
    if (PSELECT == AD9833_PSEL0) {
        Control_REG &= ~AD9833_PSEL1;
    } else {
        Control_REG |= AD9833_PSEL1;
    }
    // 先清
    Control_REG &= ~(AD9833_OPBITEN | AD9833_MODE | AD9833_DIV2);
    switch (OutPut_Mode) {
        case AD9833_OUT_SINUS:
        break;
        case AD9833_OUT_TRIANGLE:
        Control_REG |= AD9833_MODE;         
        break;
        case AD9833_OUT_MSB2:
        Control_REG |= AD9833_OPBITEN;
        break;
        case AD9833_OUT_MSB:
        Control_REG |= AD9833_OPBITEN;
        Control_REG |= AD9833_DIV2;
        break;    
    }    
    AD9833_SetControlValue(Control_REG);
}

void AD9833_FSELECT(uint16_t FSELECT){ 
    if (FSELECT == AD9833_FSEL0) {
        Control_REG &= ~AD9833_FSEL1;
    } else {
        Control_REG |= AD9833_FSEL1;
    }
    AD9833_SetControlValue(Control_REG);

}

void AD9833_PSELECT(uint16_t PSELECT){ 
    if (PSELECT == AD9833_PSEL0) {
        Control_REG &= ~AD9833_PSEL1;
    } else {
        Control_REG |= AD9833_PSEL1;
    }
    AD9833_SetControlValue(Control_REG);

}

void AD9833_OutPut_Mode(uint16_t OutPut_Mode){ 
    Control_REG &= ~(AD9833_OPBITEN | AD9833_MODE | AD9833_DIV2);
    switch (OutPut_Mode) {
        case AD9833_OUT_SINUS:
        break;
        case AD9833_OUT_TRIANGLE:
        Control_REG |= AD9833_MODE;         
        break;
        case AD9833_OUT_MSB2:
        Control_REG |= AD9833_OPBITEN;
        break;
        case AD9833_OUT_MSB:
        Control_REG |= AD9833_OPBITEN;
        Control_REG |= AD9833_DIV2;
        break;    
    }
    AD9833_SetControlValue(Control_REG);

}