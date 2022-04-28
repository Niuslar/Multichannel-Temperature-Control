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
    /**
     * Query about current state of all active channels.
     *
     * ?temperature()
     */
    if (p_command->getName()->compare("?temperature"))
    {
        // todo: compile temperature report and send via p_comchannel.
        b_command_recognised = true;
    }
    /**
     * Set target temperature [for specific channel]. If channel argument is
     * missing, target is applied to all channels. If target is zero, then
     * disable control.
     *
     * temperature(target, [channel])
     */
    if (p_command->getName()->compare("temperature"))
    {
        // todo: process input arguments to set channel target temperatures
        b_command_recognised = true;
    }
    /**
     * Set heater power to specified level. If channel argument is missing, then
     * apply to all channels. if power argument is negative value, disable
     * heater override.
     * @note This command suspends operation of the control on
     * the channel, but does not disable it. Once override is removed, channel
     * returns to pre-override state. If it was idle, it returns to idle, if it
     * was under control, then control execution resumes.
     *
     * heater(power, [channel])
     */
    if (p_command->getName()->compare("heater"))
    {
        // todo: process input arguments to set heater output to specified
        // level.
        b_command_recognised = true;
    }
    return b_command_recognised;
}
void CTemperatureController::reset()
{
    // TODO: reset the controller to original constructed state.
}

void CTemperatureController::getStatus()
{
    // TODO: report current status of the controller.
}

void CTemperatureController::setTemperature(float target_temperature,
                                            uint8_t channel)
{
    // TODO: set target temperature for the channels
}

void CTemperatureController::overrideHeater(float power, uint8_t channel)
{
    // TODO: override the heaters.
}
