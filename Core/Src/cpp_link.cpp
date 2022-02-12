/**
 * @file cpp_link.cpp
 */

/*
 *  Created on: Feb 11, 2022
 *      Author: niuslar
 */

#include "error_log.h"

#ifdef __cplusplus
extern "C"
{
#endif

    void my_main()
    {
        Log log_main(huart2, "Main");

#ifdef DEBUG
        log_main.info("Entered main function");
#endif
        log_main.error("AAAA");
        // C++ Code here
    }

#ifdef __cplusplus
}
#endif
