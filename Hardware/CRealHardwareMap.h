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

class CRealHardwareMap : public IHardwareMap
{
public:
    CRealHardwareMap();

    virtual void init();
};

#endif /* CREALHARDWAREMAP_H_ */
