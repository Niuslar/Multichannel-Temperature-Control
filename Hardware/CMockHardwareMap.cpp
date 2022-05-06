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

#define AMBIENT_T            20    // [degC]
#define MIN_TEMP             0     // [degC]
#define MAX_TEMP             100   // [degC]
#define HEATER_RATING        50    // [Watt]
#define HEATER_CAPACITY      1     // [Watt/degC]
#define HEATER_CONDUCTANCE   0.1   // [degC/Watt]
#define RADIATOR_CAPACITY    1     // [Watt/degC]
#define RADIATOR_CONDUCTANCE 0.1   // [degC/Watt]
#define INCUBATOR_CAPACITY   10    // [Watt/degC]
#define INCUBATOR_LOSS       0.01  // [degC/Watt]

#define DIGITAL_CURRENT_CONSUMPTION 0.5  // half an amp for digital part of PCB

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
    return getControlCurrent() + DIGITAL_CURRENT_CONSUMPTION;
}

float CMockHardwareMap::getControlCurrent() const
{
    // TODO: model current consumption based on number of heaters powered on.
    return 0;
}

float CMockHardwareMap::getAmbientTemp() const
{
    return m_ambient_temperature;
}

float CMockHardwareMap::getChannelTemp(uint8_t channel) const
{
    if (channel < HARD_PWM_OUTPUTS)
    {
        return m_temperature[channel - 1][1];
    }
    else
    {
        return 0;
    }
}

float CMockHardwareMap::setHardPwmOutput(float power, uint8_t channel)
{
    if (channel < HARD_PWM_OUTPUTS)
    {
        if (power < 0)
        {
            power = 0;
        }
        if (power > 100)
        {
            power = 100;
        }
        m_heater_power[channel - 1] = power;
    }
    else
    {
        power = 0;
    }
    return power;
}

float CMockHardwareMap::getHardPwmOutput(uint8_t channel)
{
    float power = 0;
    if (channel < HARD_PWM_OUTPUTS)
    {
        power = m_heater_power[channel - 1];
    }
    return power;
}

#ifdef SOFT_PWM_OUTPUTS
float CMockHardwareMap::setSoftPwmOutput(float power, uint8_t channel) = 0;
float CMockHardwareMap::getSoftPwmOutput(uint8_t channel) = 0;
#endif

void CMockHardwareMap::setBreathingLight(float duty_cycle) {}

void CMockHardwareMap::enableControlPower(bool b_enable)
{
    mb_power_enable = b_enable;
}

void CMockHardwareMap::run()
{
    float total_radiator_flow = 0;
    m_control_current = 0;
    for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
    {
        float heater_flow;
        float heater_power;
        float radiator_flow;
        /**
         * Update temperature of the heater stage. heater_flow is heat flowing
         * from heater to radiator due to temperature gradient. From heater
         * point of view this is net loss of heat.
         * The heater temperature is then updated based on heater power input
         * and heater power loss to radiator. */
        heater_flow = (m_temperature[i][0] - m_temperature[i][1]) /
                      m_heat_conductance[i][0];
        if (mb_power_enable)
        {
            heater_power = m_heater_power[i] * m_heater_rating[i] / 100;
            /**
               Calculate control current based on each heater consumption and
               current input voltage.          */
            m_control_current += heater_power / getInputVoltage();
        }
        else
        {
            heater_power = 0;
        }
        m_temperature[i][0] += heater_power - heater_flow * m_run_period_ms /
                                                  1000 / m_heat_capacity[i][0];

        /**
         * Update temperature of the radiator stage. radiator_flow is heat
         * flowing from radiator into incubator ambient air. From radiator point
         * of view this is net loss of heat.
         * The radiator temperature is then updated based on positive flow from
         * heater and loss of heat to incubator.
         * Total radiator heat flow is also accumulated to calculate incubator
         * temperature. Negative sign is due to radiator loss being net positive
         * flow for incubator.
         */
        radiator_flow = (m_temperature[i][1] - m_incubator_temperature) /
                        m_heat_conductance[i][1];
        m_temperature[i][1] += (heater_flow - radiator_flow) * m_run_period_ms /
                               1000 / m_heat_capacity[i][1];
        total_radiator_flow -= radiator_flow;
    }
    float incubator_flow;
    incubator_flow =
        (m_incubator_temperature - m_ambient_temperature) / m_incubator_loss;
    m_incubator_temperature += (total_radiator_flow - incubator_flow) *
                               m_run_period_ms / 1000 / m_incubator_capacity;
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

    // Loop through commands table
    for (uint8_t i = 0; i < COMMANDS_COUNT; i++)
    {
        // Compare name
        if (!(p_command->getName()->compare(commands_table[i].name)))
        {
            // Compare argument count
            if (p_command->getArgumentCount() ==
                commands_table[i].argument_count)
            {
                // Execute function
                float arg_1 = (*p_command)[0];
                float arg_2 = (*p_command)[1];
                int channel = (*p_command)[2];
                bool b_success =
                    (this->*(commands_table[i].execute))(arg_1, arg_2, channel);
                if (b_success == false)
                {
                    p_comchannel->send("Arguments out of bounds\n");
                }
                b_command_recognised = true;
            }
            else
            {
                p_comchannel->send("Argument count check failed\n");
            }
        }
    }

    return b_command_recognised;
}

/**
 * Command to modify mock hardware ambient temperature
 * >setambient(20);
 */
bool CMockHardwareMap::setambient(float temperature)
{
    bool b_success = false;
    if ((temperature > MIN_TEMP) || (temperature < MAX_TEMP))
    {
        m_ambient_temperature = temperature;
        b_success = true;
    }

    return b_success;
}

/**
 * Command to modify incubator parameters. Incubator heat capacity and heat
 * loss rate to ambient.
 * >setincubator(incubator_capacity, incubator_conductance)
 */
bool CMockHardwareMap::setincubator(float capacity, float loss)
{
    bool b_success = false;
    if (capacity >= 0 || loss >= 0)
    {
        m_incubator_capacity = capacity;
        m_incubator_loss = loss;
        b_success = true;
    }
    return b_success;
}

void CMockHardwareMap::reset()
{
    mb_power_enable = false;
    m_ambient_temperature = AMBIENT_T;
    for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
    {
        m_heater_power[i] = 0;
        m_heater_rating[i] = HEATER_RATING;
        m_temperature[i][0] = AMBIENT_T;
        m_temperature[i][1] = AMBIENT_T;
        m_heat_capacity[i][0] = HEATER_CAPACITY;
        m_heat_capacity[i][1] = RADIATOR_CAPACITY;
        m_heat_conductance[i][0] = HEATER_CONDUCTANCE;
        m_heat_conductance[i][1] = RADIATOR_CONDUCTANCE;
    }
    m_incubator_temperature = m_ambient_temperature;
    m_incubator_capacity = AMBIENT_T;
    m_incubator_loss = INCUBATOR_LOSS;
    m_control_current = 0;
}

bool CMockHardwareMap::setrating(float rating, uint8_t channel)
{
    bool b_success = false;
    if ((rating >= 0) || (channel <= HARD_PWM_OUTPUTS))
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
        b_success = true;
    }

    return b_success;
}

bool CMockHardwareMap::setcapacity(float heater_capacity,
                                   float radiator_capacity,
                                   uint8_t channel)
{
    bool b_success = false;
    if ((heater_capacity >= 0) || (radiator_capacity >= 0) ||
        (channel <= HARD_PWM_OUTPUTS))
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
        b_success = true;
    }
    return b_success;
}

bool CMockHardwareMap::setconductance(float heater_conductance,
                                      float radiator_conductance,
                                      uint8_t channel)
{
    bool b_success = false;
    if ((heater_conductance >= 0) || (radiator_conductance >= 0) ||
        (channel <= HARD_PWM_OUTPUTS))
    {
        if (channel == 0)
        {
            for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
            {
                m_heat_conductance[i][0] = heater_conductance;
                m_heat_conductance[i][1] = radiator_conductance;
            }
        }
        else
        {
            m_heat_conductance[channel - 1][0] = heater_conductance;
            m_heat_conductance[channel - 1][1] = radiator_conductance;
        }
        b_success = true;
    }
    return b_success;
}

// Wrappers for function pointer
bool CMockHardwareMap::setratingwrapper(float x, float y, uint8_t z)
{
    return setrating(x, (uint8_t)y);
}
bool CMockHardwareMap::setambientwrapper(float x, float y, uint8_t z)
{
    return setambient(x);
}
bool CMockHardwareMap::setincubatortwrapper(float x, float y, uint8_t z)
{
    return setincubator(x, y);
}
