/**
 * @file cpp_link.cpp
 */

/*
 *  Created on: Feb 11, 2022
 *      Author: niuslar
 */

#include "CAdcData.h"
#include "CLog.h"
#include "CUartCom.h"
#include "adc.h"

// TODO: everything should be instantiated here, so its not on the stack.

#ifdef __cplusplus
extern "C"
{
#endif

    void cpp_main()
    {
        CUartCom uart_for_errors(&huart2);

        CLog log_main(&uart_for_errors, "Main");
        log_main.setLogLevel(CLog::LOG_INFO);

        CAdcData adc_1(&hadc);
        adc_1.init();

#ifdef DEBUG
        log_main.log(CLog::LOG_INFO, "Entered cpp_main function");
#endif

        // Infinite Loop

        // TODO: Once everything has been instantiated, CDispatcher instance
        // should call its run() method instead of this while(1) loop.
        while (1)
        {
#ifdef DEBUG
            // Test 3 of the 14 ADC channels

            // First read channels
            uint16_t adc_ch_1 = adc_1[0];
            uint16_t adc_ch_2 = adc_1[1];
            uint16_t adc_ch_14 = adc_1[13];

            std::string value1 = "ADC1 = " + std::to_string(adc_ch_1);
            std::string value2 = "ADC2 = " + std::to_string(adc_ch_2);
            std::string value3 = "ADC14 = " + std::to_string(adc_ch_14);

            log_main.log(CLog::LOG_INFO, value1);
            log_main.log(CLog::LOG_INFO, value2);
            log_main.log(CLog::LOG_INFO, value3);

            HAL_Delay(2000);
            adc_1.trigger();
#endif
        }

        log_main.log(CLog::LOG_ERROR, "Reached end of cpp_main()");
    }

#ifdef __cplusplus
}
#endif
