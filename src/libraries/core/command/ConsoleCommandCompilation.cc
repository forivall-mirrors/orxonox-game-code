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
    SetConsoleCommand("echo", echo);

    SetConsoleCommand("orxout", orxout_level);
    SetConsoleCommand("orxout_context", orxout_level_context);

    SetConsoleCommand("log"    , log    );
    SetConsoleCommand("error"  , error  ).hide();
    SetConsoleCommand("warning", warning).hide();
    SetConsoleCommand("status" , status ).hide();
    SetConsoleCommand("info"   , info   ).hide();
    SetConsoleCommand("debug"  , debug  ).hide();

//    SetConsoleCommand("source", source).argumentCompleter(0, autocompletion::files());  // disabled because we use the implementation in Tcl
//    SetConsoleCommand("read", read).argumentCompleter(0, autocompletion::files());      // disabled because we use the implementation in Tcl
//    SetConsoleCommand("append", append).argumentCompleter(0, autocompletion::files());  // disabled because we use the implementation in Tcl
//    SetConsoleCommand("write", write).argumentCompleter(0, autocompletion::files());    // disabled because we use the implementation in Tcl

    SetConsoleCommand("calculate", calculate);

    /**
        @brief Simply returns the arguments.
    */
    std::string echo(const std::string& text)
    {
        return text;
    }

    /**
        @brief Builds a map that maps the levels of all output levels to their ID.
    */
    std::map<std::string, OutputLevel> getOutputLevelsMap()
    {
        std::map<std::string, OutputLevel> levels;

        levels["message"]          = level::message;
        levels["debug_output"]     = level::debug_output;
        levels["user_error"]       = level::user_error;
        levels["user_warning"]     = level::user_warning;
        levels["user_status"]      = level::user_status;
        levels["user_info"]        = level::user_info;
        levels["internal_error"]   = level::internal_error;
        levels["internal_warning"] = level::internal_warning;
        levels["internal_status"]  = level::internal_status;
        levels["internal_info"]    = level::internal_info;
        levels["verbose"]          = level::verbose;
        levels["verbose_more"]     = level::verbose_more;
        levels["verbose_ultra"]    = level::verbose_ultra;

        return levels;
    }

    /**
        @brief Prints text to the console.
        @param level_name The name of the output level
    */
    void orxout_level(const std::string& level_name, const std::string& text)
    {
        static std::map<std::string, OutputLevel> levels = getOutputLevelsMap();

        OutputLevel level = level::debug_output;
        std::map<std::string, OutputLevel>::iterator it = levels.find(level_name);
        if (it != levels.end())
            level = it->second;
        else
            orxout(internal_warning) << "'" << level_name << "' is not a valid output level" << endl;

        orxout(level) << text << endl;
    }

    /**
        @brief Prints text to the console.
        @param level_name The name of the output level
        @param context_name The name of the output context
    */
    void orxout_level_context(const std::string& level_name, const std::string& context_name, const std::string& text)
    {
        static std::map<std::string, OutputLevel> levels = getOutputLevelsMap();

        OutputLevel level = level::debug_output;
        std::map<std::string, OutputLevel>::iterator it = levels.find(level_name);
        if (it != levels.end())
            level = it->second;
        else
            orxout(internal_warning) << "'" << level_name << "' is not a valid output level" << endl;

        OutputContextContainer context = registerContext(context_name);

        orxout(level, context) << text << endl;
    }

    /// @brief Prints text to the console and the logfile.
    void log(const std::string& text)
    { orxout() << text << endl; }

    /// @brief Prints output with error level.
    void error(const std::string& text)
    { orxout(user_error) << text << endl; }

    /// @brief Prints output with warning level.
    void warning(const std::string& text)
    { orxout(user_warning) << text << endl; }

    /// @brief Prints output with status level.
    void status(const std::string& text)
    { orxout(user_status) << text << endl; }

    /// @brief Prints output with info level.
    void info(const std::string& text)
    { orxout(user_info) << text << endl; }

    /// @brief Prints debug output with verbose level.
    void debug(const std::string& text)
    { orxout(verbose, context::tcl) << text << endl; }

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
