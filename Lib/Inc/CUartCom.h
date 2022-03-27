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
#include "CGpioWrapper.h"
#include "IComChannel.h"
#include "usart.h"

#define UART_TIMEOUT 100

class CUartCom : public IComChannel
{
public:
    CUartCom(UART_HandleTypeDef *p_huart, const std::string name);
    CUartCom(UART_HandleTypeDef *p_huart,
             const std::string name,
             GPIO_TypeDef *uart_de_port,
             uint16_t uart_de_pin);

    void send(std::string msg);
    bool isCommandAvailable();
    std::string getCommand();

private:
    UART_HandleTypeDef *mp_huart;
    CGpioWrapper m_uart_de_pin;
};

#endif /* CUARTCOM_H_ */
