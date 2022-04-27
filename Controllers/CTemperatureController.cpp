/**
 * @file CTemperatureController.cpp
 *
 */

/*
 * CTemperatureController.cpp
 *
 *  Created on: 26 Apr 2022
 *      Author: salavat.magazov
 */

#include <CTemperatureController.h>

CTemperatureController::CTemperatureController(
    etl::string<MAX_STRING_SIZE> name,
    uint32_t run_period_ms)
    : CController(name, run_period_ms)
{
}

/**
 * @brief Run all repeated temperature control activities.
 *
 */
void CTemperatureController::run() {}

bool CTemperatureController::newCommand(ICommand *p_command,
                                        IComChannel *p_comchannel)
{
    bool b_command_recognised = false;
    etl::string<MAX_STRING_SIZE> command_name;
    command_name = p_command->getName();
    /* generic query about current state of all active channels
     *?temperature()
     */
    if (command_name.compare("?temperature"))
    {
        // todo: compile temperature report and send via p_comchannel.
    }
    /* set target temperature [for specific channel]. If channel argument is
     * missing, target is applied to all channels.
     *  temperature(target, [channel])
     */
    if (command_name.compare("temperature")) return b_command_recognised;
}
void CTemperatureController::reset() {}
