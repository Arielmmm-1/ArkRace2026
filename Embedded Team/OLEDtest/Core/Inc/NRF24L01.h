#ifndef __NRF24L01_H
#define __NRF24L01_H

#include "stm32f4xx_hal.h"    
#include "NRF24L01_Define.h"  

/* 遥控器按键判断宏（从 KEY0/KEY1 数据提取按键） */
#define K1_PRESSED(KEY0)  ((KEY0) & 0x01)
#define K2_PRESSED(KEY0)  ((KEY0) & 0x02)
#define K3_PRESSED(KEY0)  ((KEY0) & 0x04)
#define K4_PRESSED(KEY0)  ((KEY0) & 0x08)
#define K5_PRESSED(KEY0)  ((KEY0) & 0x10)
#define K6_PRESSED(KEY0)  ((KEY0) & 0x20)
#define K7_PRESSED(KEY0)  ((KEY0) & 0x40)
#define K8_PRESSED(KEY0)  ((KEY0) & 0x80)

#define K9_PRESSED(KEY1)  ((KEY1) & 0x01)
#define K10_PRESSED(KEY1) ((KEY1) & 0x02)
#define K11_PRESSED(KEY1) ((KEY1) & 0x04)
#define K12_PRESSED(KEY1) ((KEY1) & 0x08)

/*外部可调用全局数组***********/
extern uint8_t NRF24L01_TxAddress[];
extern uint8_t NRF24L01_TxPacket[];
extern uint8_t NRF24L01_RxAddress[];
extern uint8_t NRF24L01_RxPacket[];

/***********外部可调用全局数组*/

/*函数声明*********************/
/*指令实现*/
uint8_t NRF24L01_ReadReg(uint8_t RegAddress);
void NRF24L01_ReadRegs(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count);
void NRF24L01_WriteReg(uint8_t RegAddress, uint8_t Data);
void NRF24L01_WriteRegs(uint8_t RegAddress, uint8_t *DataArray, uint8_t Count);
void NRF24L01_ReadRxPayload(uint8_t *DataArray, uint8_t Count);
void NRF24L01_WriteTxPayload(uint8_t *DataArray, uint8_t Count);
void NRF24L01_FlushTx(void);
void NRF24L01_FlushRx(void);
void NRF24L01_GPIO_Init(void);
uint8_t NRF24L01_ReadStatus(void);

/*功能函数*/
void NRF24L01_PowerDown(void);
void NRF24L01_StandbyI(void);
void NRF24L01_Rx(void);
void NRF24L01_Tx(void);

void NRF24L01_Init(void);
uint8_t NRF24L01_Send(void);
uint8_t NRF24L01_Receive(void);
void NRF24L01_UpdateRxAddress(void);


/*********************函数声明*/
#endif