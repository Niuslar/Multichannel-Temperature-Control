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
#include "CTemperatureController.h"
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
 * ran without engaging peripherals.
 */
#ifdef MOCK_HARDWARE
CMockHardwareMap g_hardware_map("mock_hardware", 101);
#else
CRealHardwareMap g_hardware_map;
#endif

/**
 * The controllers are instantiated here. Note that the runtime variables are
 * prime numbers. This reduces contention between controllers requiring
 * attention simultaneously. Example: for runtime period of 97ms and 89ms, the
 * time between simultaneous requests will be 97 * 89 = 8633ms. This is rare
 * enough that any contention resulting in delay of execution will not stack up.
 * For three controllers to have contention over execution time period is even
 * larger. Example: for 89ms, 97ms, and 101ms to cause contention 871'933ms must
 * pass, which is equivalent to 14.5 hours. This will be very rare event that is
 * unlikely to cause any issues with clock starvation. For choices of prime
 * numbers to use see prime_numbers.txt file.
 */
CDebugController g_debug_controller("debug", 97);
CTemperatureController g_temp_controller("temperature", 89);

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
        g_dispatcher.registerController(&g_temp_controller);
        /**
         * @note Dispatcher run() method will not return. At this point
         * scheduling of controllers will start.
         */
        g_dispatcher.run();
    }

#ifdef __cplusplus
}
#endif
