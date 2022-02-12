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

class UartCom
{
private:
    UART_HandleTypeDef m_huart;

public:
    UartCom(UART_HandleTypeDef &huart);

    // I'm using C style strings because the UART HAL uses (uint8_t*) as
    // parameter
    void send_msg(const char *msg);
};

#endif /* UART_COM_H_ */
