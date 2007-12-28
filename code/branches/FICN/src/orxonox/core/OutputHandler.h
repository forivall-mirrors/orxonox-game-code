/*
 *   ORXONOX - the hottest 3D action shooter ever to exist
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

/*!
    @file OutputHandler.h
    @brief Definition of the OutputHandler class.

    The OutputHandler acts like std::cout, but output isn't only shown in the console,
    but also written to the logfile.
*/

#ifndef _OutputHandler_H__
#define _OutputHandler_H__

#include <iostream>
#include <fstream>

#include "misc/String.h"
#include "CorePrereqs.h"

namespace orxonox
{
    //! The OutputHandler acts like std::cout, but redirects output to the console AND the logfile.
    class _CoreExport OutputHandler
    {
        public:
            enum OutputDevice
            {
                LD_All,
                LD_Console,
                LD_Logfile,
                LD_Shell
            };

            static OutputHandler& getOutStream();

            /** @returns a reference to the logfile. */
            inline std::ofstream& getLogfile()
                { return this->logfile_; }

            /** @brief Sets the level of the incoming output. @param level The level of the incoming output @return The OutputHandler itself */
            inline OutputHandler& setOutputLevel(int level)
                { this->outputLevel_ = level; return *this; }

            /** @returns the level of the incoming output. */
            inline int getOutputLevel() const
                { return this->outputLevel_; }

            static int getSoftDebugLevel(OutputHandler::OutputDevice device);

            template <class T>
            OutputHandler& output(const T& output);

            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned char val)   { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(short val)           { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned short val)  { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(int val)             { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned int val)    { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(long val)            { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned long val)   { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(float val)           { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(double val)          { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(long double val)     { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(const void* val)     { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(bool val)            { return this->output(val); }

            OutputHandler& operator<<(std::streambuf* sb);

            OutputHandler& operator<<(std::ostream& (*manipulator)(std::ostream&));
            OutputHandler& operator<<(std::ios& (*manipulator)(std::ios&));
            OutputHandler& operator<<(std::ios_base& (*manipulator)(std::ios_base&));

        private:
            explicit OutputHandler(const String& logfilename);
            OutputHandler(const OutputHandler& oh) {}; // don't copy
            virtual ~OutputHandler();
            std::ofstream logfile_;     //!< The logfile where the output is logged
            String logfilename_;   //!< The name of the logfile
            int outputLevel_;           //!< The level of the incoming output
    };

    /**
        @brief Redirects the output to the console and the logfile.
        @param output The value that should be shown in the console
        @return A reference to the OutputHandler itself
    */
    template<class T>
    OutputHandler& OutputHandler::output(const T& output)
    {
        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Console) >= this->outputLevel_)
            std::cout << output;

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Logfile) >= this->outputLevel_)
        {
            this->logfile_ << output;
            this->logfile_.flush();
        }

        return *this;
    }

    /**
        @brief Overloading of the non-member << operator to redirect the output of classes with self defined '<< to std::ostream' operators to the console and the logfile.
        @param out The OutputHandler itself
        @param output The class that should be shown in the console
        @return The OutputHandler itself
    */
    template<class T>
    OutputHandler& operator<<(OutputHandler& out, const T& output)
    {
        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Console) >= out.getOutputLevel())
            std::cout << output;

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Logfile) >= out.getOutputLevel())
        {
            out.getLogfile() << output;
            out.getLogfile().flush();
        }

        return out;
    }

}

#endif /* _OutputHandler_H__ */
