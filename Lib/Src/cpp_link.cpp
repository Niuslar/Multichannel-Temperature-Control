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

#ifdef DEBUG
        log_main.info("Entered main function");
#endif
        log_main.error("AAAA");
        // C++ Code here

        // Infinite Loop
        while (1)
        {
        }
    }

#ifdef __cplusplus
}
#endif
