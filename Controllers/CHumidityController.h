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
#include "CHumidifier.h"
#include "CPIDLoop.h"
#include "IHardwareMap.h"

class CHumidityController : public CController
{
public:
    CHumidityController(IHardwareMap *p_hardwaremap,
                        etl::string<MAX_STRING_SIZE> name,
                        uint32_t run_period_ms,
                        SPI_HandleTypeDef *p_spi,
                        GPIO_TypeDef *p_slave_select_port,
                        uint16_t slave_select_pin);
    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset();

private:
    static constexpr uint8_t CHANNEL_NUMBER = 1;
    static constexpr uint8_t MAX_HUMIDITY = 95;
    static constexpr uint8_t MIN_HUMIDITY = 85;
    static constexpr uint8_t MAX_POWER = 100;
    static constexpr uint8_t MIN_POWER = 0;
    static constexpr uint8_t DISABLE_OVERRIDE = -1;
    static constexpr uint8_t DISABLE_TARGET = 0;
    void sendStatus(IComChannel *p_comchannel);
    ICommand::command_error_code_t setHumidity(ICommand *p_command);
    ICommand::command_error_code_t overrideHumidifier(ICommand *p_command);

    IHardwareMap *mp_hw;

    CBME280 *mp_humidity_sensor;

    float m_target_humidity;
    float m_power_override;
    CPIDLoop m_control_loop;
};

#endif /* CHUMIDITYCONTROLLER_H_ */
