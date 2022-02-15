/*
 * CGpioWrapper.cpp
 *
 *  Created on: 14 Feb 2022
 *      Author: salavat.magazov
 */

#include "CGpioWrapper.h"

CGpioWrapper::CGpioWrapper(GPIO_TypeDef *p_port, uint32_t pin)
    : mp_port(p_port),
      m_pin(pin)
{
    if (mp_port == NULL)
    {
        Error_Handler();
    }
    if (!IS_GPIO_PIN(m_pin))
    {
        Error_Handler();
    }
}

bool CGpioWrapper::get()
{
    if (HAL_GPIO_ReadPin(mp_port, m_pin) == GPIO_PIN_SET)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void CGpioWrapper::set(bool value)
{
    HAL_GPIO_WritePin(mp_port, m_pin, (value ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

void CGpioWrapper::toggle()
{
    HAL_GPIO_TogglePin(mp_port, m_pin);
}
