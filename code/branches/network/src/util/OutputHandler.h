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
    @file OutputHandler.h
    @brief Definition of the OutputHandler class.

    The OutputHandler acts like std::cout, but output isn't only shown in the console,
    but also written to the logfile.
*/

#ifndef _OutputHandler_H__
#define _OutputHandler_H__

#include "UtilPrereqs.h"

#include <iostream>
#include <fstream>
#include <string>

#include "OutputBuffer.h"

namespace orxonox
{
    //! The OutputHandler acts like std::cout, but redirects output to the console AND the logfile.
    class _UtilExport OutputHandler
    {
        public:
            enum OutputDevice
            {
                LD_All = 0,
                LD_Console = 1,
                LD_Logfile = 2,
                LD_Shell = 3
            };

            static OutputHandler& getOutStream();

            /** @brief Puts some text on the outstream. @param text The text */
            static inline std::string log(const std::string& text)
                { OutputHandler::getOutStream().setOutputLevel(0); OutputHandler::getOutStream().output(text + "\n"); return text; }

            /** @brief Puts an error on the outstream. @param text The text */
            static inline std::string error(const std::string& text)
                { OutputHandler::getOutStream().setOutputLevel(1); OutputHandler::getOutStream().output(text + "\n"); return text; }

            /** @brief Puts a warning on the outstream. @param text The text */
            static inline std::string warning(const std::string& text)
                { OutputHandler::getOutStream().setOutputLevel(2); OutputHandler::getOutStream().output(text + "\n"); return text; }

            /** @brief Puts an info on the outstream. @param text The text */
            static inline std::string info(const std::string& text)
                { OutputHandler::getOutStream().setOutputLevel(3); OutputHandler::getOutStream().output(text + "\n"); return text; }

            /** @brief Puts some debug output on the outstream. @param text The text */
            static inline std::string debug(const std::string& text)
                { OutputHandler::getOutStream().setOutputLevel(4); OutputHandler::getOutStream().output(text + "\n"); return text; }

            /** @brief Returns a reference to the logfile. @return The logfile */
            inline std::ofstream& getLogfile()
                { return this->logfile_; }

            /** @brief Returns a pointer to the OutputBuffer. @return The OutputBuffer */
            inline OutputBuffer* getOutputBuffer()
                { return this->outputBuffer_; }

            /** @brief Sets the level of the incoming output. @param level The level of the incoming output @return The OutputHandler itself */
            inline OutputHandler& setOutputLevel(int level)
                { this->outputLevel_ = level; return *this; }

            /** @brief Returns the level of the incoming output. @return The level */
            inline int getOutputLevel() const
                { return this->outputLevel_; }

            static void setSoftDebugLevel(OutputHandler::OutputDevice device, int level);
            static int getSoftDebugLevel(OutputHandler::OutputDevice device = OutputHandler::LD_All);

            void setOutputBuffer(OutputBuffer& buffer);

            template <class T>
            OutputHandler& output(const T& output);

            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned char val)      { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(short val)              { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned short val)     { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(int val)                { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned int val)       { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(long val)               { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned long val)      { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(long long val)          { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(unsigned long long val) { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(float val)              { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(double val)             { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(long double val)        { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(const void* val)        { return this->output(val); }
            /** @brief Overloaded << operator, redirects the output to the console and the logfile. @param val The value that should be shown in the console @return A reference to the OutputHandler itself */
            inline OutputHandler& operator<<(bool val)               { return this->output(val); }

            OutputHandler& operator<<(std::streambuf* sb);

            OutputHandler& operator<<(std::ostream& (*manipulator)(std::ostream&));
            OutputHandler& operator<<(std::ios& (*manipulator)(std::ios&));
            OutputHandler& operator<<(std::ios_base& (*manipulator)(std::ios_base&));

        private:
            explicit OutputHandler(const std::string& logfilename);
            OutputHandler(const OutputHandler& oh);  // don't copy
            virtual ~OutputHandler();

            std::ofstream logfile_;              //!< The logfile where the output is logged
            std::string logfilename_;            //!< The name of the logfile
            OutputBuffer fallbackBuffer_;        //!< The OutputBuffer that gets used if there is no other OutputBuffer
            OutputBuffer* outputBuffer_;         //!< The OutputBuffer to put output in (usually used by the Shell)
            int outputLevel_;                    //!< The level of the incoming output
            int softDebugLevel_[4];              //!< The soft debug level for each OutputDevice - the configurable maximal output level
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

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Shell) >= this->outputLevel_)
            (*this->outputBuffer_) << output;

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

        if (OutputHandler::getSoftDebugLevel(OutputHandler::LD_Shell) >= out.getOutputLevel())
            (*out.getOutputBuffer()) << output;

        return out;
    }
}

#endif /* _OutputHandler_H__ */
