/**
 * @file cpp_link.cpp
 */

/*
 *  Created on: Feb 11, 2022
 *      Author: niuslar
 */

#include "log.h"
#include "uart_com.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void cpp_main()
    {
        CUartCom uart_for_errors(&huart2);

        CLog log_main(&uart_for_errors, "Main");
        log_main.setLogLevel(CLog::LOG_INFO);

#ifdef DEBUG
        log_main.log(CLog::LOG_INFO, "Entered cpp_main function");
#endif
        log_main.log(CLog::LOG_ERROR, "Pass debugging point");
        log_main.log(CLog::LOG_WARNING, "Hello this is a warning!");
        // C++ Code here

        // Infinite Loop
        while (1)
        {
        }
    }

#ifdef __cplusplus
}
#endif
