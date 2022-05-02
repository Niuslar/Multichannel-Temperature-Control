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
#include "CFIFOBuffer.h"
#include "CGpioWrapper.h"
#include "IComChannel.h"
#include "usart.h"

#define UART_TIMEOUT      100
#define MAX_RX_QUEUE_SIZE 50
#define MAX_TX_QUEUE_SIZE 100
#define MAX_UART_ENGINES  8
#define TX_BUF_SIZE       70
#define RX_BUF_SIZE       60

class CUartCom : public IComChannel
{
public:
    CUartCom(const etl::string<MAX_STRING_SIZE> name);

    bool init(UART_HandleTypeDef *p_huart);
    bool init(UART_HandleTypeDef *p_huart,
              GPIO_TypeDef *uart_de_port,
              uint16_t uart_de_pin);

    void startRx();
    bool send(const etl::string<MAX_STRING_SIZE> msg);
    bool isDataAvailable();
    etl::string<MAX_STRING_SIZE> getData();
    void uartRxHandler(UART_HandleTypeDef *p_huart);
    void uartTxHandler(UART_HandleTypeDef *p_huart);

    static int8_t getIndex(UART_HandleTypeDef *p_huart);

    static CUartCom *sp_UART[MAX_UART_ENGINES];
    static uint8_t s_uart_instances;
    UART_HandleTypeDef *mp_huart;
    GPIO_TypeDef *m_uart_de_port = nullptr;
    uint16_t m_uart_de_pin;

private:
    void updateTxBuffer();
    void endTx();
    void stopRx();
    bool transmit();
    etl::string<MAX_STRING_SIZE> getString();
    enum uart_status
    {
        IDLE,
        TX
    };

    bool mb_half_duplex = false;
    uint8_t m_status = IDLE;
    CGpioWrapper m_uart_de_pin;
    CFIFOBuffer<char, RX_BUF_SIZE> m_rx_buffer;
    char m_tx_buffer[TX_BUF_SIZE] = {0};
    uint8_t m_tx_msg_length = 0;
    uint8_t m_rx_char;
    CFIFOBuffer<etl::string<MAX_STRING_SIZE>, MAX_RX_QUEUE_SIZE> m_rx_queue;
    CFIFOBuffer<etl::string<MAX_STRING_SIZE>, MAX_TX_QUEUE_SIZE> m_tx_queue;
};

#endif /* CUARTCOM_H_ */
