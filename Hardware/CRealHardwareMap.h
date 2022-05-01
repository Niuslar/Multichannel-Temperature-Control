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

private:
    CAdcData m_adc;
};

#endif /* CREALHARDWAREMAP_H_ */
