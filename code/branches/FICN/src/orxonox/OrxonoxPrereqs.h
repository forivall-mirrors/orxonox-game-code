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
 *      Reto Grieder
 *   Co-authors:
 *      ...
 *
 */

/**
 @file  OrxonoxPrereq.h
 @brief Contains all the necessary forward declarations for all classes and structs.
 */

#ifndef _OrxonoxPrereqs_H__
#define _OrxonoxPrereqs_H__

#include "OrxonoxPlatform.h"

// classes that have not yet been put into a namespace
class InputManager;
class SpaceShipSteering;

// TinyXML
class TiXmlString;
class TiXmlOutStream;
class TiXmlNode;
class TiXmlHandle;
class TiXmlDocument;
class TiXmlElement;
class TiXmlComment;
class TiXmlUnknown;
class TiXmlAttribute;
class TiXmlText;
class TiXmlDeclaration;
class TiXmlParsingData;

namespace orxonox {
  class Ambient;
  class BaseEntity;
  class BaseObject;
  class Camera;
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

  class AmmunitionDump;
  class Bullet;
  class BulletManager;
  class BaseWeapon;
  class BarrelGun;
  class WeaponStation;

  class ParticleInterface;
  class HUD;
}

namespace audio {
  class AudioBuffer;
  class AudioManager;
  class AudioSource;
  class AudioStream;
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

#endif /* _OrxonoxPrereqs_H__ */
