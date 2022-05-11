/**
 * @file CJsonParser.cpp
 */
/*
 * Created on : Apr 4, 2022
 * Author : niuslar
 */

#include "CJsonParser.h"

#define ERROR_VALUE (0.00)

/**
 * @brief Constructor
 */
CJsonParser::CJsonParser() {}

/**
 * @brief Parse string
 * @param String to be parsed
 * @return Last parsing state
 */
void CJsonParser::parse(const etl::string<MAX_STRING_SIZE> &string)
{
    // Reset command variables
    m_argument_counter = 0;
    m_command_name = "";

    // Get tokens from string
    getTokens(string);

    for (uint16_t i = 0; i < m_token_counter; i++)
    {
        token_t current_token = m_tokens[i];
    }
}

/**
 * @brief Scan a string to get tokens*
 * @param String
 * @Note a lexer, tokeniser or scanner takes a string and breaks it up into
 * tokens. A token is the basic building block of the language. For example, in
 * English single words and punctuation would be tokens.
 */
void CJsonParser::getTokens(const etl::string<MAX_STRING_SIZE> &string)
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
                if (current_token.type == STRING ||
                    current_token.type == INTEGER ||
                    current_token.type == FLOAT)
                {
                    current_token.text.append(1, current_char);
                }
                else
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                    current_token.type = INTEGER;
                }
                break;
            case '.':
                if (current_token.type == INTEGER)
                {
                    current_token.text.append(1, current_char);
                    current_token.type = FLOAT;
                }
                else
                {
                    endToken(current_token);
                    current_token.text.append(1, current_char);
                    current_token.type = INVALID;
                }
                break;
            case ' ':
            case '\t':
                if (current_token.type == STRING)
                {
                    current_token.text.append(1, current_char);
                }
                else
                {
                    endToken(current_token);
                }
                break;
            case '\n':
            case '\0':
            case '\r':
                endToken(current_token);
                break;
            case ':':
                addSpecialChar(current_token, COLON, current_char);
                break;
            case '{':
                addSpecialChar(current_token, CURLY_OPEN, current_char);
                break;
            case '}':
                addSpecialChar(current_token, CURLY_CLOSE, current_char);
                break;
            case '[':
                addSpecialChar(current_token, ARRAY_OPEN, current_char);
                break;
            case ']':
                addSpecialChar(current_token, ARRAY_CLOSE, current_char);
                break;
            case ',':
                addSpecialChar(current_token, COMMA, current_char);
                break;

            case '"':
                if (current_token.type != STRING)
                {
                    endToken(current_token);
                    current_token.type = STRING;
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
                    current_token.text.append(1, current_char);
                    current_token.type = INVALID;
                }
                else
                {
                    current_token.text.append(1, current_char);
                }
                break;
        }
    }
    // Finish any token that might have not been stored
    endToken(current_token);
}

/**
 * @brief Add token to m_tokens vector
 * @param token to be added
 */
void CJsonParser::endToken(CJsonParser::token_t &token)
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
const etl::string<MAX_STRING_SIZE> *CJsonParser::getName() const
{
    return &m_command_name;
}

/**
 * @brief Get number of arguments.
 *
 * @return Number of parsed arguments.
 */
unsigned int CJsonParser::getArgumentCount() const
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
float CJsonParser::operator[](unsigned int index)
{
    if (index < MAX_ARGUMENT_COUNT)
    {
        return m_arguments[index];
    }
    return ERROR_VALUE;
}

/**
 * @brief Add special character token
 * @param token
 * @param token type
 * @param character to be appended
 */
void CJsonParser::addSpecialChar(CJsonParser::token_t &token,
                                 CJsonParser::token_type_t operator_type,
                                 char character)
{
    if (token.type != STRING)
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
