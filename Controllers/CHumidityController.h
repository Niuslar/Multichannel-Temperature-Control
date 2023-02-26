/*
 * CHumidityController.h
 *
 *  Created on: Aug. 11, 2022
 *      Author: samk
 */

#ifndef CHUMIDITYCONTROLLER_H_
#define CHUMIDITYCONTROLLER_H_

#include "CBME280.h"
#include "CController.h"
#include "CHumidifier.h"  //TODO: Sam, this is not used since we delegate hardware map to take care of humidifier control.
#include "CPIDLoop.h"
#include "IHardwareMap.h"

class CHumidityController : public CController
{
public:
    CHumidityController(IHardwareMap *p_hardwaremap,
                        etl::string<MAX_STRING_SIZE> name,
                        uint32_t run_period_ms);
    virtual bool tick(uint32_t current_time);
    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset();

    static constexpr uint8_t MAX_HUMIDITY = 95;
    static constexpr uint8_t MIN_HUMIDITY = 85;
    static constexpr uint8_t MAX_POWER = 100;
    static constexpr uint8_t MIN_POWER = 0;
    static constexpr uint8_t DISABLE_OVERRIDE = -1;
    static constexpr uint8_t DISABLE_TARGET = 0;
    static constexpr uint32_t HUMIDIFIER_PWM_PERIOD = 10000;

private:
    void sendStatus(IComChannel *p_comchannel);
    ICommand::command_error_code_t setHumidity(ICommand *p_command);
    ICommand::command_error_code_t overrideHumidifier(ICommand *p_command);

    void runHumidifier(uint32_t current_time);

    IHardwareMap *mp_hw;

    CBME280 *mp_humidity_sensor;

    float m_target_humidity;
    float m_power_override;
    CPIDLoop m_control_loop;

    float m_humidifier_power;
    uint32_t m_humidifier_cycle_timestamp;
};

#endif /* CHUMIDITYCONTROLLER_H_ */
