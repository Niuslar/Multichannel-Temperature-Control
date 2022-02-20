/*
 * CSoftPwmOutput.cpp
 *
 *  Created on: 20 Feb 2022
 *      Author: salavat.magazov
 */

#include "CSoftPwmOutput.h"

TIM_HandleTypeDef *CSoftPwmOutput::sp_timer = nullptr;
uint32_t CSoftPwmOutput::s_period = 100;
uint32_t CSoftPwmOutput::s_counter = 0;
CSoftPwmOutput *CSoftPwmOutput::sp_first_instance = nullptr;

CSoftPwmOutput::CSoftPwmOutput(CGpioWrapper *p_gpio)
    : mp_next_instance(nullptr),
      mp_gpio(p_gpio),
      m_duty_cycle(0)
{
    if (sp_first_instance == nullptr)
    {
        sp_first_instance = this;
    }
    else
    {
        CSoftPwmOutput *p_previous_instance = getLastInstance();
        p_previous_instance->mp_next_instance = this;
    }
    mp_next_instance = nullptr;
    p_gpio->set(false);
}

void CSoftPwmOutput::setDutyCycle(float duty_cycle_percent)
{
    duty_cycle_percent *= s_period + 1;
    m_duty_cycle = duty_cycle_percent / 100;
    if (m_duty_cycle > s_period)
    {
        m_duty_cycle = s_period;
    }
}

float CSoftPwmOutput::getDutyCycle() const
{
    return (m_duty_cycle / s_period);
}

void CSoftPwmOutput::init(uint32_t period, TIM_HandleTypeDef *p_timer)
{
    if (p_timer != nullptr)
    {
        HAL_TIM_RegisterCallback(sp_timer,
                                 HAL_TIM_PERIOD_ELAPSED_CB_ID,
                                 softPwmTimerTick);
    }
    sp_timer = p_timer;
    s_period = period;
}

void CSoftPwmOutput::tick()
{
    if (++s_counter >= s_period)
    {
        s_counter = 0;
    }
    CSoftPwmOutput *p_instance = sp_first_instance;
    while (p_instance != nullptr)
    {
        bool b_output = false;
        if (s_counter < p_instance->m_duty_cycle)
        {
            b_output = true;
        }
        p_instance->mp_gpio->set(b_output);
        p_instance = p_instance->mp_next_instance;
    }
}

CSoftPwmOutput *CSoftPwmOutput::getLastInstance()
{
    CSoftPwmOutput *p_instance = sp_first_instance;
    while (p_instance->mp_next_instance != nullptr)
    {
        p_instance = p_instance->mp_next_instance;
    }
    return p_instance;
}
