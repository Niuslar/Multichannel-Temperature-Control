/**
 * @file cpp_link.cpp
 */

/*
 *  Created on: Feb 11, 2022
 *      Author: niuslar
 */

#include "CAdcData.h"
#include "CDebugController.h"
#include "CDispatcher.h"
#include "CMockHardwareMap.h"
#include "CRealHardwareMap.h"
#include "CUartCom.h"
#include "adc.h"

#ifdef CSOFTPWMOUTPUT_H_
#    warning The software PWM has not been tested, do not use without testing.
#endif

/** @note: Instantiating classes here means the ctor for each class will run
 * before any of the setup code is being executed. Therefore, ctor that relies
 * on configured hardware will fail. If class that is meant to run hardware is
 * instantiated, then ctor must only do bare essentials. Once cpp_main() starts
 * executing relevant classes can call initialisation methods that will
 * configure the hardware.
 */
CUartCom g_debug_uart("Main");
CDispatcher g_dispatcher(&g_debug_uart);

/* hardware map */
/**
 * @note This compile time switch allows for defining of whether a real
 * peripherals are used or mock ones. Mock peripherals allow for the logic to be
 * ran without engaging peripherals. I
 */
#ifdef MOCK_HARDWARE
CMockHardwareMap g_hardware_map;
#else
CRealHardwareMap g_hardware_map;
#endif

/* controllers */
CDebugController g_debug_controller("debug", 100);

#ifdef __cplusplus
extern "C"
{
#endif

    void cpp_main()
    {
        g_hardware_map.init();
        /**
         * @note By this point in the code all hardware has been configured. Run
         * initialisation code here.
         */
        g_dispatcher.registerController(&g_debug_controller);
        //        CUartCom uart_for_errors(&huart2);

        //        CLog log_main(&uart_for_errors, "Main");
        //        log_main.setLogLevel(CLog::LOG_INFO);

        //        CAdcData adc_1(&hadc);
        //        adc_1.init();

        // Infinite Loop

        /**
         * @note Dispatcher run() method will not return. At this point
         * scheduling of controllers will start.
         */
        g_dispatcher.run();
        //        while (1)
        //        {
        //#ifdef DEBUG
        //            // Test 3 of the 14 ADC channels
        //
        //            // First read channels
        //            uint16_t adc_ch_1 = adc_1[0];
        //            uint16_t adc_ch_2 = adc_1[1];
        //            uint16_t adc_ch_14 = adc_1[13];
        //
        //            std::string value1 = "ADC1 = " + std::to_string(adc_ch_1);
        //            std::string value2 = "ADC2 = " + std::to_string(adc_ch_2);
        //            std::string value3 = "ADC14 = " +
        //            std::to_string(adc_ch_14);
        //
        //            log_main.log(CLog::LOG_INFO, value1);
        //            log_main.log(CLog::LOG_INFO, value2);
        //            log_main.log(CLog::LOG_INFO, value3);
        //
        //            HAL_Delay(2000);
        //            adc_1.trigger();
        //#endif
        //        }
    }

#ifdef __cplusplus
}
#endif
