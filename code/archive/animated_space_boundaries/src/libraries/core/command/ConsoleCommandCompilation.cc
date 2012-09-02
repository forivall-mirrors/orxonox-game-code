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

#include "util/Debug.h"
#include "util/ExprParser.h"
#include "util/StringUtils.h"
#include "ConsoleCommand.h"
#include "CommandExecutor.h"

namespace orxonox
{
//    SetConsoleCommand("source", source).argumentCompleter(0, autocompletion::files());  // disabled because we use the implementation in Tcl
    SetConsoleCommand("echo", echo);
//    SetConsoleCommand("puts", puts);                                                    // disabled because we use the implementation in Tcl

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

    /**
        @brief Simply returns the arguments.
    */
    std::string echo(const std::string& text)
    {
        return text;
    }

    /**
        @brief Writes text to the console, depending on the first argument with or without a line-break after it.
    */
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

    /**
        @brief Writes text to a file.
    */
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

    /**
        @brief Appends text to a file.
    */
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

    /**
        @brief Reads text from a file
    */
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
