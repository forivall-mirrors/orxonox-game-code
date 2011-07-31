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

/**
    @file
    @brief Implementation of some console commands.
*/

#include "ConsoleCommandCompilation.h"

#include <fstream>
#include <set>
#include <string>

#include "util/Output.h"
#include "util/ExprParser.h"
#include "util/StringUtils.h"
#include "ConsoleCommand.h"
#include "CommandExecutor.h"

namespace orxonox
{
//    SetConsoleCommand("source", source).argumentCompleter(0, autocompletion::files());  // disabled because we use the implementation in Tcl
    SetConsoleCommand("echo", echo);

//    SetConsoleCommand("read", read).argumentCompleter(0, autocompletion::files());      // disabled because we use the implementation in Tcl
//    SetConsoleCommand("append", append).argumentCompleter(0, autocompletion::files());  // disabled because we use the implementation in Tcl
//    SetConsoleCommand("write", write).argumentCompleter(0, autocompletion::files());    // disabled because we use the implementation in Tcl

    SetConsoleCommand("calculate", calculate);

    /**
        @brief Reads the content of a file and executes the commands in it line by line.
    */
    void source(const std::string& filename)
    {
        static std::set<std::string> executingFiles;

        std::set<std::string>::const_iterator it = executingFiles.find(filename);
        if (it != executingFiles.end())
        {
            orxout(user_error) << "Recurring source command in \"" << filename << "\". Stopped execution." << endl;
            return;
        }

        // Open the file
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            orxout(user_error) << "Couldn't open file \"" << filename << "\"." << endl;
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

    /**
        @brief Simply returns the arguments.
    */
    std::string echo(const std::string& text)
    {
        return text;
    }

    /**
        @brief Writes text to a file.
    */
    void write(const std::string& filename, const std::string& text)
    {
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::out);

        if (!file.is_open())
        {
            orxout(user_error) << "Couldn't write to file \"" << filename << "\"." << endl;
            return;
        }

        file << text << endl;
        file.close();
    }

    /**
        @brief Appends text to a file.
    */
    void append(const std::string& filename, const std::string& text)
    {
        std::ofstream file;
        file.open(filename.c_str(), std::fstream::app);

        if (!file.is_open())
        {
            orxout(user_error) << "Couldn't append to file \"" << filename << "\"." << endl;
            return;
        }

        file << text << endl;
        file.close();
    }

    /**
        @brief Reads text from a file
    */
    std::string read(const std::string& filename)
    {
        std::ifstream file;
        file.open(filename.c_str(), std::fstream::in);

        if (!file.is_open())
        {
            orxout(user_error) << "Couldn't read from file \"" << filename << "\"." << endl;
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

    /**
        @brief Parses the mathematical expression and returns the result.
    */
    float calculate(const std::string& calculation)
    {
        ExprParser expr;
        expr.parse(calculation);
        if (expr.getSuccess())
        {
            if (expr.getResult() == 42.0)
            {
                orxout(user_info) << "Greetings from the restaurant at the end of the universe." << endl;
            }
            if (!expr.getRemains().empty())
            {
                orxout(user_warning) << "Expression could not be parsed to the end! Remains: '" << expr.getRemains() << '\'' << endl;
            }
            return static_cast<float>(expr.getResult());
        }
        else
        {
            orxout(user_error) << "Cannot calculate expression: Parse error." << endl;
            return 0;
        }
    }
}
