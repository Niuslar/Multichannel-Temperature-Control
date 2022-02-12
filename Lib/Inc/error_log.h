/**
 * @file error_log.h
 */

/*
 *  Created on: Feb 12, 2022
 *      Author: niuslar
 */

#ifndef ERROR_LOG_H_
#define ERROR_LOG_H_

#include <string>
#include "uart_com.h"

class Log
{
public:
    // Public variables
    enum log_level_t
    {
        LOG_ERROR = 0,
        LOG_WARNING,
        LOG_INFO
    };

private:
    // Private Variables
    log_level_t m_log_level = LOG_INFO;
    UartCom m_error_uart;
    const std::string m_module_name;

public:
    // Public methods
    Log(UART_HandleTypeDef &huart, const std::string module_name);
    void setLogLevel(log_level_t log_level);
    void info(const std::string &message);
    void error(const std::string &message);
    void warning(const std::string &message);
};

#endif /* ERROR_LOG_H_ */
