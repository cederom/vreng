//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
//
// Copyright (C) 1997-2011 Philippe Dax
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
#include "vreng.hpp"
#include "dress.hpp"
#include "user.hpp"	// User


const OClass Dress::oclass(DRESS_TYPE, "Dress", Dress::creator);

//local
static uint16_t oid = 0;

struct sDress {
  uint8_t dress_id;
  const char dress_str[16];
};

static struct sDress dresss[] = {
  {Dress::SPIRES,  "spires"},
  {Dress::BANDS,   "bands"},
  {Dress::ROSES,   "roses"},
  {Dress::NODRESS, ""},
};


WObject * Dress::creator(char *l)
{ 
  return new Dress(l);
}

void Dress::defaults()
{
  article = Cloth::DRESS;
  ttl = MAXFLOAT;
  dx = 0;
  dy = 0;
  dz = 0.10;	// hip +10
  dax = 0;
  day = 0;
  daz = 0;
  model = Dress::NODRESS;
}

/* solid geometry */
void Dress::geometry()
{
  char s[128];

  switch (model) {
  case Dress::SPIRES:
    sprintf(s,"solid shape=\"cone\" rb=\"%f\" rt=\"%f\" h=\"%f\" tx=\"%s\" />",.1,.3,-.45,"/gif/pyjama-spires.gif");
    parseSolid(s);
    sprintf(s,"solid shape=\"cone\" rb=\"%f\" rt=\"%f\" h=\"%f\" tx=\"%s\" />",.1,.15,.25,"/gif/pyjama-spires.gif");
    parseSolid(s);
    break;
  case Dress::BANDS:
    sprintf(s,"solid shape=\"cone\" rb=\"%f\" rt=\"%f\" h=\"%f\" tx=\"%s\" />",.1,.25,-.45,"/gif/redbands.gif");
    parseSolid(s);
    sprintf(s,"solid shape=\"cone\" rb=\"%f\" rt=\"%f\" h=\"%f\" tx=\"%s\" />",.1,.15,.25,"/gif/redbands.gif");
    parseSolid(s);
    break;
  case Dress::ROSES:
    sprintf(s,"solid shape=\"cone\" rb=\"%f\" rt=\"%f\" h=\"%f\" tx=\"%s\" />",.1,.2,-.45,"/gif/roses.gif");
    parseSolid(s);
    sprintf(s,"solid shape=\"cone\" rb=\"%f\" rt=\"%f\" h=\"%f\" tx=\"%s\" />",.1,.15,.25,"/gif/roses.gif");
    parseSolid(s);
    break;
  default:
    break;
  }
}

/* Created from file */
void Dress::parser(char *l)
{
  defaults();
  l = tokenize(l);
  begin_while_parse(l) {
    l = parseAttributes(l);	// <solid ... />
    if (!l) break;
    if (! stringcmp(l, "model=")) {
      l = parseString(l, modelname, "model");
      if      (! stringcmp(modelname, "spires")) model = SPIRES;
      else if (! stringcmp(modelname, "bands"))  model = BANDS;
      else if (! stringcmp(modelname, "roses"))  model = ROSES;
    }
  }
  end_while_parse(l);
}

uint8_t Dress::getModel(const char *name)
{
  struct sDress *pdresss = dresss;

  if (name) {
    for ( ; pdresss; pdresss++)
      if (! strcmp(name, pdresss->dress_str))
        return pdresss->dress_id;
  }
  return NODRESS;
}

/* Created from file */
Dress::Dress(char *l)
{
  model = Dress::SPIRES;
  strcpy(modelname, "spires");
  parser(l);
  taken = false;
  behavior();
}

/* Regenerated by world via MySql */
Dress::Dress(User *user, void *d, time_t s, time_t u)
{
  char *str = (char *) d;       // name transmitted
  if (!str) return;

  strcpy(names.given, str);
  strcpy(names.type, typeName());     // need names.type for MySql
  char *p = strchr(str, '&');
  *p = '\0';
  strcpy(modelname, str);

  defaults();
  taken = true;
  model = getModel(modelname);
  setName(modelname);
  setOwner();
  getPersist();
  geometry();
  behavior();
  inits();
}

void Dress::quit()
{
  oid = 0;
  savePersistency();
}

/* wear */
void Dress::wear()
{
  if (taken) takeoff();

  taken = true;
  //model = getModel(modelname);
  //setName(modelname);
  setOwner();
  setPersist();
  behavior();
  inits();
  addWearList();
}

/* takeoff */
void Dress::takeoff()
{
  taken = false;
  restorePosition();	// restore original position
  delPersist();
  delWearList();
}

/* wear: indirectly called by user */
void Dress::wear_cb(Dress *dress, void *d, time_t s, time_t u)
{
  dress->wear();
}

/* takeoff: indirectly called by user */
void Dress::takeoff_cb(Dress *dress, void *d, time_t s, time_t u)
{
  dress->takeoff();
}

/* Creation: this method is invisible: called by the World */
void Dress::recreate_cb(User *pu, void *d, time_t s, time_t u)
{
  new Dress(pu, d, s, u);
}

void Dress::funcs()
{
  getPropertyFunc(DRESS_TYPE, PROPXY, _Payload get_xy);
  getPropertyFunc(DRESS_TYPE, PROPZ, _Payload get_z);
  getPropertyFunc(DRESS_TYPE, PROPAZ, _Payload get_az);
  getPropertyFunc(DRESS_TYPE, PROPAX, _Payload get_ax);
  getPropertyFunc(DRESS_TYPE, PROPAY, _Payload get_ay);
  getPropertyFunc(DRESS_TYPE, PROPHNAME, _Payload get_hname);

  putPropertyFunc(DRESS_TYPE, PROPXY, _Payload put_xy);
  putPropertyFunc(DRESS_TYPE, PROPZ, _Payload put_z); 
  putPropertyFunc(DRESS_TYPE, PROPAZ, _Payload put_az);
  putPropertyFunc(DRESS_TYPE, PROPAX, _Payload put_ax);
  putPropertyFunc(DRESS_TYPE, PROPAY, _Payload put_ay);
  putPropertyFunc(DRESS_TYPE, PROPHNAME, _Payload put_hname);

  setActionFunc(DRESS_TYPE, WEAR, _Action wear_cb, "Wear");
  setActionFunc(DRESS_TYPE, TAKEOFF, _Action takeoff_cb, "Takeoff");

  setActionFunc(DRESS_TYPE, RECREATE, _Action recreate_cb, "");
}
