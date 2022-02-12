/**
 * @file error_log.cpp
 */

/*
 *  Created on: Feb 12, 2022
 *      Author: niuslar
 */

#include "error_log.h"

Log::Log(UART_HandleTypeDef &huart, const std::string module_name)
    : m_error_uart(huart),
      m_module_name(module_name)
{
}

void Log::setLogLevel(log_level_t log_level)
{
    m_log_level = log_level;
}

void Log::info(const std::string &message)
{
    if (m_log_level >= LOG_INFO)
    {
        // Convert to C style string
        const char *msg = ((const std::string)("[INFO]->" + m_module_name +
                                               ": " + message + "\n"))
                              .c_str();
        // Send message using UART
        m_error_uart.send_msg(msg);
    }
}

void Log::error(const std::string &message)
{
    if (m_log_level >= LOG_ERROR)
    {
        const char *msg = ((const std::string)("[ERROR]->" + m_module_name +
                                               ": " + message + "\n"))
                              .c_str();
        // Send message using UART
        m_error_uart.send_msg(msg);
    }
}

void Log::warning(const std::string &message)
{
    if (m_log_level >= LOG_WARNING)
    {
        const char *msg = ((const std::string)("[WARNING]->" + m_module_name +
                                               ": " + message + "\n"))
                              .c_str();
        // Send message using UART
        m_error_uart.send_msg(msg);
    }
}
