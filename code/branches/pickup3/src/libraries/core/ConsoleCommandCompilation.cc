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

#include "ConsoleCommandCompilation.h"

#include <fstream>
#include <set>
#include <string>

#include "util/Debug.h"
#include "util/ExprParser.h"
#include "util/StringUtils.h"
#include "ConsoleCommand.h"

namespace orxonox
{
    SetConsoleCommandShortcutExtern(source).argumentCompleter(0, autocompletion::files());
    SetConsoleCommandShortcutExtern(echo);
    SetConsoleCommandShortcutExtern(puts);

    SetConsoleCommandShortcutExtern(read).argumentCompleter(0, autocompletion::files());
    SetConsoleCommandShortcutExtern(append).argumentCompleter(0, autocompletion::files());
    SetConsoleCommandShortcutExtern(write).argumentCompleter(0, autocompletion::files());

    SetConsoleCommandShortcutExtern(calculate);

    void source(const std::string& filename)
    {
        static std::set<std::string> executingFiles;

        std::set<std::string>::const_iterator it = executingFiles.find(filename);
        if (it != executingFiles.end())
        {
            COUT(1) << "Error: Recurring source command in \"" << filename << "\". Stopped execution." << std::endl;
            return;
        }

        // Open the file
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't open file \"" << filename << "\"." << std::endl;
            return;
        }

        executingFiles.insert(filename);

        // Iterate through the file and put the lines into the CommandExecutor
        while (file.good() && !file.eof())
        {
            std::string line;
            std::getline(file, line);
            CommandExecutor::execute(line);
        }

        executingFiles.erase(filename);
        file.close();
    }

    std::string echo(const std::string& text)
    {
        return text;
    }

    void puts(bool newline, const std::string& text)
    {
        if (newline)
        {
            COUT(0) << stripEnclosingBraces(text) << std::endl;
        }
        else
        {
            COUT(0) << stripEnclosingBraces(text);
        }
    }

    void write(const std::string& filename, const std::string& text)
    {
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::out);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't write to file \"" << filename << "\"." << std::endl;
            return;
        }

        file << text << std::endl;
        file.close();
    }

    void append(const std::string& filename, const std::string& text)
    {
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::app);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't append to file \"" << filename << "\"." << std::endl;
            return;
        }

        file << text << std::endl;
        file.close();
    }

    std::string read(const std::string& filename)
    {
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            COUT(1) << "Error: Couldn't read from file \"" << filename << "\"." << std::endl;
            return "";
        }

        std::string output;
        while (file.good() && !file.eof())
        {
            std::string line;
            std::getline(file, line);
            output += line;
            output += "\n";
        }

        file.close();

        return output;
    }

    float calculate(const std::string& calculation)
    {
        ExprParser expr;
        expr.parse(calculation);
        if (expr.getSuccess())
        {
            if (expr.getResult() == 42.0)
            {
                COUT(3) << "Greetings from the restaurant at the end of the universe." << std::endl;
            }
            if (!expr.getRemains().empty())
            {
                COUT(2) << "Warning: Expression could not be parsed to the end! Remains: '" << expr.getRemains() << '\'' << std::endl;
            }
            return static_cast<float>(expr.getResult());
        }
        else
        {
            COUT(1) << "Error: Cannot calculate expression: Parse error." << std::endl;
            return 0;
        }
    }
}
