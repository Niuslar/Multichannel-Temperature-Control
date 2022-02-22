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
#define MAX_QUEUE_SIZE 5

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
    bool isQueueFull();

    // Static members
    static uint8_t s_rx_buffer[MAX_RX_BUF_LEN];
    static uint8_t *s_rx_buf_addr;
    static uint8_t s_cmd_length_counter;
    static std::queue<std::string> s_queue;
    static bool s_queue_full_flag;

private:
    UART_HandleTypeDef *mp_huart;
    GPIO_TypeDef *m_uart_de_port = nullptr;
    uint16_t m_uart_de_pin;
};

#endif /* CUARTCOM_H_ */
