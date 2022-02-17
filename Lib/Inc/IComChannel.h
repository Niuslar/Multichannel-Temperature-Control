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
    // TODO: methods of this class are to be decided.

    std::string getName() const
    {
        return m_name;
    };

private:
    std::string m_name;
};

#endif /* INC_ICOMCHANNEL_H_ */
