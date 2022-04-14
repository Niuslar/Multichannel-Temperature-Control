/**
 * @file CGpioWrapper.cpp
 */
/*
 *  Created on: 14 Feb 2022
 *      Author: salavat.magazov
 */

#include "CGpioWrapper.h"

/**
 * @brief Default c-tor, creates virtual pin
 */
CGpioWrapper::CGpioWrapper() : mp_port(nullptr), m_pin(100), mb_state(false) {}

/**
 * @brief Create instance of a GPIO pin wrapper.
 * @param p_port Pointer to GPIO port.
 * @param pin Pin mask as defined by HAL.
 * @param init_state
 */
CGpioWrapper::CGpioWrapper(GPIO_TypeDef *p_port, uint16_t pin, bool init_state)
    : mp_port(p_port),
      m_pin(pin),
      mb_state(init_state)
{
    if (mp_port != nullptr)
    {
        /* test if this is a valid pin mask, i.e. one and only one bit is set.
         */
        if (!(m_pin && !(m_pin & (m_pin - 1))))
        {
            Error_Handler();
        }
    }
}

/**
 * @brief Initialise class with real hardware or turn GPIO pin into virtual pin.
 * @param p_port Pointer to GPIO port. If nullptr, then this becomes virtual
 * GPIO pin.
 * @param pin Pin mask as defined by HAL. If p_port is non-nullptr, pin _must_
 * be valid.
 * @return True if initialisation is successful, false if failed.
 * @note Do not attempt to write into improperly initialised GPIO.
 */
bool CGpioWrapper::init(GPIO_TypeDef *p_port, uint16_t pin)
{
    bool b_success = true;
    if (p_port != nullptr)
    {
        /* test if this is a valid pin mask, i.e. one and only one bit is set.
         */
        if (!(pin && !(pin & (pin - 1))))
        {
            Error_Handler();
            b_success = false;
        }
        else
        {
            mp_port = p_port;
            m_pin = pin;
        }
    }
    return b_success;
}

/**
 * @brief Get the state of the pin.
 *
 * @return True if pin is high, False if it is low.
 */
bool CGpioWrapper::get()
{
    if (mp_port == nullptr)
    {
        return mb_state;
    }
    else
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
}

/**
 * @brief Set the value of the pin.
 *
 * @param value Pass True is pin to be set high, False to be set low.
 */
void CGpioWrapper::set(bool value)
{
    if (mp_port == nullptr)
    {
        mb_state = value;
    }
    else
    {
        HAL_GPIO_WritePin(mp_port,
                          m_pin,
                          (value ? GPIO_PIN_SET : GPIO_PIN_RESET));
    }
}

/**
 * @brief Toggle the pin from high to low or from low to high.
 */
void CGpioWrapper::toggle()
{
    if (mp_port == nullptr)
    {
        mb_state = !mb_state;
    }
    else
    {
        HAL_GPIO_TogglePin(mp_port, m_pin);
    }
}
