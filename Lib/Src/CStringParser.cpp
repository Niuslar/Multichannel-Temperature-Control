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
#include <ctype.h>

#define CMD_START   '>'
#define ARG_START   '('
#define ARG_STOP    ')'
#define ARG_DELIMIT ','

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
    reset();
    if (isValidFormat(string) == false)
    {
        return false;
    }
    // extract command name
    uint32_t cmd_start = string.find(CMD_START) + 1;
    uint32_t arg_start = string.find(ARG_START) + 1;
    m_command_name.assign(string.substr(cmd_start, arg_start - cmd_start));
    // now that command has been extracted, need to extract the arguments.
    uint32_t arg_stop = string.find(ARG_STOP);
    uint32_t arg_delimiter = string.find(ARG_DELIMIT, arg_start);
    etl::string<MAX_COMMAND_SIZE> argument_string;
    while (arg_delimiter < arg_stop)
    {
        argument_string = string.substr(arg_start, arg_delimiter - arg_start);
        if (sscanf(argument_string.c_str(),
                   "%f",
                   &(m_arguments[m_argument_counter])) == 1)
        {
            m_argument_counter++;
        }
        else if (m_string_argument.empty())
        {
            m_string_argument =
                string.substr(arg_start, arg_delimiter - arg_start);
        }
        else
        {
            return false;
        }
        // find next delimiter
        arg_start = arg_delimiter;
        arg_delimiter = string.find(ARG_DELIMIT, arg_start) + 1;
    }
    argument_string = string.substr(arg_start, arg_stop - arg_start);
    if (sscanf(argument_string.c_str(),
               "%f",
               m_arguments + m_argument_counter) == 1)
    {
        m_argument_counter++;
    }
    else if (m_string_argument.empty())
    {
        m_string_argument = string.substr(arg_start, arg_delimiter - arg_start);
    }
    else
    {
        return false;
    }
    return true;
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
    m_parse_state = IDLE;
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
 * @brief Access parsed arguments by index of their position in the
 * command string.
 *
 * @param index Position of the argument to be accessed. Zero-based
 * index.
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
 * @note If no string was recognised during parsing, this returns an
 * empty string
 */
const etl::string<MAX_STRING_SIZE> *CStringParser::getStringArgument() const
{
    return &m_string_argument;
}

/**
 * @brief Check if command conforms to the format requirements
 *
 * @param string containing the command to be checked.
 * @return True if command is valid, false otherwise.
 */
bool CStringParser::isValidFormat(
    const etl::string<MAX_STRING_SIZE> &string) const
{
    /* check command conforms to the format:
     * >command(arg1,arg2,arg3,...)
     * Arguments and brackets are optional. "command" can contain any valid
     * ascii characters.
     */
    uint32_t cmd_start = string.find(CMD_START);
    uint32_t arg_start = string.find(ARG_START);
    uint32_t arg_stop = string.find(ARG_STOP);

    // missing command start symbol.
    if (cmd_start == string.npos)
    {
        return false;
    }
    // closing bracket comes before opening bracket
    if (arg_stop < arg_start)
    {
        return false;
    }
    // no matching closing bracket for opening bracket
    if ((arg_start != string.npos) && (arg_stop == string.npos))
    {
        return false;
    }
    return true;
}
