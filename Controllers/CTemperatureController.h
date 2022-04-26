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

class CTemperatureController : public CController
{
public:
    CTemperatureController(etl::string<MAX_STRING_SIZE> name, uint32_t run_period_ms);
    virtual ~CTemperatureController();
};

#endif /* CTEMPERATURECONTROLLER_H_ */
