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
    m_string_argument = "";

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
    resetCurrentToken();

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
                processDigit(current_char);
                break;
            case '.':
                processDecimalPoint(current_char);
                break;
            case ' ':
            case '\t':
                processSpacer(current_char);
                break;
            case '\n':
            case '\0':
            case '\r':
                finishCurrentToken();
                break;
            case ':':
                addSpecialChar(COLON, current_char);
                break;
            case '{':
                addSpecialChar(CURLY_OPEN, current_char);
                break;
            case '}':
                addSpecialChar(CURLY_CLOSE, current_char);
                break;
            case '[':
                addSpecialChar(ARRAY_OPEN, current_char);
                break;
            case ']':
                addSpecialChar(ARRAY_CLOSE, current_char);
                break;
            case ',':
                addSpecialChar(COMMA, current_char);
                break;

            case '"':
                processDoubleQuote(current_char);
                break;

            /* The default case takes care of all the letters and special
             * characters by either adding them to a string when necessary or
             * creating an INVALID token.
             */
            default:
                processDefault(current_char);
        }
    }
    // Finish any token that might have not been stored
    finishCurrentToken();
}

/**
 * @brief process digit character
 */
void CJsonParser::processDigit(char current_char)
{
    if (m_current_token.type == STRING || m_current_token.type == INTEGER ||
        m_current_token.type == FLOAT)
    {
        m_current_token.text.append(1, current_char);
    }
    else
    {
        finishCurrentToken();
        m_current_token.text.append(1, current_char);
        m_current_token.type = INTEGER;
    }
}

/**
 * @brief process decimal point character
 */
void CJsonParser::processDecimalPoint(char current_char)
{
    if (m_current_token.type == INTEGER)
    {
        m_current_token.text.append(1, current_char);
        m_current_token.type = FLOAT;
    }
    else
    {
        finishCurrentToken();
        m_current_token.text.append(1, current_char);
        m_current_token.type = INVALID;
    }
}

/**
 * @brief process space characters (space or tab)
 */
void CJsonParser::processSpacer(char current_char)
{
    if (m_current_token.type == STRING)
    {
        m_current_token.text.append(1, current_char);
    }
    else
    {
        finishCurrentToken();
    }
}

/**
 * @brief process double quote character
 */
void CJsonParser::processDoubleQuote(char current_char)
{
    if (m_current_token.type != STRING)
    {
        finishCurrentToken();
        m_current_token.type = STRING;
    }
    else
    {
        finishCurrentToken();
    }
}

/**
 * @brief Process any character without a switch case
 */
void CJsonParser::processDefault(char current_char)
{
    if (m_current_token.type == WHITESPACE || m_current_token.type == INTEGER ||
        m_current_token.type == FLOAT)
    {
        finishCurrentToken();
        m_current_token.text.append(1, current_char);
        m_current_token.type = INVALID;
    }
    else
    {
        m_current_token.text.append(1, current_char);
    }
}

/**
 * @brief Check if m_current_token is valid and store in m_tokens[] if it is.
 *
 */
void CJsonParser::finishCurrentToken()
{
    if (m_current_token.type != WHITESPACE)
    {
        if (m_current_token.text.empty() == false)
        {
            m_tokens.push_back(m_current_token);
            m_token_counter++;
        }
    }

    resetCurrentToken();
}

/**
 * @bief Reset m_current_token type and text
 */
void CJsonParser::resetCurrentToken()
{
    m_current_token.type = WHITESPACE;
    m_current_token.text.clear();
}

/**
 * @brief Add special character token
 * @param token type
 * @param character to be appended
 */
void CJsonParser::addSpecialChar(CJsonParser::token_type_t token_type,
                                 char character)
{
    if (m_current_token.type != STRING)
    {
        finishCurrentToken();
        m_current_token.type = token_type;
        m_current_token.text.append(1, character);
        finishCurrentToken();
    }
    else
    {
        m_current_token.text.append(1, character);
    }
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
 * @brief Retrieve string stored in m_string_argument
 * @note If no string was recognised during parsing, this returns an empty
 * string
 */
const etl::string<MAX_STRING_SIZE> *CJsonParser::getStringArgument() const
{
    return &m_string_argument;
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

/**
 * @brief Check if a JSON object is recognised using the tokens stored in
 * m_tokens after using stringToTokens()
 *
 * @return True if successful
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

/**
 * @brief Check for a valid key:value pair, if a valid key is found, this will
 * be stored in m_command_variable and then parseValue() will be called.
 *
 * @return True if successful
 */
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

/**
 * @brief Check for a valid Value. This could be an array of values or a single
 * value.
 *
 * @return True if successful
 */
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
    else if (m_tokens[m_token_index].type == STRING)
    {
        m_string_argument = m_tokens[m_token_index].text;
        b_success = true;
    }
    return b_success;
}

/**
 * @brief Check if the array follows the right syntax and store values in
 * m_arguments[].
 *
 * @return True if successful
 */
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
