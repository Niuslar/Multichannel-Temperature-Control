/*
 * IComChannel.h
 *
 *  Created on: 16 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef INC_ICOMCHANNEL_H_
#define INC_ICOMCHANNEL_H_

#include <string>

class IComChannel
{
public:
    IComChannel(std::string name) : m_name(name){};

    std::string getName() const
    {
        return m_name;
    };

    virtual bool isCommandAvailable() = 0;
    virtual std::string getCommand() = 0;
    virtual void send(const std::string &message) = 0;

private:
    std::string const m_name;
};

#endif /* INC_ICOMCHANNEL_H_ */
