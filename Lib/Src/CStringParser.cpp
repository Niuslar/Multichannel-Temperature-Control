/**
 * @file CStringParser.cpp
 *
 */

/*
 * CStringParser.cpp
 *
 *  Created on: 15 May 2022
 *      Author: salavat.magazov
 */

#include "CStringParser.h"

CStringParser::CStringParser()
{
    reset();
}

CStringParser::~CStringParser()
{
    // TODO Auto-generated destructor stub
}

/**
 * @brief Parse plain string command into a command structure.
 * @param String to be parsed.
 * @return True if valid command has been recognised.
 */
bool CStringParser::parse(const etl::string<MAX_STRING_SIZE> &string)
{
    bool b_success = false;
    reset();
    // TODO: here goes implementation of string parsing.
    return b_success;
}

/**
 * @brief Reset current command to constructed state.
 *
 */
void CStringParser::reset()
{
    m_argument_counter = 0;
    m_command_name = "";
    m_string_argument = "";
    for (int i = 0; i < MAX_ARGUMENT_COUNT; ++i)
    {
        m_arguments[i] = 0;
    }
}

/**
 * @brief Get name of the last parsed command.
 *
 * @return Name of the last parsed command.
 */
const etl::string<MAX_STRING_SIZE> *CStringParser::getName() const
{
    return &m_command_name;
}

/**
 * @brief Get number of arguments.
 *
 * @return Number of parsed arguments.
 */
unsigned int CStringParser::getArgumentCount() const
{
    return m_argument_counter;
}

/**
 * @brief Access parsed arguments by index of their position in the command
 * string.
 *
 * @param index Position of the argument to be accessed. Zero-based index.
 * @return Value of the argument at the requested index.
 */
float CStringParser::operator[](unsigned int index)
{
    if (index < m_argument_counter)
    {
        return m_arguments[index];
    }
    return DEFAULT_ARGUMENT;
}

/**
 * @brief Retrieve string stored in m_string_argument
 * @note If no string was recognised during parsing, this returns an empty
 * string
 */
const etl::string<MAX_STRING_SIZE> *CStringParser::getStringArgument() const
{
    return &m_string_argument;
}
