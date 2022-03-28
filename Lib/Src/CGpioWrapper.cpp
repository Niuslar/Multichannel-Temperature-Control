/**
 * @file CGpioWrapper.cpp
 *
 *  Created on: 14 Feb 2022
 *      Author: salavat.magazov
 */

#include "CGpioWrapper.h"

/**
 * @brief Create instance of a GPIO pin wrapper.
 *
 * @param p_port Pointer to GPIO port.
 * @param pin Pin mask as defined by HAL.
 */
CGpioWrapper::CGpioWrapper(GPIO_TypeDef *p_port, uint16_t pin)
    : mp_port(p_port),
      m_pin(pin)
{
    if (mp_port == NULL)
    {
        Error_Handler();
    }
    /* test if this is a valid pin mask, i.e. one and only one bit is set. */
    if (!(m_pin && !(m_pin & (m_pin - 1))))
    {
        Error_Handler();
    }
}

/**
 * @brief Get the state of the pin.
 *
 * @return True if pin is high, False if it is low.
 */
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

/**
 * @brief Set the value of the pin.
 *
 * @param value Pass True is pin to be set high, False to be set low.
 */
void CGpioWrapper::set(bool value)
{
    HAL_GPIO_WritePin(mp_port, m_pin, (value ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

/**
 * @brief Toggle the pin from high to low or from low to high.
 */
void CGpioWrapper::toggle()
{
    HAL_GPIO_TogglePin(mp_port, m_pin);
}
