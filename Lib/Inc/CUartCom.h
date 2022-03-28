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

#include <queue>
#include <string>
#include "CFifoBuffer.h"
#include "CGpioWrapper.h"
#include "IComChannel.h"
#include "usart.h"

#define UART_TIMEOUT 100
#define QUEUE_SIZE   20
class CUartCom : public IComChannel
{
public:
    CUartCom(UART_HandleTypeDef *p_huart, const std::string name);
    CUartCom(UART_HandleTypeDef *p_huart,
             const std::string name,
             GPIO_TypeDef *uart_de_port,
             uint16_t uart_de_pin);

    void startReception();
    void send(const std::string &msg);
    bool isCommandAvailable();
    std::string getCommand();
    void uartRxHandler(UART_HandleTypeDef *p_huart);

    static CUartCom *sp_UART_1;
    static CUartCom *sp_UART_2;

private:
    UART_HandleTypeDef *mp_huart;
    CGpioWrapper m_uart_de_pin;
    CFifoBuffer m_rx_buffer;
    uint8_t m_rx_char;
    std::queue<std::string> m_cmd_queue;
};

#endif /* CUARTCOM_H_ */
