/**
 * @file CTemperatureController.cpp
 *
 */

/*
 * CTemperatureController.cpp
 *
 *  Created on: 26 Apr 2022
 *      Author: salavat.magazov
 */

#include <CTemperatureController.h>

CTemperatureController::CTemperatureController(
    etl::string<MAX_STRING_SIZE> name,
    uint32_t run_period_ms)
    : CController(name, run_period_ms)
{
}

void CTemperatureController::run() {}

bool CTemperatureController::newCommand(etl::string<MAX_STRING_SIZE> command,
                                        IComChannel *p_comchannel)
{
}
void CTemperatureController::reset() {}
