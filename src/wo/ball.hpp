//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
//
// Copyright (C) 1997-2009 Philippe Dax
// Telecom-ParisTech (Ecole Nationale Superieure des Telecommunications)
//
// VREng is a free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public Licence as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// VREng is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//---------------------------------------------------------------------------
#ifndef BALL_HPP
#define BALL_HPP

#include "wobject.hpp"

#define BALL_TYPE	5
#define BALL_NAME	"Ball"


/**
 * Ball class
 */
class Ball: public WObject {

 private:
  float lspeed;		///< linear speed in xy
  float aspeed;		///< angular speed
  float zspeed;		///< linear speed in z
  float gravity;	///< gravity in z
  float ttl;		///< time to live
  float origz;		///< position at creation
  bool taken;		///< taken or not by an user

 public:
  static const float LSPEED;
  static const float ZSPEED;
  static const float ASPEED;
  static const float ORIGZ;
  static const float SHIFT;
  static const float RADIUS;
  static const float DELTAZ;
  static const float TTL;

  /* properties */
  enum {
    PROPHNAME,
    PROPXY,
    PROPZ,
    PROPAZ,
    PROPAX,
    PROPAY,
    PROPS
  };

  /* actions */
  enum {
    PUSH,	///< forward
    PULL,	///< backward
    SHOOT,	///< shoot
    UP,		///< move up
    TAKE,	///< take
    DROP,	///< drop
    TURN,	///< turn
    KILL,	///< destroy
    CREATE,	///< create
    RECREATE,	///< create
    NONE
  };

  static const OClass oclass;	///< class variable

  static void funcs();	///< init funclist

  virtual const OClass* getOClass() {return &oclass;}

  Ball(class WObject *o, void *d, time_t s, time_t u);
  /**< constructor: called by cauldron. */

  Ball(class World *world, void *d, time_t s, time_t u);
  /**< constructor: called by world. */

  Ball(uint8_t type_id, Noid noid, Payload *pp);
  /**< constructor: network replication. */

  Ball(WObject *user, char *solid);
  /**< constructor: created by user. */

  Ball(char *l);
  /**< constructor: fileline. */

  static WObject * replicator(uint8_t type_id, Noid noid, Payload *pp);
  /**< Replicates a ball coming from the Network. */

  static WObject * (creator)(char *l);
  /**< Creates a ball defined in a file. */

  virtual bool isMoving();
  /**< Checks if object is moving. */

  virtual void changePosition(float lasting);
  /**< Does any position changes. */

  virtual void changePermanent(float lasting);
  /**< Does permanent position changes. */

  virtual void updateTime(time_t s, time_t u, float *lasting);
  /**< Updates times. */

  virtual bool updateToNetwork(const Pos &oldpos);
  /**< Publishes to network. */

  virtual bool whenIntersect(WObject *pcur, WObject *pold);
  /**< Handles collisions. */

  virtual void whenWallsIntersect(WObject *pold, V3 *norm);
  /**< Handles collisions with a wall. */

  virtual void quit();
  /**< Quits. */

 private:
  virtual void defaults();
  /**< Sets defaults values. */

  virtual void parser(char *l);
  /**< Parses fileline. */

  virtual void makeSolid();
  /**< Builds solid geometry. */

  virtual void setName();
  /**< Sets an implicited name. */

  // Actions
  virtual void push();
  virtual void pull();
  virtual void shoot();
  virtual void up();
  virtual void take();
  virtual void drop();
  virtual void turn();
  virtual void destroy();

  // GUI callbacks
  static void push_cb(Ball *o, void *d, time_t s, time_t u);
  static void pull_cb(Ball *o, void *d, time_t s, time_t u);
  static void shoot_cb(Ball *o, void *d, time_t s, time_t u);
  static void up_cb(Ball *o, void *d, time_t s, time_t u);
  static void take_cb(Ball *o, void *d, time_t s, time_t u);
  static void drop_cb(Ball *o, void *d, time_t s, time_t u);
  static void turn_cb(Ball *o, void *d, time_t s, time_t u);
  static void destroy_cb(Ball *o, void *d, time_t s, time_t u);
  static void create_cb(class Cauldron *c, void *d, time_t s, time_t u);
  static void recreate_cb(class World *w, void *d, time_t s, time_t u);
};

#endif
