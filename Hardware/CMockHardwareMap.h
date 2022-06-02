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

#include "IHardwareMap.h"

class CMockHardwareMap : public IHardwareMap
{
public:
    CMockHardwareMap();

    virtual void init();
};

#endif /* CMOCKHARDWAREMAP_H_ */
