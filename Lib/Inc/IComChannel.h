/**
 * @file IComChannel.h
 *
 */

/*
 * IComChannel.h
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef ICOMCHANNEL_H_
#define ICOMCHANNEL_H_

#include "../etl/string.h"

class IComChannel
{
public:
    IComChannel(etl::string<MAX_STRING_SIZE> name) : m_name(name){};

    etl::string<MAX_STRING_SIZE> getName() const
    {
        return m_name;
    };

    virtual bool isDataAvailable() = 0;
    virtual etl::string<MAX_STRING_SIZE> getData() = 0;
    virtual bool send(etl::string<MAX_STRING_SIZE> message) = 0;

private:
    etl::string<MAX_STRING_SIZE> const m_name;
};

#endif /* ICOMCHANNEL_H_ */
