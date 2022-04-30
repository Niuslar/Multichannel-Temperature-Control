/**
 * @file CTemperatureController.h
 *
 */

/*
 * CTemperatureController.h
 *
 *  Created on: 26 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef CTEMPERATURECONTROLLER_H_
#define CTEMPERATURECONTROLLER_H_

#include "CController.h"
#include "CHeater.h"

#define CHANNEL_NUMBER 10

class CTemperatureController : public CController
{
public:
    CTemperatureController(etl::string<MAX_STRING_SIZE> name,
                           uint32_t run_period_ms);

    virtual void run();
    virtual bool newCommand(ICommand *p_command, IComChannel *p_comchannel);
    virtual void reset() = 0;

private:
    void getStatus();
    void setTemperature(float target_temperature, uint8_t channel = 0);
    void overrideHeater(float power, uint8_t channel = 0);

    CHeater m_heaters[CHANNEL_NUMBER];
};

#endif /* CTEMPERATURECONTROLLER_H_ */
