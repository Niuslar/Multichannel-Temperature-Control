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

class CLog
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
    CUartCom *mp_error_uart;
    const std::string m_module_name;

public:
    // Public methods
    CLog(CUartCom *error_uart, const std::string module_name);
    void setLogLevel(log_level_t log_level);
    void log(log_level_t log_level, const std::string &message);
};

#endif /* ERROR_LOG_H_ */
