/**
 * @file CStringParser.h
 *
 */

/*
 * CStringParser.h
 *
 *  Created on: 15 May 2022
 *      Author: salavat.magazov
 */

/**
 * @brief This class is for simple string message based commands. It is
 * distinctly different from JSON parser in that the entire command is a single
 * line and contains very little special characters.
 * General command structure:
 *
 * command_name(arg1, arg2,...)
 *
 * command_name - is a string value that denotes the intent of the command.
 * arg1 - is the first argument in the command. This argument is the only one
 * that can be either a number or a string.
 * arg2, arg3,... - are subsequent arguments in the command. All arguments
 * beyond first must be a number. They can be either an integer or a float.
 */

#ifndef CSTRINGPARSER_H_
#define CSTRINGPARSER_H_

#include "ICommand.h"

class CStringParser : public ICommand
{
public:
    CStringParser();
    virtual ~CStringParser();

    virtual bool parse(const etl::string<MAX_STRING_SIZE> &string);
    virtual void reset();

    /* implementations of abstract class methods. */
    const etl::string<MAX_STRING_SIZE> *getName() const;
    unsigned int getArgumentCount() const;
    const etl::string<MAX_STRING_SIZE> *getStringArgument() const;
    float operator[](unsigned int index);

    typedef enum PARSE_STATE_T
    {
        IDLE = 0,
        NAME,
        ARGUMENTS,
        FINISH
    } parse_state_t;

protected:
    uint8_t m_argument_counter;
    etl::string<MAX_STRING_SIZE> m_command_name;
    float m_arguments[MAX_ARGUMENT_COUNT];
    etl::string<MAX_STRING_SIZE> m_string_argument;

private:
    bool isValidFormat(const etl::string<MAX_STRING_SIZE> &string) const;

    parse_state_t m_parse_state;
};

#endif /* CSTRINGPARSER_H_ */
