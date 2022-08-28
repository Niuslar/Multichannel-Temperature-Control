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
#include "CPIDLoop.h"
#include "IHardwareMap.h"

#define CHANNEL_NUMBER 8

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
    void sendStatus(IComChannel *p_comchannel);
    ICommand::command_error_code_t setHumidity(ICommand *p_command);
    ICommand::command_error_code_t overrideHeater(ICommand *p_command);

    IHardwareMap *mp_hw;

    CBME280 *hum_sensor;

    float target_humidity;
    float m_power_override[CHANNEL_NUMBER];
    CPIDLoop m_control_loop[CHANNEL_NUMBER];
};

#endif /* CHUMIDITYCONTROLLER_H_ */
