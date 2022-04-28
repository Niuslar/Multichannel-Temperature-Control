/**
 * @file CParser.cpp
 */
/*
 * Created on : Apr 4, 2022
 * Author : niuslar
 */

#include "CParser.h"

#define ERROR_VALUE (999.99)

/**
 * @brief Constructor
 */
CParser::CParser() {}

/**
 * @brief Parse string
 * @param String to be parsed
 * @return Parsing status
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
            case BEGIN_OPERATOR:
                if (m_status == IDLE)
                {
                    m_status = BEGIN_KEY;
                }
                break;
            case STRING_LITERAL:
                if (m_status == BEGIN_KEY)
                {
                    // Store key
                    m_command_name = m_tokens[i].text;
                    m_status = END_KEY;
                }
                break;
            case START_ARRAY_OP:
                if (m_status == BEGIN_VALUE)
                {
                    m_status = BEGIN_ARRAY;
                }
                break;
            case ARGUMENT_DELIMITER:
                if (m_status == BEGIN_ARRAY || m_status == VALUE_ADDED)
                {
                    m_status = ADD_VALUE;
                }
                else
                {
                    m_status = ERROR;
                }
                break;
            case KEY_VALUE_DIV:
                if (m_status == END_KEY)
                {
                    m_status = BEGIN_VALUE;
                }
                break;
            case INTEGER:
            case FLOAT:
                if (m_status == BEGIN_VALUE)
                {
                    // We need to convert string to float
                    float number = std::stof(m_tokens[i].text.c_str());
                    m_arguments[m_argument_counter] = number;
                    m_argument_counter++;
                    m_status = END_VALUES;
                }
                else if (m_status == BEGIN_ARRAY || m_status == ADD_VALUE)
                {
                    if (m_argument_counter < MAX_ARGUMENT_COUNT)
                    {
                        float number = std::stof(m_tokens[i].text.c_str());
                        m_arguments[m_argument_counter] = number;
                        m_argument_counter++;
                        m_status = VALUE_ADDED;
                    }
                }
                break;
            case END_ARRAY_OP:
                if (m_status == VALUE_ADDED)
                {
                    m_status = END_VALUES;
                }
            case END_OPERATOR:
                if (m_status == END_VALUES)
                {
                    m_status = PARSING_OK;
                }
                break;
            default:
                m_status = ERROR;
        }
    }

    return m_status;
}

/**
 * @brief Retrieve tokens from a string
 * @param String to be tokenised
 */
void CParser::getTokens(const etl::string<MAX_STRING_SIZE> &string)
{
    // Reset variables
    m_tokens.clear();
    m_token_counter = 0;
    token_t current_token;

    // Loop through characters in string
    for (char current_char : string)
    {
        switch (current_char)
        {
            // Numbers
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                if (current_token.type == WHITESPACE)
                {
                    current_token.type = INTEGER;
                    current_token.text.append(1, current_char);
                }
                else
                {
                    current_token.text.append(1, current_char);
                }
                break;

            // Check if it is a float
            case '.':
                if (current_token.type == INTEGER)
                {
                    current_token.type = FLOAT;
                    current_token.text.append(1, current_char);
                }
                break;

            // Token end characters
            case ' ':
            case '\t':
                if (current_token.type == STRING_LITERAL)
                {
                    current_token.text.append(1, current_char);
                }
                else
                {
                    endToken(current_token);
                }
                break;
            case '\n':
            case '\r':
            case '\0':
                endToken(current_token);
                break;

            // Operators
            case ':':
                addOperator(current_token, KEY_VALUE_DIV, current_char);
                break;
            case '{':
                addOperator(current_token, BEGIN_OPERATOR, current_char);
                break;
            case '}':
                addOperator(current_token, END_OPERATOR, current_char);
                break;
            case '[':
                addOperator(current_token, START_ARRAY_OP, current_char);
                break;
            case ']':
                addOperator(current_token, END_ARRAY_OP, current_char);
                break;
            case ',':
                addOperator(current_token, ARGUMENT_DELIMITER, current_char);
                break;

            // String literal
            case '"':
                if (current_token.type != STRING_LITERAL)
                {
                    endToken(current_token);
                    current_token.type = STRING_LITERAL;
                }
                else
                {
                    endToken(current_token);
                }
                break;

            default:
                if (current_token.type == WHITESPACE ||
                    current_token.type == INTEGER ||
                    current_token.type == FLOAT)
                {
                    endToken(current_token);
                }
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
 * @brief Add operator token
 * @param token
 * @param operator type
 */
void CParser::addOperator(CParser::token_t &token,
                          CParser::token_type_t operator_type,
                          char character)
{
    if (token.type != STRING_LITERAL)
    {
        endToken(token);
        token.type = operator_type;
        token.text.append(1, character);
        endToken(token);
    }
    else
    {
        token.text.append(1, character);
    }
}

/**
 * @brief Get command name
 * @return etl::string with command name
 */
etl::string<MAX_STRING_SIZE> CParser::getName() const
{
    return m_command_name;
}

/**
 * @brief Get arguments for a given command
 */
unsigned int CParser::getArgumentCount() const
{
    return m_argument_counter;
}

/**
 * @brief get argument from index
 */
float CParser::operator[](unsigned int index)
{
    if (index < MAX_ARGUMENT_COUNT)
    {
        return m_arguments[index];
    }
    return ERROR_VALUE;
}
