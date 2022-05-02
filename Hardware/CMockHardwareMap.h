/**
 * @file CMockHardwareMap.h
 *
 */

/*
 * CMockHardwareMap.h
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef CMOCKHARDWAREMAP_H_
#define CMOCKHARDWAREMAP_H_

#include "CController.h"
#include "IHardwareMap.h"

class CMockHardwareMap : public IHardwareMap, public CController
{
public:
    CMockHardwareMap(etl::string<MAX_STRING_SIZE> name, uint32_t run_period_ms);
    virtual void init();
    virtual float getInputVoltage() const;
    virtual float getTotalCurrent() const;
    virtual float getControlCurrent() const;
    virtual float getAmbientTemp() const;
    virtual float getChanneTemp(uint8_t channel) const;
    virtual float setHardPwmOutput(float power, uint8_t channel);
    virtual float getHardPwmOutput(uint8_t channel);
#ifdef SOFT_PWM_OUTPUTS
    virtual float setSoftPwmOutput(float power, uint8_t channel) = 0;
    virtual float getSoftPwmOutput(uint8_t channel) = 0;
#endif
    virtual void setBreathingLight(float duty_cycle);
    virtual void enableControlPower(bool b_enable);
    /* CController methods. */
    //    etl::string<MAX_STRING_SIZE> getName() const;
    //    virtual bool tick(uint32_t current_time);
    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset();
    //    virtual void stop();
    //    virtual void start();

private:
    float m_heater_power[HARD_PWM_OUTPUTS];
    float m_temperature[HARD_PWM_OUTPUTS];
    float m_incubator_temperature;
    float m_ambient_temperature;
    bool mb_power_enable;
};

#endif /* CMOCKHARDWAREMAP_H_ */
