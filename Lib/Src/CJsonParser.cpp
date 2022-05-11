/**
 * @file CJsonParser.cpp
 * @brief This is a JSON parser that includes a tokeniser (view
 * stringToTokens()) and a parser. The tokeniser breaks up a string into JSON
 * components. The parser checks if the components follow JSON syntax and
 * returns true if it does.
 */
/*
 * Created on : Apr 4, 2022
 * Author : niuslar
 */

#include "CJsonParser.h"

#define DEFAULT_VALUE 0

/**
 * @brief Constructor
 */
CJsonParser::CJsonParser() {}

/**
 * @brief Parse string
 * @param String to be parsed
 * @return True if a JSON object was recognised
 */
bool CJsonParser::parse(const etl::string<MAX_STRING_SIZE> &string)
{
    // Reset command variables
    m_argument_counter = 0;
    m_command_name = "";

    stringToTokens(string);
    return parseObject();
}

/**
 * @brief Tokeniser* that scans string and breaks it up into tokens
 *
 * @param String
 *
 * @note A tokeniser or lexer takes a string and breaks it up into
 * tokens. A token is the basic building block of the language. For example, in
 * English, single words and punctuation marks would be tokens.
 */
void CJsonParser::stringToTokens(const etl::string<MAX_STRING_SIZE> &string)
{
    // Reset variables
    m_tokens.clear();
    m_token_counter = 0;
    token_t current_token;

    // Loop through characters in string with a range-base for-loop
    // Note: To learn about range-based loops:
    // https://www.techiedelight.com/iterate-over-characters-string-cpp/
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
                    finishToken(current_token);
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
                    finishToken(current_token);
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
                    finishToken(current_token);
                }
                break;
            case '\n':
            case '\0':
            case '\r':
                finishToken(current_token);
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
                    finishToken(current_token);
                    current_token.type = STRING;
                }
                else
                {
                    finishToken(current_token);
                }
                break;

            default:
                if (current_token.type == WHITESPACE ||
                    current_token.type == INTEGER ||
                    current_token.type == FLOAT)
                {
                    finishToken(current_token);
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
    finishToken(current_token);
}

/**
 * @brief Add token to m_tokens vector
 * @param token to be added
 */
void CJsonParser::finishToken(CJsonParser::token_t &token)
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
    if (index < m_argument_counter)
    {
        return m_arguments[index];
    }
    return DEFAULT_VALUE;
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
        finishToken(token);
        token.type = operator_type;
        token.text.append(1, character);
        finishToken(token);
    }
    else
    {
        token.text.append(1, character);
    }
}

/**
 * @note The following methods parse non-terminal symbols. To understand better
 * the logic behind these methods please Google Recursive Descent Parser or
 * visit: http://www.cs.binghamton.edu/~zdu/parsdemo/recintro.html
 *
 * The rules for this parser:
        |  Object   | { Pair }     		 |
        |  Pair     | key : value  		 |
        |  Key      | string       		 |
        |  Value    | Number       		 |
        |  Value    | Array        		 |
        |  Array    | [ Elements ]       |
        |  Elements | Number, Number, .. |
 */

bool CJsonParser::parseObject()
{
    m_token_index = 0;
    bool b_success = false;
    if (m_tokens[m_token_index].type == CURLY_OPEN)
    {
        m_token_index++;
        if (m_token_index < m_token_counter)
        {
            if (parsePair())
            {
                if ((m_tokens[++m_token_index].type == CURLY_CLOSE) &&
                    (m_token_index == (m_token_counter - 1)))
                {
                    b_success = true;
                }
            }
        }
    }
    return b_success;
}

bool CJsonParser::parsePair()
{
    bool b_success = false;
    if (m_tokens[m_token_index].type == STRING)
    {
        m_command_name = m_tokens[m_token_index].text;
        m_token_index++;
        if (m_tokens[m_token_index].type == COLON)
        {
            m_token_index++;
            b_success = parseValue();
        }
    }
    return b_success;
}

bool CJsonParser::parseValue()
{
    bool b_success = false;
    if (m_tokens[m_token_index].type == ARRAY_OPEN)
    {
        m_token_index++;
        b_success = parseArray();
    }
    else if (m_tokens[m_token_index].type == INTEGER ||
             m_tokens[m_token_index].type == FLOAT)
    {
        float number = std::stof(m_tokens[m_token_index].text.c_str());
        if (m_argument_counter < MAX_ARGUMENT_COUNT)
        {
            m_arguments[m_argument_counter++] = number;
            b_success = true;
        }
    }
    return b_success;
}
bool CJsonParser::parseArray()
{
    bool b_success = false;

    if (m_tokens[m_token_index].type == INTEGER ||
        m_tokens[m_token_index].type == FLOAT)
    {
        float number = std::stof(m_tokens[m_token_index].text.c_str());
        if (m_argument_counter < MAX_ARGUMENT_COUNT)
        {
            m_arguments[m_argument_counter++] = number;
            m_token_index++;
            if (m_tokens[m_token_index].type == COMMA)
            {
                m_token_index++;
                if (m_token_index < m_token_counter)
                {
                    b_success = parseArray();
                }
            }
            else if (m_tokens[m_token_index].type == ARRAY_CLOSE)
            {
                b_success = true;
            }
        }
    }
    return b_success;
}
