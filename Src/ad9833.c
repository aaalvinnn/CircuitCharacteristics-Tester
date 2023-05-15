/* The driver of ad9833 */
/*************************************************************************************
 Title	:   Analog Devices AD9833 DDS Wave Generator Library for STM32 Using HAL Libraries
 Author:    Bardia Alikhan Afshar <bardia.a.afshar@gmail.com>  
 Software:  IAR Embedded Workbench for ARM
 Hardware:  Any STM32 device
*************************************************************************************/
#include "AD9833.h"
// ------------------- Variables ----------------
uint16_t FRQLW = 0;    // MSB of Frequency Tuning Word
uint16_t FRQHW = 0;    // LSB of Frequency Tuning Word
uint32_t  phaseVal=0;  // Phase Tuning Value
uint8_t WKNOWN=0;      // Flag Variable
// -------------------------------- Functions --------------------------------

// ------------------------------------------------ AD9833 Delay
static void AD9833_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 1; i++);
}


// ------------------------------------------------ Software SPI Function
/* word 和 0x8000 相与作为data是否使能1的条件————ad9833数据传输方式是先发送高位 */
void writeSPI(uint16_t word) {
    HAL_GPIO_WritePin(AD9833PORT,AD9833FSYNC,GPIO_PIN_RESET); //set Low
	for (uint8_t i = 0; i < 16 ; i++) {
          if(word & 0x8000) HAL_GPIO_WritePin(AD9833PORT,AD9833DATA,GPIO_PIN_SET);   //bit=1, Set High
		else HAL_GPIO_WritePin(AD9833PORT,AD9833DATA,GPIO_PIN_RESET);        //bit=0, Set Low
    AD9833_Delay();
		HAL_GPIO_WritePin(AD9833PORT,AD9833SCK,GPIO_PIN_RESET);             //Data is valid on falling edge
    AD9833_Delay();
		HAL_GPIO_WritePin(AD9833PORT,AD9833SCK,GPIO_PIN_SET);
		word = word<<1; //Shift left by 1 bit
        }
	HAL_GPIO_WritePin(AD9833PORT,AD9833DATA,GPIO_PIN_RESET);                    //Idle low
    AD9833_Delay();
    HAL_GPIO_WritePin(AD9833PORT,AD9833FSYNC,GPIO_PIN_SET); //set High
}

// ------------------------------------------------ Sets Output Wave Type
void AD9833_SetWave(uint16_t Wave){
  switch(Wave){
  case 0:
  HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_RESET);
    writeSPI(0x2000); // Value for Sinusoidal Wave
    HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_SET);
    WKNOWN=0;
    break;
  case 1:
     HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_RESET);
    writeSPI(0x2028); // Value for Square Wave
    HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_SET);
    WKNOWN=1;
    break;
  case 2:
        HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_RESET);
    writeSPI(0x2002); // Value for Triangle Wave
    HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_SET);
    WKNOWN=2;
    break;
  default:
    break;
  }
}

// ------------------------------------------------ Sets Wave Frequency & Phase (In Degree) In PHASE0 & FREQ0 Registers
void AD9833_SetWaveData(float Frequency,float Phase){   AD9833_Delay();
 // ---------- Tuning Word for Phase ( 0 - 360 Degree )
 if(Phase<0)Phase=0; // Changing Phase Value to Positive
 if(Phase>360)Phase=360; // Maximum value For Phase (In Degree)
 phaseVal  = ((int)(Phase*(4096/360)))|0XC000;  // 4096/360 = 11.37 change per Degree for Register And using 0xC000 which is Phase 0 Register Address
 
 // ---------- Tuning word for Frequency      
long freq=0;
freq=(int)(((Frequency*pow(2,28))/FMCLK)+1); // Tuning Word
FRQHW=(int)((freq & 0xFFFC000) >> 14); // FREQ MSB
FRQLW=(int)(freq & 0x3FFF);  // FREQ LSB 
FRQLW |= 0x4000;
FRQHW |= 0x4000; 
 // ------------------------------------------------ Writing DATA
 HAL_GPIO_WritePin(AD9833PORT,AD9833DATA,GPIO_PIN_SET);
HAL_GPIO_WritePin(AD9833PORT,AD9833SCK,GPIO_PIN_SET);
HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_SET);  
 HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_RESET); //low = selected
    AD9833_Delay();
	writeSPI(0x2100); // enable 16bit words and set reset bit
	writeSPI(FRQLW);
	writeSPI(FRQHW);
        writeSPI(phaseVal);
	writeSPI(0x2000); // clear reset bit 
    AD9833_Delay();
	HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_SET); //high = deselected 
AD9833_SetWave(WKNOWN);   AD9833_Delay();
return;
}

// ------------------------------------------------ Initializing AD9833
void AD9833_Init(uint16_t WaveType,float FRQ,float Phase){
HAL_GPIO_WritePin(AD9833PORT,AD9833DATA,GPIO_PIN_SET); // Set All SPI pings to High
HAL_GPIO_WritePin(AD9833PORT,AD9833SCK,GPIO_PIN_SET);  // Set All SPI pings to High
HAL_GPIO_WritePin(AD9833PORT,AD9833SS,GPIO_PIN_SET);   // Set All SPI pings to High
AD9833_SetWave(WaveType);                              // Type Of Wave 
AD9833_SetWaveData(FRQ,Phase);                         // Frequency & Phase Set
return;
}
