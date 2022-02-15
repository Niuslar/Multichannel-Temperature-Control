/**
 * @file CUartCom.h
 *
 */

/*
 *  Created on: 12 Feb 2022
 *      Author: niuslar
 */

#ifndef CUARTCOM_H_
#define CUARTCOM_H_

#include <string>
#include "usart.h"

#define UART_TIMEOUT 100

class CUartCom
{
private:
    UART_HandleTypeDef *mp_huart;
    GPIO_TypeDef *m_uart_de_port = nullptr;
    uint16_t m_uart_de_pin;

public:
    CUartCom(UART_HandleTypeDef *p_huart);
    CUartCom(UART_HandleTypeDef *p_huart,
             GPIO_TypeDef *uart_de_port,
             uint16_t uart_de_pin);

    void sendMessage(const std::string &msg);
};

#endif /* CUARTCOM_H_ */
