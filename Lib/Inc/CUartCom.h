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

#define UART_TIMEOUT     100
#define MAX_UART_ENGINES 8

/**
*@note TX_BUF_SIZE needs to be greater than MAX_STRING_SIZE
*/
#define TX_BUF_SIZE      (MAX_STRING_SIZE + 1)
#define RX_QUEUE_SIZE    100
#define TX_QUEUE_SIZE    100

class CUartCom : public IComChannel
{
public:
    CUartCom(const etl::string<MAX_STRING_SIZE> name);

    bool init(UART_HandleTypeDef *p_huart);
    bool init(UART_HandleTypeDef *p_huart,
              GPIO_TypeDef *uart_de_port,
              uint16_t uart_de_pin);

    void startRx();
    void stopRx();

    bool send(uint8_t *p_data_buf, uint32_t len);
    bool send(const etl::string<MAX_STRING_SIZE> msg);

    bool isDataAvailable();
    uint8_t getData();

    void uartRxHandler(UART_HandleTypeDef *p_huart);
    void uartTxHandler(UART_HandleTypeDef *p_huart);

    static int8_t getIndex(UART_HandleTypeDef *p_huart);
    static CUartCom *sp_UART[MAX_UART_ENGINES];
    static uint8_t s_uart_instances;
    UART_HandleTypeDef *mp_huart;

private:
    void endTx();
    bool transmit();
    enum uart_status
    {
        IDLE,
        TX
    };

    typedef struct
    {
        uint32_t length;
        char buffer[TX_BUF_SIZE];
    } tx_data_t;

    bool mb_half_duplex = false;
    uint8_t m_status = IDLE;
    CGpioWrapper m_uart_de_pin;
    CFIFOBuffer<uint8_t, RX_QUEUE_SIZE> m_rx_queue;
    CFIFOBuffer<tx_data_t, TX_QUEUE_SIZE> m_tx_queue;
    tx_data_t m_data;
    uint8_t m_rx_byte;
};

#endif /* CUARTCOM_H_ */
