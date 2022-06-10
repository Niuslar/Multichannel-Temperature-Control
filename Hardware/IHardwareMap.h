/**
 * @file IHardwareMap.h
 *
 */

/*
 * IHardwareMap.h
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef IHARDWAREMAP_H_
#define IHARDWAREMAP_H_

#include "stdint.h"

#define HARD_PWM_OUTPUTS 8
/**
 * @note Declare this value to the number of soft PWM outputs that are needed.
 */
//#define SOFT_PWM_OUTPUTS 0

class IHardwareMap
{
public:
    virtual void init() = 0;
    /**
     * @note Any type of access that is required by the controllers to the
     * underlying peripherals must be done via this interface class.
     * If access is required, then the procedure is to create a virtual method
     * here, then add corresponding implementations in all child classes.
     * @note Keep in mind that specific details are most likely implementation
     * dependent. Example: telemetry like voltage and current values might be
     * instantaneous or digitally filtered.
     */
    /* ADC access */
    /**
     * @brief Get input voltage.
     *
     * @return Input voltage in [V].
     */
    virtual float getInputVoltage() const = 0;
    /**
     * @brief Get total consumed current. This is essentially control current +
     * whatever is being consumed by on-board logic.
     *
     * @return Total current in [Amp]
     */
    virtual float getTotalCurrent() const = 0;
    /**
     * @brief Get control current. This is current used to power
     * external power control peripherals.
     *
     * @return Control current in [Amp]
     */
    virtual float getControlCurrent() const = 0;
    /**
     * @brief Get ambient temperature.
     *
     * @return Temperature in the immediate vicinity of the PCB in [degC]
     */
    virtual float getAmbientTemp() const = 0;
    /**
     * @brief Get temperature of specific thermistor channel.
     *
     * @param channel Number of the channel requested.
     * @return Temperature of the thermistor in [degC]
     */
    virtual float getChannelTemp(uint8_t channel) const = 0;
    /* PWM control */
    /**
     * @brief Set power output of a specific hardware PWM control.
     *
     * @param power Value of power output in percent.
     * @param channel Number of the control channel to set.
     * @return Value of power output as set by the command in [%]. Might be
     * range limited.
     */
    virtual float setHardPwmOutput(float power, uint8_t channel) = 0;
    /**
     * @brief Get power output for a control output
     *
     * @param channel Number of the control channel to set.
     * @return Value of power output as set by the command in [%].
     */
    virtual float getHardPwmOutput(uint8_t channel) = 0;
#ifdef SOFT_PWM_OUTPUTS
    /**
     * @brief Set power output of a specific software PWM control.
     * @note Software PWM are significantly slower than hardware PWM, so only
     * suitable for slow control.
     * @param power Value of power output in percent.
     * @param channel Number of the control channel to set.
     * @return Value of power output as set by the command in [%]. Might be
     * range limited.
     */
    virtual float setSoftPwmOutput(float power, uint8_t channel) = 0;
    /**
     * @brief Get power output for a control output
     *
     * @param channel Number of the control channel to set.
     * @return Value of power output as set by the command in [%].
     */
    virtual float getSoftPwmOutput(uint8_t channel) = 0;
#endif
    /**
     * @brief Set state of the breathing light. This light is meant to show code
     * execution state.
     * @note The actual brightness might or might not be perception corrected.
     * @param duty_cycle Value in [%] to set brightness of LED.
     */
    virtual void setBreathingLight(float duty_cycle) = 0;
    /**
     * @brief Enable/disable control power to control outputs
     *
     * @param b_enable Set to true to enable, false to disable.
     */
    virtual void enableControlPower(bool b_enable) = 0;
};

#endif /* IHARDWAREMAP_H_ */
