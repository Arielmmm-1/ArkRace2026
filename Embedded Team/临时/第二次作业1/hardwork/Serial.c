#include "stm32f10x.h"                  // Device header

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h> // 需要用到 atoi 函数

//引脚B10和11，A9和10，A2和3
extern volatile int32_t num;

#define RX_BUF_SIZE     128
#define CMD_NUM_MAX     10          // 支持的指令数量

// 命令结构体
typedef struct {
    char cmdStr[32];                // 指令字符串（包含\r\n）
    void (*cmdFunc)(void);          // 对应的执行函数,声明了一个名为 cmdFunc的指针变量，它可以指向任何一个"没有返回值
} CommandEntry;						//、没有参数"的函数

//串口1独立缓冲
uint8_t rx1Buffer[RX_BUF_SIZE];
uint16_t rx1Index = 0;
//串口2独立缓冲
uint8_t rx2Buffer[RX_BUF_SIZE];
uint16_t rx2Index = 0;
//串口3独立缓冲
uint8_t rx3Buffer[RX_BUF_SIZE];
uint16_t rx3Index = 0;
// 定义一个标志位，表示是否收到了设置 num 的命令
volatile uint8_t cmdSetNumFlag = 0;


void Serial_Init(void)
{
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;//波特
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//流控
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长
	USART_Init(USART1,&USART_InitStructure);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART1,ENABLE);
}
void Serial2_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;//TX，复用推挽输出
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//RX,浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;//波特
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//流控
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长
	USART_Init(USART2,&USART_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART2,ENABLE);
}
void Serial3_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//RX,浮空输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate=9600;//波特
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//流控
	USART_InitStructure.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
	USART_InitStructure.USART_Parity=USART_Parity_No;//校验位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;//停止位
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;//字长
	USART_Init(USART3,&USART_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel=USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=3;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_Cmd(USART3,ENABLE);
}



//调用函数发送数据
void Serial2_SenByte(uint8_t Byte)
{
	USART_SendData(USART2,Byte);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
}
void Serial2_SendString(char*String)//字符串
{
	uint8_t i;
	for(i=0;String[i]!='\0';i++)
	{
	Serial2_SenByte(String[i]);	
	}	
}

void USART3_SendByte(uint8_t data)
{
    USART_SendData(USART3, data);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void USART3_SendString(char *str)
{
    while (*str)
    {
        USART3_SendByte((uint8_t)*str++);
    }
}

//print封装
int fputc(int ch,FILE *f)
{
	Serial2_SenByte(ch);
	return ch;
}




// ========== 你要的操作函数 ==========
void Cmd_NumDown(void)
{
    // static int count = 100;
	if(num < 0) num = 0;
	num--;
}

void Cmd_NumUp(void)
{
    // static int count = 100;
	if(num > 3) num = 3;
	num++;
}

void Cmd_Unknown(void)
{
    printf("未知指令\r\n");
}




// 命令表 - 在这里添加你的所有指令
const CommandEntry cmdTable[CMD_NUM_MAX] = {
    {"num--\r\n",   Cmd_NumDown},// Cmd_NumDown 是一个函数名
    {"num++\r\n",   Cmd_NumUp},//Cmd_NumDown不加括号，代表的是函数的地址，而不是调用它。
};

const uint8_t cmdCount = sizeof(cmdTable) / sizeof(cmdTable[0]);





//命令解析函数,判断是否接受到
void ParseCommand(uint8_t *buf, uint16_t len)
{
    uint8_t i;
    
    // 遍历命令表进行匹配
    for (i = 0; i < cmdCount; i++)
    {
        // 使用 strcmp 精确匹配（包括 \r\n）
        if (strcmp((char*)buf, cmdTable[i].cmdStr) == 0)
        {
            // 找到匹配的命令，执行对应函数
            if (cmdTable[i].cmdFunc != NULL)
            {
                cmdTable[i].cmdFunc();
            }
            return;
        }
    }
    
    // 没有匹配到任何命令
    Cmd_Unknown();
}
	


//中断
void USART1_IRQHandler(void)
{
	
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
	uint8_t temp = USART_ReceiveData(USART1); // 读取接收到的数据
	  // 将接收到的数据存入缓冲区，同时防止数组越界
        if (rx1Index < RX_BUF_SIZE - 1)
        {
            rx1Buffer[rx1Index++] = temp;
            rx1Buffer[rx1Index] = '\0'; // 每次存入都补个结束符，方便字符串比较
        }
        else
        {
            // 缓冲区满了，重置索引
            rx1Index = 0;
        }

        // --- 核心解析逻辑 ---
        // 检查缓冲区是否包含了 "#led = " 这个前缀
        // strstr 会查找子串，如果找到了就返回非NULL值
        if (strstr((char*)rx1Buffer, "#led = ") != NULL)
        {
            // 找到前缀后，提取后面的数字
            char *numPtr = strstr((char*)rx1Buffer, "#led = ") + strlen("#led = ");
            
            // atoi 可以将字符串转换为整数 (ASCII to Integer)
            num = atoi(numPtr);
            
            // 清除标志位（如果有其他操作需要用到的话）
            cmdSetNumFlag = 1; 
            
            // 清空缓冲区，准备接收下一帧数据（可选，视你的连续接收需求而定）
            memset(rx1Buffer, 0, RX_BUF_SIZE);
            rx1Index = 0;
            
        }	
	USART_ClearITPendingBit(USART1,	USART_IT_RXNE);	
	}
	
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART2);
        
        // 将收到的数据存入缓冲区
        if (rx2Index < RX_BUF_SIZE - 1)
        {
            rx2Buffer[rx2Index++] = data;
            rx2Buffer[rx2Index] = '\0';  // 确保字符串结束
            
            // 检查是否收到完整的命令（以 \n 结尾）
            if (data == '\n')
            {
                // 收到完整一行，进行解析
                ParseCommand(rx2Buffer, rx2Index);
                
                // 清空缓冲区准备接收下一条
                rx2Index = 0;
                memset(rx2Buffer, 0, RX_BUF_SIZE);//把 rxBuffer这块内存区域的每一个字节都设置为 0;
            }									//清空接收缓冲区，让缓冲区恢复到初始状态，不残留上一次的数据。
        }
        else
        {
            // 缓冲区溢出处理
            rx2Index = 0;
            memset(rx2Buffer, 0, RX_BUF_SIZE);
        }
        
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        uint8_t data = USART_ReceiveData(USART3);
        
        // 将收到的数据存入缓冲区
        if (rx3Index < RX_BUF_SIZE - 1)
        {
            rx3Buffer[rx3Index++] = data;
            rx3Buffer[rx3Index] = '\0';  // 确保字符串结束
            
            // 检查是否收到完整的命令（以 \n 结尾）
            if (data == '\n')
            {
                // 收到完整一行，进行解析
                ParseCommand(rx3Buffer, rx3Index);
                
                // 清空缓冲区准备接收下一条
                rx3Index = 0;
                memset(rx3Buffer, 0, RX_BUF_SIZE);//把 rxBuffer这块内存区域的每一个字节都设置为 0;
            }									//清空接收缓冲区，让缓冲区恢复到初始状态，不残留上一次的数据。
        }
        else
        {
            // 缓冲区溢出处理
            rx3Index = 0;
            memset(rx3Buffer, 0, RX_BUF_SIZE);
        }
        
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}
