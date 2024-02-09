//---------------------------------------------------------------------------
// VREng (Virtual Reality Engine)	http://vreng.enst.fr/
//
// Copyright (C) 1997-2024 Philippe Dax
// Telecom-Paris (Ecole Nationale Superieure des Telecommunications)
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
#ifndef ENDIAN_HPP
#define ENDIAN_HPP

/**
 * Endian class
 */
class Endian {
 public:
  Endian() {};
  virtual ~Endian() {};

  // Endians methods
  static bool bigEndian();
  static bool littleEndian();
  static void localEndian(void *data, int nb);
  static void swapShort(uint16_t *array, int len);
  static void swapLong(uint32_t *array, int len);

 private:
  static void * swapEndian(void *data, int nb);

};

#endif
