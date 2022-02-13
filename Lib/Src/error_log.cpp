/**
 * @file error_log.cpp
 */

/*
 *  Created on: Feb 12, 2022
 *      Author: niuslar
 */

#include "error_log.h"

/**
 * @brief Constructor
 * @param huart UART Handler
 * @param module_name Identifier that will be added to the message
 */
CLog::CLog(UART_HandleTypeDef &huart, const std::string module_name)
    : m_error_uart(huart),
      m_module_name(module_name)
{
}

/**
 * @brief change log level to change what is sent
 * @param log_level:
 * 			LOG_ERROR = will show only errors
 * 			LOG_WARNING = will show error and warnings
 * 			LOG_INFO = will show everything
 */
void CLog::setLogLevel(log_level_t log_level)
{
    m_log_level = log_level;
}

/**
 * @brief method to log errors, warnings and info
 * @param log_level Level of the message (LOG_ERROR, LOG_WARNING, LOG_INFO)
 * @param message
 */
void CLog::log(log_level_t log_level, const std::string &message)
{
    if (log_level == LOG_INFO && m_log_level >= LOG_INFO)
    {
        // Convert to C style string
        const char *msg = ((const std::string)("[INFO]->" + m_module_name +
                                               ": " + message + "\n"))
                              .c_str();
        // Send message using UART
        m_error_uart.sendMessage(msg);
    }

    if (log_level == LOG_ERROR && m_log_level >= LOG_ERROR)
    {
        const char *msg = ((const std::string)("[ERROR]->" + m_module_name +
                                               ": " + message + "\n"))
                              .c_str();
        m_error_uart.sendMessage(msg);
    }

    if (log_level == LOG_WARNING && m_log_level >= LOG_WARNING)
    {
        const char *msg = ((const std::string)("[WARNING]->" + m_module_name +
                                               ": " + message + "\n"))
                              .c_str();
        m_error_uart.sendMessage(msg);
    }
}
