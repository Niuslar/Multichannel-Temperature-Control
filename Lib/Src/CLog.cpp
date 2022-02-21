/**
 * @file CLog.cpp
 */

/*
 *  Created on: Feb 12, 2022
 *      Author: niuslar
 */

#include "CLog.h"

// TODO: The logger needs to operate with a generic base class IComChannel.
// TODO: Logger needs to have a "log, but do not send until asked" mode. In this
// mode logger stashes all logs in memory until specifically requested by the
// host. It probably makes sense to have logger either belong to
// CDebugController class or have CLog inherit form CController class to be able
// to run and process commands.
/**
 * @brief Constructor
 * @param Pointer to CUartCom object
 * @param module_name Identifier that will be added to the message
 */
CLog::CLog(CUartCom *p_error_uart, const std::string &module_name)
    : mp_error_uart(p_error_uart),
      m_module_name(module_name)
{
}

/**
 * @brief change log level to change what is sent via UART
 * @note  Messages with a log_level below m_log_level will be lost
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
    if (log_level == LOG_ERROR && m_log_level >= LOG_ERROR)
    {
        // Create full message as string
        const std::string full_msg = ((const std::string)(
            "[ERROR]->" + m_module_name + ": " + message + "\n"));

        // Send message using UART
        mp_error_uart->sendMessage(full_msg);
    }

    else if (log_level == LOG_WARNING && m_log_level >= LOG_WARNING)
    {
        const std::string full_msg = ((const std::string)(
            "[WARNING]->" + m_module_name + ": " + message + "\n"));

        mp_error_uart->sendMessage(full_msg);
    }

    else if (log_level == LOG_INFO && m_log_level >= LOG_INFO)
    {
        const std::string full_msg = ((const std::string)(
            "[INFO]->" + m_module_name + ": " + message + "\n"));

        mp_error_uart->sendMessage(full_msg);
    }

    else
    {
        const std::string full_msg = ((const std::string)(
            "[WARNING]->" + m_module_name + ": Invalid Log Level!\n"));

        mp_error_uart->sendMessage(full_msg);
    }
}
