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
 *      Reto Grieder, (C) 2007
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  OrxonoxPrereq.cc
 @brief Contains all the necessary forward declarations for all classes, structs and enums.
 */

#ifndef ORXONOX_PREREQ_H
#define ORXONOX_PREREQ_H

// hack for the usleep/Sleep problem
#ifdef WIN32
#include <windows.h>
#define usleep(x) Sleep((x)/1000)
#else
#include <unistd.h>
#endif

// classes that have not yet been put into a namespace
class InputManager;
class SignalHandler;
class SpaceShipSteering;
template <class T>
class String2Number;

namespace orxonox {
  class Ambient;
  class BaseEntity;
  class BaseObject;
  class Camera;
  template <class T>
  class ClassFactory;
  class Entity;
  class GraphicsEngine;
  class Mesh;
  class Model;
  class NPC;
  class OrxListener;
  class Orxonox;
  class SceneNode;
  class Skybox;
  class SpaceShip;
  class SpaceshipSteeringObject;
  class Tickable;
  class TickFrameListener;
  template <class T>
  class Timer;
  class TimerBase;
  class TimerFrameListener;
  class WorldEntity;

  // from core library
  class ArgReader;
  class BaseFactory;
  class BaseMetaObjectListElement;
  template <class T>
  class ClassFactory;
  template <class T>
  class ClassIdentifier;
  class ConfigValueContainer;
  class DebugLevel;
  class Error;
  class Factory;
  class Identifier;
  class IdentifierList;
  class IdentifierListElement;
  template <class T>
  class Iterator;
  class MetaObjectList;
  template <class T>
  class MetaObjectListElement;
  template <class T>
  class ObjectList;
  template <class T>
  class ObjectListElement;
  class OrxonoxClass;
  template <class T>
  class SubclassIdentifier;
}

namespace audio {
  class AudioBuffer;
  class AudioManager;
  class AudioSource;
  class AudioStream;
}

namespace hud {
  class HUD;
}

namespace loader {
  class LevelLoader;
}

namespace network {
  class Client;
  class ClientConnection;
  class ClientFrameListener;
  class ClientInformation;
  class ConnectionManager;
  class GameStateClient;
  class GameStateManager;
  class PacketBuffer;
  class PacketDecoder;
  class PacketGenerator;
  class Server;
  class ServerFrameListener;
  class Synchronisable;
  struct ack;
  struct chat;
  struct classid;
  struct ClientList;
  struct GameState;
  struct GameStateCompressed;
  struct keyboard;
  struct mouse;
  struct PacketEnvelope;
  struct QueueItem;
  struct synchData;
  struct synchronisableVariable;

}

namespace particle {
  class ParticleInterface;
}

#endif /* ORXONOX_PREREQ_H */
