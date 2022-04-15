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
 */
void CParser::parse(const etl::string<MAX_STRING_SIZE> &string)
{
    // Get tokens from string
    getTokens(string);

    // TODO: Process tokens
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
            case '(':
            case ')':
            case '{':
            case '}':
            case '-':
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
        }
    }
    token.type = WHITESPACE;
    token.text.clear();
}
