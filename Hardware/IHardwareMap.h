/**
 * @file IHardwareMap.h
 *
 */

/*
 * IHardwareMap.h
 *
 *  Created on: 30 Apr 2022
 *      Author: salavat.magazov
 */

#ifndef IHARDWAREMAP_H_
#define IHARDWAREMAP_H_

class IHardwareMap
{
public:
    IHardwareMap();
    virtual void init() = 0;

    /**
     * @note Any type of access that is required by the controllers to the
     * underlying peripherals must be done via this interface class.
     * If access is required, then the procedure is to create a virtual method
     * here, then add corresponding implementations in all child classes.
     */
};

#endif /* IHARDWAREMAP_H_ */
