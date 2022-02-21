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
#include "IComChannel.h"
#include "usart.h"

#define UART_TIMEOUT   100
#define MAX_RX_BUF_LEN 100

class CUartCom : public IComChannel
{
public:
    CUartCom(const std::string name, UART_HandleTypeDef *p_huart);
    CUartCom(std::string name,
             UART_HandleTypeDef *p_huart,
             GPIO_TypeDef *uart_de_port,
             uint16_t uart_de_pin);

    void startReception();
    void send(const std::string &message);
    std::string getCommand();
    bool isCommandAvailable();
    void pushCommand();

    // Static members
    // TODO: Check if they need to be volatile
    static bool s_buffer_full;
    static uint8_t s_rx_buffer[MAX_RX_BUF_LEN];
    static uint8_t *s_rx_buf_addr;

private:
    UART_HandleTypeDef *mp_huart;
    GPIO_TypeDef *m_uart_de_port = nullptr;
    uint16_t m_uart_de_pin;
    std::queue<std::string> m_queue;
};

#endif /* CUARTCOM_H_ */
