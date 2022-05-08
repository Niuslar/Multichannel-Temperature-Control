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
#include "ICommand.h"
#include "IHardwareMap.h"

extern IHardwareMap *gp_hardware_map;

#define MIN_TEMPERATURE  10
#define MAX_TEMPERATURE  50
#define MIN_POWER        0
#define MAX_POWER        100
#define DISABLE_OVERRIDE -1
#define DISABLE_TARGET   0

CTemperatureController::CTemperatureController(
    etl::string<MAX_STRING_SIZE> name,
    uint32_t run_period_ms)
    : CController(name, run_period_ms)
{
    // TODO: need to run PID loop setups.
    reset();
}

/**
 * @brief Run all repeated temperature control activities.
 *
 */
void CTemperatureController::run()
{
    for (int i = 0; i < CHANNEL_NUMBER; i++)
    {
        float power = 0;
        if (m_power_override[i] == DISABLE_OVERRIDE)
        {
            if (m_target_temperature != DISABLE_TARGET)
            {
                float actual_temperature = gp_hardware_map->getChannelTemp(i);
                if ((MIN_TEMPERATURE <= actual_temperature) &&
                    (actual_temperature <= MAX_TEMPERATURE))
                {
                    power = m_control_loop[i].run(m_target_temperature[i],
                                                  actual_temperature);
                }
            }
        }
        else
        {
            power = m_power_override[i];
        }
        gp_hardware_map->setHardPwmOutput(power, i);
    }
}

bool CTemperatureController::newCommand(ICommand *p_command,
                                        IComChannel *p_comchannel)
{
    bool b_command_recognised = false;
    ICommand::command_error_code_t result = ICommand::COMMAND_OK;
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
        result = setTemperature(p_command);
        b_command_recognised = true;
    }
    /**
     * Set heater power to specified level. If channel argument is missing,
     * then apply to all channels. if power argument is negative value,
     * disable heater override.
     * @note This command suspends operation of the control on
     * the channel, but does not disable it. Once override is removed,
     * channel returns to pre-override state. If it was idle, it returns to
     * idle, if it was under control, then control execution resumes.
     *
     * heater(power, [channel])
     */
    if (p_command->getName()->compare("heater"))
    {
        result = overrideHeater(p_command);
        b_command_recognised = true;
    }
    if (b_command_recognised)
    {
        switch (result)
        {
            case ICommand::COMMAND_OK:
                p_comchannel->send("OK.\n");
                break;
            case ICommand::ERROR_ARG_COUNT:
                p_comchannel->send("Wrong number of arguments.\n");
                break;
            case ICommand::ERROR_OUT_OF_BOUNDS:
                p_comchannel->send("Argument out of bounds.\n");
                break;
            case ICommand::ERROR_TYPE_MISMATCH:
                p_comchannel->send("Argument type mismatch.\n");
                break;
            default:
                p_comchannel->send("Non-specific error with the command.");
        }
    }
    return b_command_recognised;
}
void CTemperatureController::reset()
{
    for (int i = 0; i < CHANNEL_NUMBER; i++)
    {
        m_target_temperature[i] = DISABLE_TARGET;
        m_power_override[i] = DISABLE_OVERRIDE;
        m_control_loop[i].reset();
    }
}

void CTemperatureController::getStatus()
{
    // TODO: report current status of the controller.
}

ICommand::command_error_code_t CTemperatureController::setTemperature(
    ICommand *p_command)
{
    // Sanitise command arguments
    if ((p_command->getArgumentCount() < 1) ||
        (p_command->getArgumentCount() > 2))
    {
        return ICommand::ERROR_ARG_COUNT;
    }
    float target_temp = (*p_command)[0];
    uint8_t channel = (uint8_t)(*p_command)[1];
    if (channel > CHANNEL_NUMBER)
    {
        return ICommand::ERROR_OUT_OF_BOUNDS;
    }
    if (p_command->getArgumentCount() == 1)
    {
        channel = 0;
    }
    else
    {
        // compare typecast value to original to find out if there is
        // fraction.
        if (channel != (*p_command)[1])
        {
            return ICommand::ERROR_TYPE_MISMATCH;
        }
    }
    // Range check target temperature
    if (target_temp > MAX_TEMPERATURE)
    {
        target_temp = MAX_TEMPERATURE;
    }
    if ((target_temp < MIN_TEMPERATURE) && (target_temp != DISABLE_TARGET))
    {
        target_temp = MIN_TEMPERATURE;
    }
    if (channel == 0)
    {
        for (int i = 0; i < CHANNEL_NUMBER; i++)
        {
            m_target_temperature[i] = target_temp;
        }
    }
    else
    {
        m_target_temperature[channel - 1] = target_temp;
    }
    return ICommand::COMMAND_OK;
}

ICommand::command_error_code_t CTemperatureController::overrideHeater(
    ICommand *p_command)
{
    // Sanitise command arguments
    if ((p_command->getArgumentCount() < 1) ||
        (p_command->getArgumentCount() > 2))
    {
        return ICommand::ERROR_ARG_COUNT;
    }
    float power = (*p_command)[0];
    uint8_t channel = (uint8_t)(*p_command)[1];
    if (channel > CHANNEL_NUMBER)
    {
        return ICommand::ERROR_OUT_OF_BOUNDS;
    }
    if (p_command->getArgumentCount() == 1)
    {
        channel = 0;
    }
    else
    {
        // compare typecast value to original to find out if there is
        // fraction.
        if (channel != (*p_command)[1])
        {
            return ICommand::ERROR_TYPE_MISMATCH;
        }
    }
    // Execute command
    if (power > MAX_POWER)
    {
        power = MAX_POWER;
    }
    if ((power < MIN_POWER) && (power != DISABLE_OVERRIDE))
    {
        power = MIN_POWER;
    }
    if (channel == 0)
    {
        for (int i = 0; i < CHANNEL_NUMBER; i++)
        {
            m_power_override[i] = power;
        }
    }
    else
    {
        m_power_override[channel - 1] = power;
    }
    return ICommand::COMMAND_OK;
}
