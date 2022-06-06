/**
 * @file CRealHardwareMap.h
 *
 */

/*
 * CRealHardwareMap.h
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef CREALHARDWAREMAP_H_
#define CREALHARDWAREMAP_H_

#include "CAdcData.h"
#include "CGpioWrapper.h"
#include "CHardPwmOutput.h"
#include "IHardwareMap.h"

#ifdef SOFT_PWM_OUTPUTS
#    include "CSoftPwmOutput.h"
#endif

class CRealHardwareMap : public IHardwareMap
{
public:
    CRealHardwareMap();
    virtual void init();
    virtual float getInputVoltage() const;
    virtual float getTotalCurrent() const;
    virtual float getControlCurrent() const;
    virtual float getAmbientTemp() const;
    virtual float getChannelTemp(uint8_t channel) const;
    virtual float setHardPwmOutput(float power, uint8_t channel);
    virtual float getHardPwmOutput(uint8_t channel);
#ifdef SOFT_PWM_OUTPUTS
    virtual float setSoftPwmOutput(float power, uint8_t channel) = 0;
    virtual float getSoftPwmOutput(uint8_t channel) = 0;
#endif
    virtual void setBreathingLight(float duty_cycle);
    virtual void enableControlPower(bool b_enable);
    virtual void triggerADC();

private:
    typedef struct TIMER_INIT_MAP_T
    {
        TIM_HandleTypeDef *p_timer;
        uint8_t channel;
    } timer_init_map_t;
    typedef struct GPIO_INIT_MAP_T
    {
        GPIO_TypeDef *p_port;
        uint16_t pin;
    } gpio_init_map_t;

    static const timer_init_map_t s_timer_init_map[HARD_PWM_OUTPUTS];
    static const gpio_init_map_t s_gpio_init_map[HARD_PWM_OUTPUTS];
    CAdcData m_adc;
    CHardPwmOutput m_hard_pwm_output[HARD_PWM_OUTPUTS];
    CGpioWrapper m_polarity_switch[HARD_PWM_OUTPUTS];
    CHardPwmOutput m_breathing_light;
    CGpioWrapper m_power_enable;
#ifdef SOFT_PWM_OUTPUTS
    CSoftPwmOutput m_soft_pwm_output[SOFT_PWM_OUTPUTS];
#endif
};

#endif /* CREALHARDWAREMAP_H_ */
