
/***************************************************************************
 *  message_content.h - Fawkes network message content
 *
 *  Created: Fri Jun 01 13:29:09 2007
 *  Copyright  2006-2007  Tim Niemueller [www.niemueller.de]
 *
 *  $Id$
 *
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software Foundation,
 *  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02111-1307, USA.
 */

#ifndef __NETCOMM_FAWKES_MESSAGE_CONTENT_H_
#define __NETCOMM_FAWKES_MESSAGE_CONTENT_H_

#include <sys/types.h>
#include <cstddef>

class FawkesNetworkMessageContent
{
 public:
  FawkesNetworkMessageContent();
  virtual ~FawkesNetworkMessageContent();

  virtual void   serialize() = 0;
  virtual void * payload();
  virtual size_t payload_size();

 protected:
  void copy_payload(size_t offset, void *buf, size_t len);

 protected:
  /** Pointer to payload. */
  void *  _payload;
  /** Payloda size. */
  size_t  _payload_size;
};


#endif
