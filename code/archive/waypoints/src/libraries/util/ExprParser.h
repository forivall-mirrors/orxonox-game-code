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
@ingroup Util
@brief Declaration of FloatParser
*/

#ifndef _FloatParser_H__
#define _FloatParser_H__

#include "UtilPrereqs.h"

#include <map>
#include <string>

namespace orxonox
{
    /** Parser for expressions like \c "3 * cos(5 + 4) / a" where \a a is a predeclared
        variable.
    @par Usage
        Using it is rather simple:
        @code
        std::string str("3 + 4");
        ExprParser expr;
        expr.parse(str);
        if (expr.getSuccess())
        {
            if (!expr.getRemains().empty())
            {
                orxout(user_warning) << "Expression could not be parsed to the end! Remains: '" << expr.getRemains() << '\'' << endl;
            }
            float result = expr.getResult();
        }
        else
            orxout(user_error) << "Cannot calculate expression: Parse error." << endl;
        @endcode
        getRemains() returns the expression after what could be parsed. For instance
        \c "2*3 text" will return \c "text" as remains.
    @details
        The implementation of this class is really old and sort of a trial for
        a first C++ class by Reto. That explains why it looks more like C than
        C++... Also some of the variable names are in German. <br>
        Explaining how it works exactly is probably not possible anymore, but it
        is based on recursively parsing the expression character by character.
        That much I can remember.
    @par Functions, operators and variables supported
        - Variables:
            - e
            - pi
        - Functions:
            - sin, asin, sinh, asinh
            - cos, acos, cosh, acosh
            - tan, atan, tanh, atanh
            - int, floor, ceil, abs, sign
            - pow, sqrt, exp, ln, log
            - mod, div
            - min, max
            - radians, degrees
        - Operators:
            - +, -, ! (unary)
            - +, -, *, /, %, ^, |, &, !, <, >, !=, <=, >=, =
    @note
        Operators may not be very consistent with C++ rules, but using the class
        for plus and minus should be perfectly ok.
    */
    class _UtilExport ExprParser
    {
    public:
        enum binary_operator
        {
            b_plus,
            b_minus,
            mal,
            durch,
            modulo,
            hoch,
            undef,
            oder,
            und,
            gleich,
            b_nicht,
            kleiner,
            groesser,
            ungleich,
            kleinergleich,
            groessergleich
        };

        enum unary_operator
        {
            u_plus,
            u_minus,
            u_nicht
        };


        ExprParser();
        void parse(const std::string& str);
        const std::string& getRemains() { return  this->remains_; }
        float              getResult()  { return  this->result_; }
        bool               getSuccess() { return !this->failed_; }

        void setVariable(const std::string& varname, float value);

    private:
        float parse_expr_1();
        float parse_expr_2();
        float parse_expr_3();
        float parse_expr_4();
        float parse_expr_5();
        float parse_expr_6();
        float parse_expr_7();
        float parse_expr_8();
        char* parse_word(char* str);
        binary_operator parse_binary_operator();
        unary_operator parse_unary_operator();

        float parse_argument();
        float parse_last_argument();

        binary_operator op;
        const char* reading_stream;
        bool failed_;
        float result_;
        std::string remains_;
        std::map<std::string, float> variables_;
    };

    //Endzeichen für float expression: ')', '}', ']', ',', ';'
    _UtilExport bool parse_float(char* const, char**, float*);
    //Endzeichen angegeben
    _UtilExport bool parse_float(char* const, char**, char, float*);
    //Letzter Teil-float eines Vektors parsen (keine Vergleichs- und Logikoperationen)
    _UtilExport bool parse_vector_float(char* const, char**, bool, float*);
}

#endif /* _FloatParser_H__ */
