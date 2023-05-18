#include "mcp41010.h"
// -------------------------------- Functions --------------------------------

// ------------------------------------------------ MCP41010 Delay
/* 根据传输时间而定 */
static void MCP41010_Delay(void)
{
	uint16_t i;
	for (i = 0; i < 1; i++);
}


//软件模拟SPI发送单字节
//xtemp：发送的字节
void MCP41010_SPI_SendOneByte(uint8_t xtemp){

	uint8_t xnum = 0;
	HAL_GPIO_WritePin(MCP41010_PORT, MCP41010_CS, GPIO_PIN_RESET);	//拉低片选，开始发送
	MCP41010_Delay();
	/* DP_SCK发送8个脉冲，在上升沿时送入数据 */
	for(xnum=0; xnum<8; xnum++)
	{
		//先传高位
		if(xtemp & 0x80)	HAL_GPIO_WritePin(MCP41010_PORT, MCP41010_DATA, GPIO_PIN_SET);
		else	HAL_GPIO_WritePin(MCP41010_PORT, MCP41010_DATA, GPIO_PIN_RESET);
		MCP41010_Delay();
		HAL_GPIO_WritePin(MCP41010_PORT, MCP41010_SCK, GPIO_PIN_SET);	//上升沿传数据
		MCP41010_Delay();
		HAL_GPIO_WritePin(MCP41010_PORT, MCP41010_SCK, GPIO_PIN_RESET);	
		xtemp = xtemp << 1;
	}
	HAL_GPIO_WritePin(MCP41010_PORT, MCP41010_CS, GPIO_PIN_SET);	//拉高片选，停止发送
}

//设置MCP41010的滑动变阻器抽头位置
//xRes：电阻编码值，范围为00H~FFH
//注：00时，W抽头处于抽头B的位置，阻值等于抽头电阻，典型值为52Ω
//注：FF时，W抽头处于抽头A的位置，阻值等为9985.94Ω，抽头始终不会与A端相连
//注：对于10KΩ的器件来说，LSB为39.0625Ω
//注：每次芯片上电后，抽头寄存器的值为0x80，约为5KΩ
void MCP41010_SetRes(uint8_t xRes){
	//发送第一字节，控制方式位
	//0x13。写数据，对电位器0，1操作
	MCP41010_SPI_SendOneByte(0x13);
	
	//发送第二字节，滑动变阻器抽头位
	MCP41010_SPI_SendOneByte(xRes);
}

/* 开启MCP41010 */
void MCP41010_Init(uint8_t xRes){
	HAL_GPIO_WritePin(MCP41010_PORT,MCP41010_CS,GPIO_PIN_SET);   // Set All SPI pings to High
	HAL_GPIO_WritePin(MCP41010_PORT,MCP41010_DATA,GPIO_PIN_SET); // Set All SPI pings to High
	HAL_GPIO_WritePin(MCP41010_PORT,MCP41010_SCK,GPIO_PIN_SET);  // Set All SPI pings to High
	MCP41010_SetRes(xRes);	//设置电阻值
	return ;
}
