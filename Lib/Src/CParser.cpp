/**
 * @file CParser.cpp
 */
/*
 * Created on : Apr 4, 2022
 * Author : niuslar
 */

#include "CParser.h"

#define ERROR_VALUE (0.00)

/**
 * @brief Constructor
 */
CParser::CParser() {}

/**
 * @brief Parse string
 * @param String to be parsed
 * @return Last parsing state
 */
CParser::parser_state_t CParser::parse(
    const etl::string<MAX_STRING_SIZE> &string)
{
    // Reset command variables
    m_status = IDLE;
    m_argument_counter = 0;
    m_command_name = "";

    // Get tokens from string
    getTokens(string);

    for (uint16_t i = 0; i < m_token_counter; i++)
    {
        token_t current_token = m_tokens[i];

        switch (current_token.type)
        {
        }
    }

    return m_status;
}

/**
 * @brief Process string to get tokens
 * @param String to be tokenised
 */
void CParser::getTokens(const etl::string<MAX_STRING_SIZE> &string)
{
    // Reset variables
    m_tokens.clear();
    m_token_counter = 0;
    token_t current_token;
    bool b_exit_loop_flag = false;

    // Loop through characters in string
    for (char current_char : string)
    {
        switch (current_token.type)
        {
            case WHITESPACE:
                if (isdigit(current_char))
                {
                    current_token.text.append(1, current_char);
                    current_token.type = INTEGER;
                }
                else if (isSpecialChar(current_char))
                {
                    current_token.text.append(1, current_char);
                    current_token.type = SPECIAL_CHAR;
                }
                else if (current_char == '"')
                {
                    current_token.type = STRING_LITERAL;
                }
                else
                {
                    // We append a character because we need to store the
                    // invalid token for the parser
                    current_token.text.append(1, current_char);
                    current_token.type = INVALID;
                }

            case INTEGER:
                if (isdigit(current_char))
                {
                    current_token.text.append(1, current_char);
                }
                else if (isSpecialChar(current_char))
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                    current_token.type = SPECIAL_CHAR;
                }
                else if (current_char == '.')
                {
                    current_token.text.append(1, current_char);
                }
                else if (current_char == '"')
                {
                    endToken(current_token);
                    current_token.type = STRING_LITERAL;
                }
                else if (current_char == '\n' || current_char == '\0' ||
                         current_char == ' ')
                {
                    endToken(current_token);
                }

            case FLOAT:
                if (isdigit(current_char))
                {
                    current_token.text.append(1, current_char);
                }
                else if (isSpecialChar(current_char))
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                    current_token.type = SPECIAL_CHAR;
                }
                else if (current_char == '"')
                {
                    endToken(current_token);
                    current_token.type = STRING_LITERAL;
                }
                else if (current_char == '\n' || current_char == '\0' ||
                         current_char == ' ')
                {
                    endToken(current_token);
                }
                else
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                    current_token.type = INVALID;
                }

            case STRING:
                if (current_char == '\n' || current_char == '\0' ||
                    current_char == '"' || current_char == ' ')
                {
                    endToken(current_token);
                }
                else
                {
                    current_token.text.append(1, current_char);
                }
            case SPECIAL_CHAR:
                if (isdigit(current_char))
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                    current_token.type = INTEGER;
                }
                if (isSpecialChar(current_char))
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                }
                else if (current_char == '"')
                {
                    endToken(current_token);
                    current_token.type = STRING_LITERAL;
                }
                else if (current_char == '\n' || current_char == '\0' ||
                         current_char == ' ')
                {
                    endToken(current_token);
                }
                else
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                    current_token.type = INVALID;
                }
            case INVALID:
                // Set flag to exit loop, there was an error
                endToken(current_token);
                b_exit_loop_flag = true;
            default:
                endToken(current_token);
                current_token.text.append(1, current_char);
                current_token.type = INVALID;
        }
        if (b_exit_loop_flag == true)
        {
            break;
        }
    }
}

/**
 * @brief Add token to m_tokens vector
 * @param token to be added
 */
void CParser::endToken(CParser::token_t &token)
{
    if (token.type != WHITESPACE)
    {
        if (token.text.empty() == false)
        {
            m_tokens.push_back(token);
            m_token_counter++;
        }
    }
    token.type = WHITESPACE;
    token.text.clear();
}

/**
 * @brief Get name of the last parsed command.
 *
 * @return Name of the last parsed command.
 */
etl::string<MAX_STRING_SIZE> CParser::getName() const
{
    return m_command_name;
}

/**
 * @brief Get number of arguments.
 *
 * @return Number of parsed arguments.
 */
unsigned int CParser::getArgumentCount() const
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
float CParser::operator[](unsigned int index)
{
    if (index < MAX_ARGUMENT_COUNT)
    {
        return m_arguments[index];
    }
    return ERROR_VALUE;
}

/**
 * @brief Checks if a character is a special character used in JSON
 *
 * @param character
 *
 * @return True if it is a special character
 */
bool CParser::isSpecialChar(char character)
{
    bool b_is_special = false;
    for (char i : m_special_characters)
    {
        if (character == i)
        {
            b_is_special = true;
        }
    }
    return b_is_special;
}
