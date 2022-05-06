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

#define COMMANDS_COUNT 5

class CMockHardwareMap : public IHardwareMap, public CController
{
public:
    CMockHardwareMap(etl::string<MAX_STRING_SIZE> name, uint32_t run_period_ms);
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
    /* CController methods. */
    //    etl::string<MAX_STRING_SIZE> getName() const;
    //    virtual bool tick(uint32_t current_time);
    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset();
    //    virtual void stop();
    //    virtual void start();

private:
    typedef bool (CMockHardwareMap::*function_pointer_t)(float, float, uint8_t);
    typedef struct COMMAND_T
    {
        etl::string<MAX_STRING_SIZE> name;
        uint8_t argument_count;
        function_pointer_t execute;
    } command_t;
    bool setrating(float rating, uint8_t channel);
    bool setcapacity(float heater_capacity,
                     float radiator_capacity,
                     uint8_t channel);
    bool setconductance(float heater_conductance,
                        float radiator_conductance,
                        uint8_t channel);
    bool setambient(float temperature);
    bool setincubator(float capacity, float loss);

    // Wrappers for function pointer
    bool setratingwrapper(float x, float y, uint8_t z);
    bool setambientwrapper(float x, float y, uint8_t z);
    bool setincubatortwrapper(float x, float y, uint8_t z);

    bool mb_power_enable;
    /* Modelling parameters for heater and incubator heat transfer.
     * Two stage heat transfer
     * power -> heater -> radiator -> incubator
     */
    float m_ambient_temperature;
    float m_heater_power[HARD_PWM_OUTPUTS];
    float m_heater_rating[HARD_PWM_OUTPUTS];
    float m_temperature[HARD_PWM_OUTPUTS][2];
    float m_heat_capacity[HARD_PWM_OUTPUTS][2];
    float m_heat_conductance[HARD_PWM_OUTPUTS][2];
    float m_incubator_temperature;
    float m_incubator_capacity;  // heat capacity of incubator
    float m_incubator_loss;      // rate of heat loss to ambient air.
    float m_control_current;     // total current flowing through the heaters.

    // Table of commands
    command_t commands_table[COMMANDS_COUNT] = {
        {"setrating", 2, &CMockHardwareMap::setratingwrapper},
        {"setcapacity", 3, &CMockHardwareMap::setcapacity},
        {"setconductance", 3, &CMockHardwareMap::setconductance},
        {"setambient", 1, &CMockHardwareMap::setambientwrapper},
        {"setincubator", 2, &CMockHardwareMap::setincubatortwrapper}};
};

#endif /* CMOCKHARDWAREMAP_H_ */
