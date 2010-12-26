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
 *      Benjamin Knecht <beni_at_orxonox.net>, (C) 2007
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
@file
@brief
*/

#include "Analyser.h"

#include <fstream>
#include <list>
#include <map>
#include <set>
#include <QFileInfo>

#include "core/PathConfig.h"

namespace orxonox
{
    void lowercase(std::string* str)
    {
        for (size_t i = 0; i < str->size(); ++i)
        {
            (*str)[i] = (char)tolower((*str)[i]);
        }
    }

    void parse()
    {
        std::string name = "tracker";
        std::ifstream file;
        file.open((PathConfig::getDataPathString() + name + "_includes_pch.txt").c_str());

        std::map<std::string, std::set<std::string> > triggeredFiles;
        std::map<std::string, std::pair<size_t, size_t> > totalTimesIncluded;
        std::set<std::string> locallyIncluded;
        std::vector<std::string> includeStack;
        std::set<std::string> pchFiles;
        bool inPCH = false;

        while (file.good())
        {
            std::string line;
            std::getline(file, line);
            if (line.size() >= 8 && line.substr(2, 6) == "Note: ")
            {
                // Count white spaces
                size_t indentation = 24;
                while (line[indentation] == ' ')
                    ++indentation;
                size_t stackLevel = indentation - 24;
                QFileInfo info(QString::fromStdString(line.substr(indentation)));
                std::string filePath = info.canonicalFilePath().toStdString();
                lowercase(&filePath);
                if (inPCH)
                {
                    pchFiles.insert(filePath);
                }
                else
                {
                    if (pchFiles.find(filePath) == pchFiles.end())
                    {
                        while (stackLevel + 1 <= includeStack.size())
                            includeStack.pop_back();

                        triggeredFiles[filePath].insert(filePath);

                        std::set<std::string>& set = triggeredFiles[filePath];
                        for (std::set<std::string>::iterator it = set.begin();
                            it != set.end(); ++it)
                        {
                            for (size_t i = 0; i < includeStack.size(); ++i)
                                triggeredFiles[includeStack[i]].insert(*it);
                        }

                        includeStack.push_back(filePath);
                        locallyIncluded.insert(filePath);
                    }
                }
            }
            else if (line.find("PrecompiledHeaders") != std::string::npos)
            {
                // Included in PCH file
                inPCH = true;
            }
            else
            {
                // New file
                inPCH = false;
                for (std::set<std::string>::const_iterator it = locallyIncluded.begin();
                    it != locallyIncluded.end(); ++it)
                {
                    std::map<std::string, std::pair<size_t, size_t> >::iterator mapIt = totalTimesIncluded.find(*it);
                    if (mapIt != totalTimesIncluded.end())
                        mapIt->second.first += 1;
                    else
                        totalTimesIncluded[*it].first = 1;
                }

                includeStack.clear();
                locallyIncluded.clear();
            }
        }

        // Get all file sizes
        for (std::map<std::string, std::pair<size_t, size_t> >::iterator it = totalTimesIncluded.begin();
            it != totalTimesIncluded.end(); ++it)
        {
            QFileInfo info(QString::fromStdString(it->first));
            it->second.second = static_cast<size_t>(info.size());
        }

        struct IncludeFile
        {
            std::string filename;
            size_t timesIncluded;
            size_t fileSize;
            size_t bytesIncluded;
            size_t totalBytesIncluded;
        };
        std::list<IncludeFile> allFiles;

        for (std::map<std::string, std::pair<size_t, size_t> >::const_iterator it = totalTimesIncluded.begin();
            it != totalTimesIncluded.end(); ++it)
        {
            size_t totalFileSize = 0;
            assert(triggeredFiles.find(it->first) != triggeredFiles.end());
            std::set<std::string>& fileSet = triggeredFiles[it->first];
            //if (it->first.find("scenemanager") != std::string::npos)
            //    int a = 8;
            for (std::set<std::string>::const_iterator setIt = fileSet.begin(); setIt != fileSet.end(); ++setIt)
            {
                totalFileSize += totalTimesIncluded[*setIt].second;
            }

            // Insert
            IncludeFile file = { it->first, it->second.first, it->second.second, totalFileSize, totalFileSize * it->second.first };
            if (allFiles.empty())
            {
                allFiles.insert(allFiles.end(), file);
            }
            else
            {
                bool inserted = false;
                for (std::list<IncludeFile>::iterator listIt = allFiles.begin(); listIt != allFiles.end(); ++listIt)
                {
                    if (file.timesIncluded > listIt->timesIncluded || file.timesIncluded == listIt->timesIncluded && file.totalBytesIncluded >= listIt->totalBytesIncluded)
                    {
                        allFiles.insert(listIt, file);
                        inserted = true;
                        break;
                    }
                }
                if (!inserted)
                    allFiles.push_back(file);
            }
        }

        // Clean list
        //for (std::list<IncludeFile>::iterator listIt = allFiles.begin(); listIt != allFiles.end(); ++listIt)
        //{
        //    bool isCore = (listIt->filename.find("src/core/") != std::string::npos);
        //    for (std::list<IncludeFile>::const_iterator listIt2 = allFiles.begin(); listIt2 != listIt; ++listIt2)
        //    {
        //        std::set<std::string>& fileSet = triggeredFiles[listIt2->filename];
        //        std::set<std::string>::const_iterator setIt = fileSet.find(listIt->filename);
        //        if (setIt != fileSet.end())// && listIt->timesIncluded < listIt2->timesIncluded)
        //        {
        //            if (isCore || listIt2->filename.find("src/core/") == std::string::npos)
        //            {
        //                // File already gets included by a 'worse' one --> remove
        //                listIt = allFiles.erase(listIt);
        //                --listIt;
        //                break;
        //            }
        //        }
        //    }
        //}

        // Write results
        std::ofstream outFile;
        outFile.open((PathConfig::getDataPathString() + name + "_results_pch.txt").c_str());
        for (std::list<IncludeFile>::const_iterator listIt = allFiles.begin(); listIt != allFiles.end(); ++listIt)
        {
            outFile.width(10);
            outFile << listIt->totalBytesIncluded;
            outFile.width(10);
            outFile << listIt->bytesIncluded;
            outFile.width(5);
            outFile << listIt->timesIncluded;
            outFile << "  " << listIt->filename;
            outFile << listIt->filename;
            outFile << std::endl;
        }
    }
}
