/*
 * CHumidityController.cpp
 *
 *  Created on: Aug. 11, 2022
 *      Author: samk
 */

#include "CHumidityController.h"
#include <stdio.h>
#include "ICommand.h"
#include "IHardwareMap.h"
#include "CBME280.h"

#define MIN_TEMPERATURE  10
#define MAX_TEMPERATURE  50
#define MIN_POWER        0
#define MAX_POWER        100
#define DISABLE_OVERRIDE -1
#define DISABLE_TARGET   0
#define MIN_HUMIDITY 85
#define MAX_HUMIDITY 95
//#define X_HEATERS_ON 4
//#define X_HEATERS_OFF 4

/*
 * Read relative humidity value
 * If ideal, do nothing
 * If below ideal, turn on heater
 * If above ideal, turn off heater
 */

CHumidityController::CHumidityController(
		IHardwareMap *p_hardware,
	    etl::string<MAX_STRING_SIZE> name,
	    uint32_t run_period_ms)
	    : CController(name, run_period_ms),
	      mp_hw(p_hardware)
{
	// TODO Auto-generated constructor stub
	reset();
}

/**
 * @brief Run all repeated humidity control activities.
 *
 */
void CHumidityController::run()
{
	float actual_humidity;
	for (int i = 0; i < CHANNEL_NUMBER; i++)
	{
		float power = 0;
		actual_humidity = hum_sensor->getHumidity();
		float actual_temperature = hum_sensor->getTemperature();
		if (m_power_override[i] == DISABLE_OVERRIDE &&
				target_humidity != DISABLE_TARGET)
		{
			if (actual_humidity != target_humidity)
			{
				if(actual_humidity < target_humidity)
				{
					power = m_control_loop[i].run(actual_temperature,
														MIN_TEMPERATURE);
				}
				else
				{
					power = m_control_loop[i].run(actual_temperature,
														MAX_TEMPERATURE);
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			power = m_power_override[i];
		}
		mp_hw->setHardPwmOutput(power, i);
	}
	while(actual_humidity != target_humidity){
		run();
	}
}

bool CHumidityController::newCommand(ICommand *p_command,
                                        IComChannel *p_comchannel)
{
	bool b_command_recognised = false;
	ICommand::command_error_code_t result = ICommand::COMMAND_OK;
	/**
	 * Query about current state of all active channels.
	 *
	 */
    if (p_command->getName()->compare("?humidity") == 0)
    {
        sendStatus(p_comchannel);
        b_command_recognised = true;
    }

    if (p_command->getName()->compare("humidity") == 0)
	{
		result = setHumidity(p_command);
		b_command_recognised = true;
	}

    if (p_command->getName()->compare("heater") == 0)
    {
        result = overrideHeater(p_command);
        b_command_recognised = true;
    }
    if (b_command_recognised)
    {
        switch (result)
        {
            case ICommand::COMMAND_OK:
                //                p_comchannel->send("OK.\n");
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

void CHumidityController::reset()
{
	for (int i = 0; i < CHANNEL_NUMBER; i++)
	{
		target_humidity = DISABLE_TARGET;
		m_power_override[i] = DISABLE_OVERRIDE;
		m_control_loop[i].reset();
	}
}

void CHumidityController::sendStatus(IComChannel *p_comchannel){
    etl::string<MAX_STRING_SIZE> message;
    char value[10];
    //send target humidity
    message.assign("Target:      ");
    sprintf(value, "%2.1f", target_humidity);
    message.append(value);
    p_comchannel->send(message);
    // Send actual humidity
    message.assign("Humidity: ");
    sprintf(value, "%2.1f, ", hum_sensor->getHumidity());
    message.append(value);
    p_comchannel->send(message);
    // Send heater power
    message.assign("Power:       ");
    float power;
    for (int i = 0; i < CHANNEL_NUMBER - 1; i++)
    {
        if (m_power_override[i] == DISABLE_OVERRIDE)
        {
            power = mp_hw->getHardPwmOutput(i);
        }
        else
        {
            power = m_power_override[i];
        }
        sprintf(value, "%4.1f, ", power);
        message.append(value);
    }
    if (m_power_override[CHANNEL_NUMBER - 1] == DISABLE_OVERRIDE)
    {
        power = mp_hw->getHardPwmOutput(CHANNEL_NUMBER - 1);
    }
    else
    {
        power = m_power_override[CHANNEL_NUMBER - 1];
    }
    sprintf(value, "%4.1f\n", power);
    message.append(value);
    p_comchannel->send(message);
}

/**
 * @brief Set humidity value
 */
ICommand::command_error_code_t CHumidityController::setHumidity(
    ICommand *p_command)
{
	// Sanitise command arguments
	if ((p_command->getArgumentCount() < 1) ||
		(p_command->getArgumentCount() > 2))
	{
		return ICommand::ERROR_ARG_COUNT;
	}
	target_humidity = (*p_command)[0];

    if (target_humidity > MAX_HUMIDITY)
    {
        target_humidity = MAX_HUMIDITY;
    }
    if ((target_humidity < MIN_HUMIDITY) && (target_humidity != DISABLE_TARGET))
    {
        target_humidity = MIN_HUMIDITY;
    }

    return ICommand::COMMAND_OK;
}

ICommand::command_error_code_t CHumidityController::overrideHeater(
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

