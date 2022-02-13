/**
 * @file uart_com.h
 *
 */

/*
 *  Created on: 12 Feb 2022
 *      Author: niuslar
 */

#ifndef UART_COM_H_
#define UART_COM_H_

#include <string.h>
#include "usart.h"

#define UART_TIMEOUT 100

class CUartCom
{
private:
    UART_HandleTypeDef m_huart;
    GPIO_TypeDef *m_uart_de_port = NULL;
    uint16_t m_uart_de_pin;

public:
    CUartCom(UART_HandleTypeDef &huart);
    CUartCom(UART_HandleTypeDef &huart,
             GPIO_TypeDef *uart_de_port,
             uint16_t uart_de_pin);

    // I'm using C style strings because the UART HAL uses (uint8_t*) as
    // parameter
    void sendMessage(const char *msg);
};

#endif /* UART_COM_H_ */
