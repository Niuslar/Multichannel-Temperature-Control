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
#define MIN_TEMP          0
#define MAX_TEMP          100
#define HEATER_CAPACITY   1
#define HEATER_FLOW       0.1
#define RADIATOR_CAPACITY 1
#define RADIATOR_FLOW     0.1

CMockHardwareMap::CMockHardwareMap(etl::string<MAX_STRING_SIZE> name,
                                   uint32_t run_period_ms)
    : CController(name, run_period_ms)
{
}

void CMockHardwareMap::init()
{
    /* all initialisation of mock hardware layer goes here. */
    reset();
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
            if (m_heater_power[i] > 0)
            {
                // incoming power
                float q_heat;
                q_heat =
            }
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
    if (p_command->getName()->compare("setambient"))
    {
        float temperature = (*p_command)[0];
        if ((temperature < MIN_TEMP) || (MAX_TEMP < temperature))
        {
            p_comchannel->send("Arguments out of bounds.");
        }
        else
        {
            m_ambient_temperature = temperature;
        }
        b_command_recognised = true;
    }
    /**
     * Command to modify power rating of the heater. Rating refers to heater
     * full power output. Channel is optional and if omitted command is applied
     * to all channels.
     * >setRating(rating, [channel])
     */
    if (p_command->getName()->compare("setrating"))
    {
        uint8_t channel = (uint8_t)(*p_command)[1];
        float rating = (*p_command)[0];
        if ((rating < 0) || (channel > HARD_PWM_OUTPUTS))
        {
            p_comchannel->send("Arguments out of bounds.");
        }
        else
        {
            setrating(rating, channel);
        }
        b_command_recognised = true;
    }
    /**
     * Command to modify heat capacity of the heater. Channel is optional and if
     * omitted command is applied to all channels.
     * >setcapacity(heater_capacity, radiator_capacity, [channel])
     */
    if (p_command->getName()->compare("setcapacity"))
    {
        uint8_t channel = (uint8_t)(*p_command)[2];
        float heater_capacity = (*p_command)[0];
        float radiator_capacity = (*p_command)[1];
        if ((heater_capacity < 0) || (radiator_capacity < 0) ||
            (channel > HARD_PWM_OUTPUTS))
        {
            p_comchannel->send("Arguments out of bounds.");
        }
        else
        {
            setcapacity(heater_capacity, radiator_capacity, channel);
        }
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
        m_temperature[i][0] = m_ambient_temperature;
        m_temperature[i][1] = m_ambient_temperature;
        m_heat_capacity[i][0] =
    }
}

void CMockHardwareMap::setrating(float rating, uint8_t channel)
{
    if (channel == 0)
    {
        for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
        {
            m_heater_rating[i] = rating;
        }
    }
    else
    {
        m_heater_rating[channel - 1] = rating;
    }
}

void CMockHardwareMap::setcapacity(float heater_capacity,
                                   float radiator_capacity,
                                   uint8_t channel)
{
    if (channel == 0)
    {
        for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
        {
            m_heat_capacity[i][0] = heater_capacity;
            m_heat_capacity[i][1] = radiator_capacity;
        }
    }
    else
    {
        m_heat_capacity[channel - 1][0] = heater_capacity;
        m_heat_capacity[channel - 1][1] = radiator_capacity;
    }
}
