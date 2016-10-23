#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
//#include "bsp_ngdf_conf.h"
#include "bsp_ngdf_uart.h"
#include <stdint.h>
#include <string.h>

static void sendByte(uint8_t c);
static int hasData(void);
static uint8_t getChar(void);
static void uartReceiveIT(UART_HandleTypeDef* huart);
static uint8_t checkTelegram(void);

UART_HandleTypeDef UartHandle;

/* used by interrupt */
#define BUFFER_SIZE 200
char aRxBuffer[BUFFER_SIZE]; //rename this
static volatile int writecount = 0;
static volatile int readcount = 0;
static uint8_t overrun = 0;
static uint8_t noDataCount = 0;
static uint8_t telegramBuffer[100];
static int telegramIndex = 0;

//public routine
char* runUART(uint32_t* length)
{
    if (hasData())
    {
        uint8_t len;
        noDataCount = 0;
        getChar();
        len = checkTelegram();
        if (len > 0)
        {
            char* p = (char*)&telegramBuffer[2];
            *length = len - 2; //scrap header
            telegramIndex = 0; //clear whole buffer, maybe loosing next package!!
            return p;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        noDataCount++;

        if (noDataCount == 60)
        {
            noDataCount = 0;
            telegramIndex = 0;
        }
    }

    return 0;
}

static uint8_t checkTelegram()
{
    int stx_index = 0;
    while (telegramBuffer[stx_index] != 0xB4 && stx_index < telegramIndex)
    {
        stx_index++;
    }

    if (stx_index == telegramIndex)
    {
        telegramIndex = 0; //no start byte found, scrap all
        return 0;
    }

    //move data so start byte is at the beginning
    if (stx_index > 0)
    {
        memmove(telegramBuffer, telegramBuffer + stx_index, telegramIndex - stx_index);
        telegramIndex -= stx_index;
    }

    //do we have enough data
    if (telegramIndex >= 4) // STX, LEN, CODE WORD, {PAYLOAD WORDS}, 4 bytes is minimum
    {
        uint8_t length = telegramBuffer[1];
        if (length > telegramIndex)
        {
            return 0;
        }
        else
        {
            return length;
        }
    }
    return 0; //nothing yet
}

static uint8_t getChar(void)
{
    int index = 0;
    if(hasData())
    {
        index = readcount;
        readcount = (readcount + 1) % BUFFER_SIZE;
                
        //Populate telegram buffer
        telegramBuffer[telegramIndex] = aRxBuffer[index];
        telegramIndex++;
        
        return aRxBuffer[index];
    }

    return 0;
}


//private implementation

void USART6_IRQHandler(void)
{
    uint32_t tmp1 = __HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_RXNE);
    uint32_t tmp2 = __HAL_UART_GET_IT_SOURCE(&UartHandle, UART_IT_RXNE);
    /* UART in mode Receiver ---------------------------------------------------*/
    if((tmp1 != RESET) && (tmp2 != RESET))
    { 
        uartReceiveIT(&UartHandle);
    }
}

static void Error_Handler(void)
{
    while(1);
}

#define USARTx                           USART6
#define USARTx_CLK_ENABLE()              __USART6_CLK_ENABLE()
#define USARTx_RX_GPIO_CLK_ENABLE()      __GPIOG_CLK_ENABLE()
#define USARTx_TX_GPIO_CLK_ENABLE()      __GPIOG_CLK_ENABLE()

#define USARTx_FORCE_RESET()             __USART6_FORCE_RESET()
#define USARTx_RELEASE_RESET()           __USART6_RELEASE_RESET()

/* Definition for USARTx Pins */
#define USARTx_TX_PIN                    GPIO_PIN_14
#define USARTx_TX_GPIO_PORT              GPIOG
#define USARTx_TX_AF                     GPIO_AF8_USART6
#define USARTx_RX_PIN                    GPIO_PIN_9
#define USARTx_RX_GPIO_PORT              GPIOG
#define USARTx_RX_AF                     GPIO_AF8_USART6

/* Definition for USARTx's NVIC */
#define USARTx_IRQn                      USART6_IRQn
#define USARTx_IRQHandler                USART6_IRQHandler

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{  
  GPIO_InitTypeDef  GPIO_InitStruct;
  
  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  USARTx_TX_GPIO_CLK_ENABLE();
  USARTx_RX_GPIO_CLK_ENABLE();

  /* Enable USARTx clock */
  USARTx_CLK_ENABLE(); 
  
  /*##-2- Configure peripheral GPIO ##########################################*/  
  /* UART TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = USARTx_TX_AF;

  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin = USARTx_RX_PIN;
  GPIO_InitStruct.Alternate = USARTx_RX_AF;

  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);
    
  /*##-3- Configure the NVIC for UART ########################################*/
  /* NVIC for USART */
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
}

static void uartReceiveIT(UART_HandleTypeDef* huart)
{
    //Read into buffer
    aRxBuffer[writecount] = (uint8_t)(huart->Instance->DR); 

    //Move on to next free slot
    writecount = (writecount + 1) % BUFFER_SIZE;                   
                                        
    //If write pointer == read pointer => overrun (reset buffer)
    if(writecount == readcount)
    {
        overrun = 1;        
    }
}

void setupUART()
{
    /*##-1- Configure the UART peripheral ######################################*/
    /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
    /* UART configured as follows:
       - Word Length = 8 Bits
       - Stop Bit = One Stop bit
       - Parity = None
       - BaudRate = 9600 baud
       - Hardware flow control disabled (RTS and CTS signals) */
    UartHandle.Instance        = USART6;
    UartHandle.Init.BaudRate   = 9600;
    UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle.Init.StopBits   = UART_STOPBITS_1;
    UartHandle.Init.Parity     = UART_PARITY_NONE;
    UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle.Init.Mode       = UART_MODE_TX_RX;
    if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }  
    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    } 
    /* Enable the UART Data Register not empty Interrupt */
    __HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
}

void sendUART(const void* data, uint32_t length)
{
    const char* ch = (const char*)data;
    uint32_t i;

	//protocol - HEADER + size + payload
//    sendByte(0xB4);
//    sendByte((length+2) & 0xFF);
    for(i=0; i<length; i++)
    {
        sendByte(ch[i]);
    }
}

static void sendByte(uint8_t c)
{
    while(!__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_TXE )) { };
    UartHandle.Instance->DR = (uint32_t)c;   
    while(!__HAL_UART_GET_FLAG(&UartHandle, UART_FLAG_TC )) { };
}

static int hasData(void)
{
    return (writecount!=readcount);
}
