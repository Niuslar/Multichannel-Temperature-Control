/*
 * CHumidityController.cpp
 *
 *  Created on: Aug. 11, 2022
 *      Author: samk
 */

#include "CHumidityController.h"
#include <stdio.h>

CHumidityController::CHumidityController(IHardwareMap *p_hardware,
                                         etl::string<MAX_STRING_SIZE> name,
                                         uint32_t run_period_ms,
                                         SPI_HandleTypeDef *p_spi,
                                         GPIO_TypeDef *p_slave_select_port,
                                         uint16_t slave_select_pin)
    : CController(name, run_period_ms),
      mp_hw(p_hardware)
{
    // TODO Auto-generated constructor stub
    reset();

    // TODO Fix this constructor
    // CBME280 humidity_sensor(p_spi, p_slave_select_port, slave_select_pin);
    // mp_humidity_sensor = &humidity_sensor;
}

/**
 * @brief Run all repeated humidity control activities.
 *
 */
void CHumidityController::run()
{
    float power = 0;
    if (m_power_override == DISABLE_OVERRIDE)
    {
        if (m_target_humidity != DISABLE_TARGET)
        {
            float actual_humidity = mp_humidity_sensor->getHumidity();
            power = m_control_loop.run(m_target_humidity, actual_humidity);
        }
    }
    else
    {
        power = m_power_override;
    }
    mp_hw->setHumidifierPower(power);
}

bool CHumidityController::newCommand(ICommand *p_command,
                                     IComChannel *p_comchannel)
{
    bool b_command_recognised = false;
    ICommand::command_error_code_t result = ICommand::COMMAND_OK;
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
        result = overrideHumidifier(p_command);
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
    m_target_humidity = DISABLE_TARGET;
    m_power_override = DISABLE_OVERRIDE;
    m_control_loop.reset();
}

void CHumidityController::sendStatus(IComChannel *p_comchannel)
{
    etl::string<MAX_STRING_SIZE> message;
    char value[10];
    // send target humidity
    message.assign("Target:      ");
    sprintf(value, "%2.1f", m_target_humidity);
    message.append(value);
    p_comchannel->send(message);
    // Send actual humidity
    message.assign("Humidity: ");
    sprintf(value, "%2.1f, ", mp_humidity_sensor->getHumidity());
    message.append(value);
    p_comchannel->send(message);
    // Send heater power
    message.assign("Power:       ");
    float power;
    if (m_power_override == DISABLE_OVERRIDE)
    {
        power = mp_hw->getHardPwmOutput(CHANNEL_NUMBER - 1);
    }
    else
    {
        power = m_power_override;
    }
    sprintf(value, "%4.1f, ", power);
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
    if (p_command->getArgumentCount() != 1)
    {
        return ICommand::ERROR_ARG_COUNT;
    }
    m_target_humidity = (*p_command)[0];

    if (m_target_humidity > MAX_HUMIDITY)
    {
        m_target_humidity = MAX_HUMIDITY;
    }
    if ((m_target_humidity < MIN_HUMIDITY) &&
        (m_target_humidity != DISABLE_TARGET))
    {
        m_target_humidity = MIN_HUMIDITY;
    }

    return ICommand::COMMAND_OK;
}

ICommand::command_error_code_t CHumidityController::overrideHumidifier(
    ICommand *p_command)
{
    // Sanitise command arguments
    if (p_command->getArgumentCount() != 1)
    {
        return ICommand::ERROR_ARG_COUNT;
    }
    float power = (*p_command)[0];

    // Execute command
    if (power > MAX_POWER)
    {
        power = MAX_POWER;
    }
    if ((power < MIN_POWER) && (power != DISABLE_OVERRIDE))
    {
        power = MIN_POWER;
    }
    m_power_override = power;
    return ICommand::COMMAND_OK;
}
