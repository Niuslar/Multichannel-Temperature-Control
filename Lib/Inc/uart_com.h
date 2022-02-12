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

#include <string>
#include "main.h"
#include "usart.h"

// I was thinking about using PascalCase for classes
// camelCase for methods/functions
// snake_case for variables
class UartCom
{
private:
public:
    void send_msg(const std::string &msg);
};

#endif /* UART_COM_H_ */
