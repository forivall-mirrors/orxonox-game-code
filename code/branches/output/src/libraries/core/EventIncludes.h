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
    @ingroup Event
    @brief Definition of the XMLPortEventState() macro, as well as some more useful macros.
*/

#ifndef _EventIncludes_H__
#define _EventIncludes_H__

#include "CorePrereqs.h"

#include "Event.h"
#include "XMLPort.h"
#include "command/Executor.h"

/**
    @brief Defines a new event state (a state of the object which can be changed by events).

    @param classname    The name of this class
    @param subclassname Usually BaseObject - if different, only instances of this class can send events to this object
    @param statename    The name (string) of this state
    @param function     The function which should be used to set the state
    @param xmlelement   Argument for XMLPort
    @param mode         Argument for XMLPort
*/
#define XMLPortEventState(classname, subclassname, statename, function, xmlelement, mode) \
    orxonox::EventState* containername##function = this->getEventState(statename); \
    if (!containername##function) \
    { \
        containername##function = new orxonox::EventState(orxonox::createFunctor(&classname::function, this), orxonox::ClassIdentifier<subclassname>::getIdentifier()); \
        this->addEventState(statename, containername##function); \
    } \
    XMLPortEventStateIntern(xmlportevent##function, classname, statename, xmlelement, mode)

/**
    @brief Like XMLPortEventState but creates an event sink instead of an event state.
           The most important destinction between an EventState and an EventSink is, that an EventState only processes events which change the state of the EventState, where as an EventSink is an EventState that processes any Event that reaches it.
*/

#define XMLPortEventSink(classname, subclassname, statename, function, xmlelement, mode) \
    orxonox::EventState* containername##function = this->getEventState(statename); \
    if (!containername##function) \
    { \
        containername##function = new orxonox::EventState(orxonox::createFunctor(&classname::function, this), orxonox::ClassIdentifier<subclassname>::getIdentifier(), true); \
        this->addEventState(statename, containername##function); \
    } \
    XMLPortEventStateIntern(xmlportevent##function, classname, statename, xmlelement, mode)


#define XMLPortEventStateTemplate(classname, subclassname, statename, function, xmlelement, mode, ...) \
    orxonox::EventState* containername##function = this->getEventState(statename); \
    if (!containername##function) \
    { \
        containername##function = new orxonox::EventState(orxonox::createFunctor<classname, __VA_ARGS__ >(&classname::function, this), orxonox::ClassIdentifier<subclassname>::getIdentifier()); \
        this->addEventState(statename, containername##function); \
    } \
    XMLPortEventStateIntern(xmlportevent##function, classname, statename, xmlelement, mode)

#define XMLPortEventStateIntern(name, classname, statename, xmlelement, mode) \
    static orxonox::ExecutorMemberPtr<classname> xmlsetfunctor##name = orxonox::createExecutor(orxonox::createFunctor(&classname::addEventSource), std::string( #classname ) + "::" + "addEventSource" + '(' + statename + ')').cast<orxonox::ExecutorMember<classname> >(); \
    static orxonox::ExecutorMemberPtr<classname> xmlgetfunctor##name = orxonox::createExecutor(orxonox::createFunctor(&classname::getEventSource), std::string( #classname ) + "::" + "getEventSource" + '(' + statename + ')').cast<orxonox::ExecutorMember<classname> >(); \
    xmlsetfunctor##name->setDefaultValue(1, statename); \
    xmlgetfunctor##name->setDefaultValue(1, statename); \
    XMLPortObjectGeneric(xmlport##name, classname, orxonox::BaseObject, statename, xmlsetfunctor##name, xmlgetfunctor##name, xmlelement, mode, false, true)


/**
    @brief Defines a new event name for a class. Named events can only have names which were defined with this macro.

    @param classname The name of the class
    @param name      The name of the event
*/
#define CreateEventName(classname, name) \
    static std::string eventname##classname##name = #name

/**
    @brief This macro is needed to fire an event with this name. The event name must previously be declared with @ref CreateEventName.
*/
#define FireEventName(classname, name) \
    eventname##classname##name

#endif /* _EventIncludes_H__ */
