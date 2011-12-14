/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
 *                    > www.orxonox.net <
 *
 *
 *   License notice:
 *
 *   This program is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU General Public License
 *   as published by the Free Software Foundation; either version 2
 *   of the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *   Author:
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
  @file
  @brief Declaration of FloatParser
*/

#include "ExprParser.h"

#include <cmath>
#include <cstring>
#include <cstdlib>
#include "Math.h"

// macros for easier if, else statements
#define CASE_1(var) if (!strcmp(SWITCH,var))
#define CASE(var) else if (!strcmp(SWITCH,var))
#define CASE_ELSE else

//! skip white spaces
#define PARSE_BLANKS while (*reading_stream == ' ') ++reading_stream

namespace orxonox
{
    ExprParser::ExprParser()
    {
        this->failed_ = false;
        this->variables_["pi"] = math::pi;
        this->variables_["e"] = math::e;
    }

    void ExprParser::setVariable(const std::string& varname, float value)
    {
        this->variables_[varname] = value;
    }

    void ExprParser::parse(const std::string& str)
    {
        this->reading_stream = str.c_str();
        if (str.size() == 0 || *reading_stream == '\0')
        {
            this->failed_ = true;
            this->result_ = 0.0;
        }
        else
        {
            this->result_ = parse_expr_8();
            this->remains_ = reading_stream;
        }
    }

    //Private functions:
    /******************/
    float ExprParser::parse_argument()
    {
        float value = parse_expr_8();
        if (*reading_stream == ',')
        {
            ++reading_stream;
            return value;
        }
        else
        {
            this->failed_ = true;
            return 0;
        }
    }

    float ExprParser::parse_last_argument()
    {
        float value = parse_expr_8();
        if (*reading_stream == ')')
        {
            ++reading_stream;
            return value;
        }
        else
        {
            this->failed_ = true;
            return 0;
        }
    }

    float ExprParser::parse_expr_8()
    {
        float value = parse_expr_7();
        for(;;)
        {
            switch (op)
            {
            case oder:
                value = parse_expr_7() || value;
                break;
            default: return value;
            }
        };
    }


    float ExprParser::parse_expr_7()
    {
        float value = parse_expr_6();
        for(;;)
        {
            switch (op)
            {
            case und:
                value = value && parse_expr_6();
                break;
            default: return value;
            }
        };
    }

    float ExprParser::parse_expr_6()
    {
        float value = parse_expr_5();
        for(;;)
        {
            switch (op)
            {
            case gleich:
                value = (value == parse_expr_5());
                break;
            case ungleich:
                value = (value != parse_expr_5());
                break;
            default:
                return value;
            }
        };
    }

    float ExprParser::parse_expr_5()
    {
        float value = parse_expr_4();
        for(;;)
        {
            switch (op)
            {
            case kleiner:
                value = (value < parse_expr_4());
                break;
            case kleinergleich:
                value = (value <= parse_expr_4());
                break;
            case groesser:
                value = (value > parse_expr_4());
                break;
            case groessergleich:
                value = (value >= parse_expr_4());
                break;
            default:
                return value;
            }
        };
    }

    float ExprParser::parse_expr_4()
    {
        float value = parse_expr_3();
        for(;;)
        {
            switch (op)
            {
            case b_plus:
                value += parse_expr_3();
                break;
            case b_minus:
                value -= parse_expr_3();
                break;
            default:
                return value;
            }
        };
    }

    float ExprParser::parse_expr_3()
    {
        float value = parse_expr_2();
        for(;;)
        {
            switch (op)
            {
            case mal:
                value *= parse_expr_2();
                break;
            case durch:
                value /= parse_expr_2();
                break;
            case modulo:
                {
                    float temp = parse_expr_2();
                    value = value - floor(value/temp)*temp;
                    break;
                }
            default:
                return value;
            }
        };
    }

    float ExprParser::parse_expr_2()
    {
        float value = parse_expr_1();
        while (*reading_stream != '\0')
        {
            op = parse_binary_operator();
            switch (op)
            {
            case hoch:
                value = pow(value,parse_expr_1());
                break;
            default:
                return value;
            }
        };
        op = undef;
        return value;
    }

    float ExprParser::parse_expr_1()
    {
        PARSE_BLANKS;
        float value;

        unary_operator op = parse_unary_operator();
        PARSE_BLANKS;

        if (*reading_stream == '\0')
        {
            // end of string
            this->failed_ = true;
            return 0;
        }
        else if ((*reading_stream > 47 && *reading_stream < 59) || *reading_stream == 46)
        {  // number
            value = (float)strtod(reading_stream, const_cast<char**>(&reading_stream));
        }
        else if ((*reading_stream > 64 && *reading_stream < 91) || (*reading_stream > 96 && *reading_stream < 123) || *reading_stream == 46)
        {  // variable or function
            char* word = new char[256];
            parse_word(word);
            PARSE_BLANKS;
            if (*reading_stream == '(')
            {
                ++reading_stream;
#define SWITCH word
                CASE_1("sin")
                    value = sin(parse_last_argument());
                CASE("asin")
                    value = asin(parse_last_argument());
                CASE("sinh")
                    value = sinh(parse_last_argument());
                CASE("asinh")
                {
                    value = parse_last_argument();
                    value = log(sqrt(pow(value, 2) + 1) + value);
                }
                CASE("cos")
                    value = cos(parse_last_argument());
                CASE("acos")
                    value = acos(parse_last_argument());
                CASE("cosh")
                    value = cosh(parse_last_argument());
                CASE("acosh")
                {
                    value = parse_last_argument();
                    value = log(sqrt(pow(value, 2) - 1) + value);
                }
                CASE("tan")
                    value = tan(parse_last_argument());
                CASE("atan")
                    value = atan(parse_last_argument());
                CASE("atan2")
                    value = atan2(parse_argument(),parse_last_argument());
                CASE("tanh")
                    value = tanh(parse_last_argument());
                CASE("atanh")
                {
                    value = parse_last_argument();
                    value = 0.5f*log((value + 1.0f)/(value - 1.0f));
                }
                CASE("int")
                    value = floor(parse_last_argument());
                CASE("floor")
                    value = floor(parse_last_argument());
                CASE("ceil")
                    value = ceil(parse_last_argument());
                CASE("abs")
                    value = fabs(parse_last_argument());
                CASE("exp")
                    value = exp(parse_last_argument());
                CASE("log")
                    value = log10(parse_last_argument());
                CASE("ln")
                    value = log(parse_last_argument());
                CASE("sign")
                {
                    value = parse_last_argument();
                    value = (value>0.0f ? 1.0f : (value<0.0f ? -1.0f : 0.0f));
                }
                CASE("sqrt")
                    value = sqrt(parse_last_argument());
                CASE("degrees")
                    value = parse_last_argument()*180.0f/math::pi;
                CASE("radians")
                    value = parse_last_argument()*math::pi/180.0f;
                CASE("mod")
                {
                    value = parse_argument();
                    float value2 = parse_last_argument();
                    value = value - floor(value/value2)*value2;
                }
                CASE("pow")
                    value = pow(parse_argument(),parse_last_argument());
                CASE("div")
                    value = floor(parse_argument()/parse_last_argument());
                CASE("max")
                    value = std::max(parse_argument(),parse_last_argument());
                CASE("min")
                    value = std::min(parse_argument(),parse_last_argument());
                CASE_ELSE
                {
                    this->failed_ = true;
                    delete[] word;
                    return 0;
                }
            }
            else
            {
                std::map<std::string, float>::const_iterator it = this->variables_.find(word);
                if (it != this->variables_.end())
                    value = it->second;
                else
                {
                    this->failed_ = true;
                    delete[] word;
                    return 0;
                }
            }
            delete[] word;
        }
        else if (*reading_stream == 40)
        {  // expression in parenthesis
            ++reading_stream;
            value = parse_last_argument();
        }
        else
        {
            this->failed_ = true;
            return 0;
        }

        PARSE_BLANKS;
        switch (op)
        {
        case u_nicht: return !value;
        case u_plus:  return  value;
        case u_minus: return -value;
        default:
            this->failed_ = true;
            return 0;
        }
    }

    char* ExprParser::parse_word(char* str)
    {
        char* word = str;
        int counter = 0;
        while ((*reading_stream > 47 && *reading_stream < 58) || (*reading_stream > 64 && *reading_stream < 91) || (*reading_stream > 96 && *reading_stream < 123) || *reading_stream == 46)
        {
            *word++ = *reading_stream++;
            counter++;
            if (counter > 255)
            {
                this->failed_ = true;
                return '\0';
            }
        };
        *word = '\0';
        return str;
    }

    ExprParser::binary_operator ExprParser::parse_binary_operator()
    {
        binary_operator op;
        switch (*reading_stream)
        {
        case '+': op = b_plus; break;
        case '-': op = b_minus; break;
        case '*': op = mal; break;
        case '/': op = durch; break;
        case '^': op = hoch; break;
        case '%': op = modulo; break;
        case '&': op = und; break;
        case '|': op = oder; break;
        case '=': op = gleich; break;
        case '!': op = b_nicht; break;
        case '<': op = kleiner; break;
        case '>': op = groesser; break;
        default: return undef;
        }
        if (*++reading_stream == '=')
        {
            if (op > 9)
            {
                ++reading_stream;
                return (binary_operator)(op + 3);
            }
            else
            {
                --reading_stream;
                return undef;
            }
        }
        else
            return op;
    }

    ExprParser::unary_operator ExprParser::parse_unary_operator()
    {
        switch (*reading_stream)
        {
        case '!':
            ++reading_stream;
            return u_nicht;
        case '+':
            ++reading_stream;
            return u_plus;
        case '-':
            ++reading_stream;
            return u_minus;
        default :
            return u_plus;
        }
    }
}
