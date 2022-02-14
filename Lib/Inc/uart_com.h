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

#include <string>
#include "usart.h"

#define UART_TIMEOUT 100

class CUartCom
{
private:
    UART_HandleTypeDef m_huart;
    GPIO_TypeDef *m_uart_de_port = nullptr;
    uint16_t m_uart_de_pin;

public:
    CUartCom(UART_HandleTypeDef &huart);
    CUartCom(UART_HandleTypeDef &huart,
             GPIO_TypeDef *uart_de_port,
             uint16_t uart_de_pin);

    void sendMessage(const std::string &msg);
};

#endif /* UART_COM_H_ */
