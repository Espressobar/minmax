#ifndef BSP_NGDF_CONF_H
#define BSP_NGDF_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"


    /* UART handler declaration */
   extern UART_HandleTypeDef UartHandle;
    /* TIM handler declaration */
   extern TIM_HandleTypeDef  TimHandle;

#ifdef __cplusplus
}
#endif
#endif // BSP_NGDF_CONF_H
