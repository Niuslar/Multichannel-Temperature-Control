/**
 * @file CMockHardwareMap.cpp
 *
 */

/*
 * CMockHardwareMap.cpp
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#include "CMockHardwareMap.h"

#define DEFAULT_AMBIENT_T 20

CMockHardwareMap::CMockHardwareMap(etl::string<MAX_STRING_SIZE> name,
                                   uint32_t run_period_ms)
    : CController(name, run_period_ms)
{
    reset();
}

void CMockHardwareMap::init()
{
    /* all initialisation of mock hardware layer goes here. */
}

float CMockHardwareMap::getInputVoltage() const
{
    return 24.0;
}

float CMockHardwareMap::getTotalCurrent() const
{
    // TODO: model current consumption based control current + some for digital
    // logic.
    return getControlCurrent() + 0.1;
}

float CMockHardwareMap::getControlCurrent() const
{
    // TODO: model current consumption based on number of heaters powered on.
    return 0;
}

float CMockHardwareMap::getAmbientTemp() const
{
    // TODO: consider modelling this.
    return 21.0;
}

float CMockHardwareMap::getChanneTemp(uint8_t channel) const
{
    return getAmbientTemp();
}

float CMockHardwareMap::setHardPwmOutput(float power, uint8_t channel)
{
    // TODO: This sets power parameters for the model.
    return 0;
}

float CMockHardwareMap::getHardPwmOutput(uint8_t channel)
{
    // TODO: this just returns what's been recorded for heater power
    return 0;
}

#ifdef SOFT_PWM_OUTPUTS
float CMockHardwareMap::setSoftPwmOutput(float power, uint8_t channel) = 0;
float CMockHardwareMap::getSoftPwmOutput(uint8_t channel) = 0;
#endif

void CMockHardwareMap::setBreathingLight(float duty_cycle) {}

void CMockHardwareMap::enableControlPower(bool b_enable)
{
    // todo: this should probably participate in the model of control current
    // flow and temperature control.
}

void CMockHardwareMap::run()
{
    /* if control power enabled, then need to model total heat input. */
    if (mb_power_enable)
    {
        for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
        {
            // TODO: run model here and update temperatures for sensors and
            // total incubator temperature. Also, calculate total control
            // current.
        }
    }
}

/**
 * @note: This method is not for sending commands like >setHeaterPower(). This
 * method is to simulate input from real world conditions changing as the device
 * is being tested. For example: induce failure of a heater or thermistor. Or
 * set different ambient temperature.
 */
bool CMockHardwareMap::newCommand(ICommand *p_command,
                                  IComChannel *p_comchannel)
{
    bool b_command_recognised = false;
    /**
     * Command to modify mock hardware ambient temperature
     * >setambient(20);
     */
    if (p_command->getName()->compare("setAmbient"))
    {
        m_ambient_temperature = (*p_command)[0];
        b_command_recognised = true;
    }
    return b_command_recognised;
}

void CMockHardwareMap::reset()
{
    m_ambient_temperature = DEFAULT_AMBIENT_T;
    m_incubator_temperature = m_ambient_temperature;
    mb_power_enable = false;
    for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
    {
        m_heater_power[i] = 0;
        m_temperature[i] = m_ambient_temperature;
    }
}
