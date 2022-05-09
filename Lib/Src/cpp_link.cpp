/**
 * @file cpp_link.cpp
 */

/*
 *  Created on: Feb 11, 2022
 *      Author: niuslar
 */

#include "../etl/to_string.h"
#include "CAdcData.h"
#include "CDebugController.h"
#include "CDispatcher.h"
#include "CParser.h"
#include "CUartCom.h"
#include "adc.h"

#ifdef CSOFTPWMOUTPUT_H_
#    warning The software PWM has not been tested, do not use without testing.
#endif

etl::string<60> parsing_state[] = {"IDLE",
                                   "BEGIN_KEY",
                                   "END_KEY",
                                   "BEGIN_VALUE",
                                   "BEGIN_ARRAY",
                                   "ADD_VALUE",
                                   "VALUE_ADDED",
                                   "END_VALUES",
                                   "COMMAND_OK",
                                   "ERROR_MAX_ARGS",
                                   "ERROR_NESTED_JSON",
                                   "ERROR"};

/** @note: Instantiating classes here means the ctor for each class will run
 * before any of the setup code is being executed. Therefore, ctor that relies
 * on configured hardware will fail. If class that is meant to run hardware is
 * instantiated, then ctor must only do bare essentials. Once cpp_main() starts
 * executing relevant classes can call initialisation methods that will
 * configure the hardware.
 */
CUartCom g_debug_uart("Main");
CDispatcher g_dispatcher(&g_debug_uart);
CParser g_parser;

/* controllers */
CDebugController g_debug_controller("debug", 100);

#ifdef __cplusplus
extern "C"
{
#endif

    void cpp_main()
    {
        /**
         * @note By this point in the code all hardware has been configured. Run
         * initialisation code here.
         */
        g_dispatcher.registerController(&g_debug_controller);
        //        CUartCom uart_for_errors(&huart2);

        //        CLog log_main(&uart_for_errors, "Main");
        //        log_main.setLogLevel(CLog::LOG_INFO);

        g_debug_uart.init(&huart2);
        g_debug_uart.startRx();

        // Infinite Loop
        while (1)
        {
            if (g_debug_uart.isDataAvailable())
            {
                CParser::parser_state_t state =
                    g_parser.parse(g_debug_uart.getData());
                if (state == CParser::COMMAND_OK)
                {
                    g_debug_uart.send("Command name: ");
                    g_debug_uart.send(g_parser.getName());
                    g_debug_uart.send("\n");
                    unsigned int arg_count = g_parser.getArgumentCount();
                    for (unsigned int i = 0; i < arg_count; i++)
                    {
                        g_debug_uart.send("Argument ");
                        etl::string<10> index;
                        etl::to_string((i + 1), index);
                        g_debug_uart.send(index);
                        g_debug_uart.send(": ");
                        etl::string<30> argument;
                        etl::to_string(g_parser[i],
                                       argument,
                                       etl::format_spec().precision(1),
                                       true);
                        g_debug_uart.send(argument);
                        g_debug_uart.send("\n");
                    }
                }
                else
                {
                    g_debug_uart.send(parsing_state[state]);
                    g_debug_uart.send("\n");
                }
            }
        }
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
