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

#include <cstring>
#include <queue>
#include <string>
#include "CCmdBuffer.h"
#include "CGpioWrapper.h"
#include "IComChannel.h"
#include "usart.h"

#define UART_TIMEOUT      100
#define MAX_RX_QUEUE_SIZE 40
#define MAX_TX_QUEUE_SIZE 40
#define MAX_UART_ENGINES  8
#define TX_BUF_SIZE       60

class CUartCom : public IComChannel
{
public:
    CUartCom(const std::string name);

    bool init(UART_HandleTypeDef *p_huart);
    bool init(UART_HandleTypeDef *p_huart,
              GPIO_TypeDef *uart_de_port,
              uint16_t uart_de_pin);

    void startRx();
    void send(const std::string msg);
    bool isCommandAvailable();
    std::string getCommand();
    void uartRxHandler(UART_HandleTypeDef *p_huart);
    void uartTxHandler(UART_HandleTypeDef *p_huart);

    static CUartCom *sp_UART[MAX_UART_ENGINES];
    static uint8_t s_uart_instances;
    UART_HandleTypeDef *mp_huart;

private:
    enum uart_states
    {
        IDLE,
        TX
    };
    enum uart_events
    {
        EMPTY_MSG,
        NON_EMPTY_MSG
    };
    bool mb_state = IDLE;
    CGpioWrapper m_uart_de_pin;
    CCmdBuffer m_rx_buffer;
    char m_tx_buffer[TX_BUF_SIZE] = {0};
    uint8_t m_rx_char;
    std::queue<std::string> m_rx_queue;
    std::queue<std::string> m_tx_queue;
};

#endif /* CUARTCOM_H_ */
