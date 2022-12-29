/**
 * @file CHumidifier.h
 *
 */

/*
 * CHumidifier.h
 *
 *  Created on: 26 Sep 2022
 *      Author: salavat.magazov
 */

#ifndef CHUMIDIFIER_H_
#define CHUMIDIFIER_H_

class CHumidifier
{
public:
    CHumidifier();
    virtual ~CHumidifier();

    void run();

    void addVapour(float quantity);
};

#endif /* CHUMIDIFIER_H_ */
