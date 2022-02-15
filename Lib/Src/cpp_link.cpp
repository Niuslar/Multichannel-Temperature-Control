/**
 * @file cpp_link.cpp
 */

/*
 *  Created on: Feb 11, 2022
 *      Author: niuslar
 */

#include "adc.h"
#include "adc_data.h"
#include "log.h"
#include "stdio.h"
#include "thermistor.h"
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

        CAdcData adc_1(&hadc);
        adc_1.init();

        CThermistor thermistor_1(adc_1, 0);

#ifdef DEBUG
        log_main.log(CLog::LOG_INFO, "Entered cpp_main function");
#endif

        char buffer[20];

        // Infinite Loop
        while (1)
        {
#ifdef DEBUG
            // Test 3 of the 14 ADC channels

            // First read channels
            uint16_t adc_ch_1 = adc_1[0];
            uint16_t adc_ch_2 = adc_1[1];
            uint16_t adc_ch_14 = adc_1[13];

            float temp = thermistor_1.getTemperature();

            sprintf(buffer, "Temp: %.2f", temp);

            std::string value1 = "ADC1 = " + std::to_string(adc_ch_1);
            std::string value2 = "ADC2 = " + std::to_string(adc_ch_2);
            std::string value3 = "ADC14 = " + std::to_string(adc_ch_14);

            log_main.log(CLog::LOG_INFO, buffer);
            log_main.log(CLog::LOG_INFO, value1);
            log_main.log(CLog::LOG_INFO, value2);
            log_main.log(CLog::LOG_INFO, value3);

            HAL_Delay(1000);
            adc_1.trigger();
#endif
        }

        log_main.log(CLog::LOG_ERROR, "Reached end of cpp_main()");
    }

#ifdef __cplusplus
}
#endif
