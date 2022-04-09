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

    void put(const char data);
    char get();
    std::string getString();

private:
    char m_rx_buffer[BUF_SIZE];
    uint32_t m_head;
    uint32_t m_tail;
};

#endif /* CUARTBUFFER_H_ */
