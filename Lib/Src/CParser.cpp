/**
 * @file CParser.cpp
 */
/*
 * Created on : Apr 4, 2022
 * Author : niuslar
 */

#include "CParser.h"

/**
 * @brief Parse string
 * @param String to be parsed
 * @return Parsing status
 */
CParser::parse_status_t CParser::parse(
    const etl::string<MAX_STRING_SIZE> &string)
{
    m_status = IDLE;

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
                    m_command.name = m_tokens[i].text;
                    m_status = END_KEY;
                }
                else if (m_status == BEGIN_VALUE)
                {
                    m_command.argument = m_tokens[i].text;
                    m_status = END_VALUE;
                }
                break;
            case OPERATOR:
                if (m_status == END_KEY)
                {
                    m_status = BEGIN_VALUE;
                }
                break;
            case INTEGER:
            case FLOAT:
                if (m_status == BEGIN_VALUE)
                {
                    m_command.argument = m_tokens[i].text;
                    m_status = END_VALUE;
                }
                break;
            case END_OPERATOR:
                if (m_status == END_VALUE)
                {
                    m_status = FINISHED;
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
 * @param String to be parsed
 * @return vector containing tokens
 */
void CParser::getTokens(const etl::string<MAX_STRING_SIZE> &string)
{
    // Reset vector
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
                else
                {
                    current_token.type = INVALID;
                    current_token.text.append(1, current_char);
                    endToken(current_token);
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
            case ':':
                if (current_token.type != STRING_LITERAL)
                {
                    endToken(current_token);
                    current_token.type = OPERATOR;
                    current_token.text.append(1, current_char);
                    endToken(current_token);
                }
                else
                {
                    current_token.text.append(1, current_char);
                }
                break;
            case '{':
                if (current_token.type != STRING_LITERAL)
                {
                    endToken(current_token);
                    current_token.type = BEGIN_OPERATOR;
                    current_token.text.append(1, current_char);
                    endToken(current_token);
                }
                else
                {
                    current_token.text.append(1, current_char);
                }
                break;
            case '}':
                if (current_token.type != STRING_LITERAL)
                {
                    endToken(current_token);
                    current_token.type = END_OPERATOR;
                    current_token.text.append(1, current_char);
                    endToken(current_token);
                }
                else
                {
                    current_token.text.append(1, current_char);
                }
                break;

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
                    current_token.type = IDENTIFIER;
                    current_token.text.append(1, current_char);
                }
                else
                {
                    current_token.text.append(1, current_char);
                }
                break;
        }
    }
}

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

CParser::command_t CParser::getCommand()
{
    return m_command;
}
