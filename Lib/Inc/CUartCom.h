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

#include "../etl/string.h"
#include "usart.h"

#define MAX_STRING_SIZE 60
#define UART_TIMEOUT    100

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

    void send(const etl::string<MAX_STRING_SIZE> &msg);
};

#endif /* CUARTCOM_H_ */
