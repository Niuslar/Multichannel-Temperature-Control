/**
 * @file cpp_link.cpp
 */

/*
 *  Created on: Feb 11, 2022
 *      Author: niuslar
 */

#include "error_log.h"
#include "uart_com.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void cpp_main()
    {
        CLog log_main(huart2, "Main");
        log_main.setLogLevel(CLog::LOG_WARNING);

#ifdef DEBUG
        log_main.log(CLog::LOG_INFO, "123456789012345678901234");
#endif
        log_main.log(CLog::LOG_ERROR, "123456789012345678901234");
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
