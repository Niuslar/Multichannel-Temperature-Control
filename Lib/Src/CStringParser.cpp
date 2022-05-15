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
    bool b_command_valid = false;
    reset();
    for (const char *p_character = string.begin();
         p_character < string.end() && m_parse_state != FINISH;
         ++p_character)
    {
        switch (m_parse_state)
        {
            case IDLE:
                b_command_valid = processIdle(p_character);
                break;
            case NAME:
                b_command_valid = processName(p_character);
                break;
            case ARGUMENTS:
                b_command_valid = processArguments(p_character);
                break;
            case FINISH:
                break;
            default:
                // TODO: call error handler
                break;
        }
    }
    return b_command_valid;
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

bool CStringParser::processIdle(const char *p_character)
{
    if (*p_character == '{')
    {
        m_parse_state = FINISH;
        return false;
    }
    if (isalnum(*p_character) || (*p_character == '?'))
    {
        m_command_name.append(p_character);
        m_parse_state = NAME;
        return true;
    }
}

bool CStringParser::processName(const char *p_character)
{
    // another character for the name. Command complete.
    if (isalnum(*p_character))
    {
        m_command_name.append(p_character);
        return true;
    }
    // EOL, LF, CR. Command complete.
    if ((*p_character == 0) || (*p_character == 10) || (*p_character == 13))
    {
        m_parse_state = FINISH;
        return true;
    }
    // start of arguments. Command not complete.
    if (*p_character == '(')
    {
        m_parse_state = ARGUMENTS;
        return false;
    }
}

bool CStringParser::processArguments(const char *p_character)
{
    if (*p_character == ')')
    {
        m_parse_state = FINISH;
        return true;
    }
    float argument;
    if (sscanf(p_character, "%f", &argument) == 1)
    {
        m_arguments[m_argument_counter] = argument;
        m_argument_counter++;
        return false;
    }
    else if (sscanf(p_character, "%s", m_string_argument.c_str()))
    {
    }
}

bool CStringParser::isValid(const char character) const
{
    bool b_valid = false;
    if (isalnum(character))
    {
        b_valid = true;
    }
    if ((character == '?') || (character == '_'))
    {
        b_valid = true;
    }
    return b_valid;
}
