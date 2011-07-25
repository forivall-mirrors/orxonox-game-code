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
 *      Fabian 'x3n' Landau
 *   Co-authors:
 *      ...
 *
 */

#ifndef _ArgumentCompleter_H__
#define _ArgumentCompleter_H__

#include "CorePrereqs.h"
#include "ArgumentCompletionListElement.h"

namespace orxonox
{
    class _CoreExport ArgumentCompleter
    {
        public:
            ArgumentCompleter(ArgumentCompletionList (*function) (void)) : paramCount_(0), function_0_(function) {}
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1)) : paramCount_(1), function_1_(function) {}
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2)) : paramCount_(2), function_2_(function) {}
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2, const std::string& param3)) : paramCount_(3), function_3_(function) {}
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4)) : paramCount_(4), function_4_(function) {}
            ArgumentCompleter(ArgumentCompletionList (*function) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4, const std::string& param5)) : paramCount_(5), function_5_(function) {}

            ArgumentCompletionList operator()(const std::string& param1 = "", const std::string& param2 = "", const std::string& param3 = "", const std::string& param4 = "", const std::string& param5 = "")
            {
                switch (this->paramCount_)
                {
                    case 0:
                        return (*this->function_0_)();
                    case 1:
                        return (*this->function_1_)(param1);
                    case 2:
                        return (*this->function_2_)(param1, param2);
                    case 3:
                        return (*this->function_3_)(param1, param2, param3);
                    case 4:
                        return (*this->function_4_)(param1, param2, param3, param4);
                    case 5:
                        return (*this->function_5_)(param1, param2, param3, param4, param5);
                    default:
                        return ArgumentCompletionList();
                }
            }

        private:
            unsigned char paramCount_;
            ArgumentCompletionList (*function_0_) (void);
            ArgumentCompletionList (*function_1_) (const std::string& param1);
            ArgumentCompletionList (*function_2_) (const std::string& param1, const std::string& param2);
            ArgumentCompletionList (*function_3_) (const std::string& param1, const std::string& param2, const std::string& param3);
            ArgumentCompletionList (*function_4_) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4);
            ArgumentCompletionList (*function_5_) (const std::string& param1, const std::string& param2, const std::string& param3, const std::string& param4, const std::string& param5);
    };
}

#endif /* _ArgumentCompleter_H__ */
