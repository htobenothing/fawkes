
/***************************************************************************
 *  laser360.cpp - Fawkes BlackBoard Interface - Laser360Interface
 *
 *  Templated created:   Thu Oct 12 10:49:19 2006
 *  Copyright  2008  Tim Niemueller
 *
 *  $Id$
 *
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version. A runtime exception applies to
 *  this software (see LICENSE.GPL_WRE file mentioned below for details).
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL_WRE file in the doc directory.
 */

#include <interfaces/laser360.h>

#include <core/exceptions/software.h>

#include <cstring>
#include <cstdlib>

namespace fawkes {

/** @class Laser360Interface <interfaces/laser360.h>
 * Laser360Interface Fawkes BlackBoard Interface.
 * 
      This interface provides access to data of a laser scanner that produces
      360 beams per scan.
    
 */



/** Constructor */
Laser360Interface::Laser360Interface() : Interface()
{
  data_size = sizeof(Laser360Interface_data_t);
  data_ptr  = malloc(data_size);
  data      = (Laser360Interface_data_t *)data_ptr;
  memset(data_ptr, 0, data_size);
  add_fieldinfo(Interface::IFT_FLOAT, "distances", &data->distances);
  unsigned char tmp_hash[] = {0xc4, 0x7a, 0xf3, 0xa0, 0x4, 0x6, 0x97, 0x1d, 0xdb, 0x17, 0xfe, 0x5e, 0xd0, 0x9b, 0xa, 0xa3};
  set_hash(tmp_hash);
}

/** Destructor */
Laser360Interface::~Laser360Interface()
{
  free(data_ptr);
}
/* Methods */
/** Get distances value.
 * 
      The distances in meter of the beams.
    
 * @return distances value
 */
float *
Laser360Interface::distances() const
{
  return data->distances;
}

/** Get maximum length of distances value.
 * @return length of distances value, can be length of the array or number of 
 * maximum number of characters for a string
 */
size_t
Laser360Interface::maxlenof_distances() const
{
  return 360;
}

/** Set distances value.
 * 
      The distances in meter of the beams.
    
 * @param new_distances new distances value
 */
void
Laser360Interface::set_distances(const float * new_distances)
{
  memcpy(data->distances, new_distances, sizeof(float) * 360);
}

/* =========== message create =========== */
Message *
Laser360Interface::create_message(const char *type) const
{
  throw UnknownTypeException("The given type '%s' does not match any known "
                             "message type for this interface type.", type);
}


/* =========== messages =========== */
/** Check if message is valid and can be enqueued.
 * @param message Message to check
 */
bool
Laser360Interface::message_valid(const Message *message) const
{
  return false;
}

/// @cond INTERNALS
EXPORT_INTERFACE(Laser360Interface)
/// @endcond


} // end namespace fawkes