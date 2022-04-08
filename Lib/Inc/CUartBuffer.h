/**
 * @file CUartBuffer.h
 */

/*
 *  Created on: Mar 28, 2022
 *      Author: niuslar
 */

#ifndef CUARTBUFFER_H_
#define CUARTBUFFER_H_

#include <string>
#include "main.h"

#define BUF_SIZE 60

class CUartBuffer
{
public:
    CUartBuffer();
    ~CUartBuffer();

    bool put(const char data);
    std::string get();
    bool isFull();

private:
    char m_rx_buffer[BUF_SIZE];
    uint32_t m_head;
    uint32_t m_tail;
};

#endif /* CUARTBUFFER_H_ */
