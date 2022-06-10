/**
 * @file CRealHardwareMap.cpp
 *
 */

/*
 * CRealHardwareMap.cpp
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#include "CRealHardwareMap.h"
#include "gpio.h"
#include "tim.h"

/**
 * @note All of these parameters are subject to PCB design layout. Refer to
 * specific PCB this code will run on for correct information on what
 * peripherals are being used.
 */
#define V_REF 5.0

#define EVAPORATOR_T_CHANNEL    8
#define INPUT_VOLTAGE_CHANNEL   9
#define TOTAL_CURRENT_CHANNEL   10
#define CONTROL_CURRENT_CHANNEL 11
#define AMBIENT_TEMP_CHANNEL    12

#define INPUT_VOLTAGE_SCALE   ((28.0 + 3.01) / 3.01)  // voltage divider on PCB
#define TOTAL_CURRENT_SCALE   (750E-6 * 200)  // 750uOhm resistor 200x amplifier
#define CONTROL_CURRENT_SCALE (750E-6 * 200)
#define AD22100_SCALE         ((V_REF / 5.0) * (1 / 22.5E-3))
#define AD22100_OFFSET        ((V_REF / 5.0) * (-1.375 / 22.5E-3))

const CRealHardwareMap::timer_init_map_t CRealHardwareMap::s_timer_init_map[] =
    {{&htim1, TIM_CHANNEL_1},
     {&htim4, TIM_CHANNEL_1},
     {&htim2, TIM_CHANNEL_2},
     {&htim1, TIM_CHANNEL_4},
     {&htim3, TIM_CHANNEL_1},
     {&htim3, TIM_CHANNEL_2},
     {&htim3, TIM_CHANNEL_3},
     {&htim3, TIM_CHANNEL_4}};

const CRealHardwareMap::gpio_init_map_t CRealHardwareMap::s_gpio_init_map[] = {
    {PWM_1_GPIO_Port, PWM_1_Pin},
    {PWM_2_GPIO_Port, PWM_2_Pin},
    {PWM_3_GPIO_Port, PWM_3_Pin},
    {PWM_4_GPIO_Port, PWM_4_Pin},
    {PWM_5_GPIO_Port, PWM_5_Pin},
    {PWM_6_GPIO_Port, PWM_6_Pin},
    {PWM_7_GPIO_Port, PWM_7_Pin},
    {PWM_8_GPIO_Port, PWM_8_Pin}};

CRealHardwareMap::CRealHardwareMap() : m_adc(&hadc1)
{
    // TODO Auto-generated constructor stub
}

/**
 * @brief Initialise hardware peripherals.
 * @note This method must be called as it sets up all hardware that cannot be
 * setup at construction time. Example: timers and ADC.
 *
 */
void CRealHardwareMap::init()
{
    /* all hardware initialisation of peripherals goes here. */
    m_adc.init();
    for (int i = 0; i < HARD_PWM_OUTPUTS; i++)
    {
        m_hard_pwm_output[i].init(s_timer_init_map[i].p_timer,
                                  s_timer_init_map[i].channel);
        m_polarity_switch[i].init(s_gpio_init_map[i].p_port,
                                  s_gpio_init_map[i].pin);
    }
    m_breathing_light.init(&htim2, TIM_CHANNEL_1);
    m_power_enable.init(BREATHING_GPIO_Port, BREATHING_Pin);
}

/**
 * @brief Get instantaneous measure of input voltage.
 *
 * @return Input power supply voltage in [Volt].
 */
float CRealHardwareMap::getInputVoltage() const
{
    return m_adc[INPUT_VOLTAGE_CHANNEL] * INPUT_VOLTAGE_SCALE;
}

/**
 * @brief Get instantaneous measure of total consumed current.
 *
 * @return Instant total current in [Amp].
 */
float CRealHardwareMap::getTotalCurrent() const
{
    return m_adc[TOTAL_CURRENT_CHANNEL] * TOTAL_CURRENT_SCALE;
}

/**
 * @brief Get instantaneous measure of current consumed by control peripherals.
 *
 * @return Instant control current in [Amp].
 */
float CRealHardwareMap::getControlCurrent() const
{
    return m_adc[CONTROL_CURRENT_CHANNEL] * CONTROL_CURRENT_SCALE;
}

/**
 * @brief Get ambient temperature from on-board sensor.
 *
 * @return Instant temperature from sensor mounted on the PCB.
 */
float CRealHardwareMap::getAmbientTemp() const
{
    float temperature = m_adc[AMBIENT_TEMP_CHANNEL];
    temperature *= AD22100_SCALE;
    temperature += AD22100_OFFSET;
    return temperature;
}

/**
 * @brief Get temperature for off-board thermistor sensor.
 *
 * @param channel Number of the channel to query.
 * @return Temperature for requested channel in [degC].
 */
float CRealHardwareMap::getChannelTemp(uint8_t channel) const
{
    float temperature = m_adc[channel];
    // TODO: calibration for thermistor goes here.
    return temperature;
}

float CRealHardwareMap::setHardPwmOutput(float duty_cycle_percent,
                                         uint8_t channel)
{
    float duty_cycle = 0;
    if (channel < HARD_PWM_OUTPUTS)
    {
        // request for hard PWM output
        m_hard_pwm_output[channel].setDutyCycle(duty_cycle_percent);
        duty_cycle = m_hard_pwm_output[channel].getDutyCycle();
    }
    return duty_cycle;
}

float CRealHardwareMap::getHardPwmOutput(uint8_t channel)
{
    float duty_cycle = 0;
    if (channel < HARD_PWM_OUTPUTS)
    {
        duty_cycle = m_hard_pwm_output[channel].getDutyCycle();
    }
    return duty_cycle;
}

void CRealHardwareMap::setBreathingLight(float duty_cycle)
{
    m_breathing_light.setDutyCycle(duty_cycle);
}

void CRealHardwareMap::enableControlPower(bool b_enable)
{
    m_power_enable.set(b_enable);
}
