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
#include "CParser.h"
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
CParser g_CParser_parser;

/* controllers */
CDebugController g_debug_controller("debug", 100);

etl::string<60> token_types[] = {"WHITESPACE",
                                 "IDENTIFIER",
                                 "INTEGER",
                                 "FLOAT",
                                 "STRING_LITERAL",
                                 "BEGIN_OPERATOR",
                                 "END_OPERATOR",
                                 "OPERATOR",
                                 "INVALID"};

etl::string<30> parser_status[] = {"IDLE",
                                   "BEGIN_KEY",
                                   "END_KEY",
                                   "BEGIN_VALUE",
                                   "END_VALUE",
                                   "FINISHED",
                                   "ERROR"};

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

        //        CAdcData adc_1(&hadc);
        //        adc_1.init();

        g_debug_uart.init(&huart2);
        g_debug_uart.startRx();

#ifdef DEBUG
        g_debug_uart.send("[INFO]: Entered cpp_main function\n");
#endif

        // Infinite Loop
        while (1)
        {
            if (g_debug_uart.isCommandAvailable())
            {
                etl::string<60> string = g_debug_uart.getCommand();

                CParser::parse_status_t status = g_CParser_parser.parse(string);

                CParser::command_t command = g_CParser_parser.getCommand();
                g_debug_uart.send("Parsing status :");
                g_debug_uart.send(parser_status[status]);
                g_debug_uart.send("\n");
                if (status == CParser::FINISHED)
                {
                    g_debug_uart.send("Command name: ");
                    g_debug_uart.send(command.name);
                    g_debug_uart.send("\n");
                    g_debug_uart.send("Argument: ");
                    g_debug_uart.send(command.argument);
                    g_debug_uart.send("\n");
                }
            }
        }
        /**
         * @note Dispatcher run() method will not return. At this point
         * scheduling of controllers will start.
         */
        // g_dispatcher.run();
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
        //            std::string value1 = "ADC1 = " +
        //            std::to_string(adc_ch_1); std::string value2 = "ADC2 =
        //            " + std::to_string(adc_ch_2); std::string value3 =
        //            "ADC14 = " + std::to_string(adc_ch_14);
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
