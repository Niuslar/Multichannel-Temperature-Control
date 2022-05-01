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

#include "IHardwareMap.h"

#include "CAdcData.h"
#include "CHardPwmOutput.h"

class CRealHardwareMap : public IHardwareMap
{
public:
    CRealHardwareMap();

    virtual void init();

    virtual float getInputVoltage() const;
    virtual float getTotalCurrent() const;
    virtual float getControlCurrent() const;
    virtual float getAmbientTemp() const;
    virtual float getChanneTemp(uint8_t channel) const;

    virtual float setPwmOutput(float power, uint8_t channel);

private:
    CAdcData m_adc;
    typedef struct TIMER_INIT_MAP_T
    {
        TIM_HandleTypeDef *p_timer;
        uint8_t channel;
    } timer_init_map_t;
    static const timer_init_map_t s_timer_init_map[HARD_PWM_OUTPUTS];
    CHardPwmOutput m_pwm_output[HARD_PWM_OUTPUTS];
};

#endif /* CREALHARDWAREMAP_H_ */
