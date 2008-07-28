
/***************************************************************************
 *  speechsynth.h - Fawkes BlackBoard Interface - SpeechSynthInterface
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

#ifndef __INTERFACES_SPEECHSYNTH_H_
#define __INTERFACES_SPEECHSYNTH_H_

#include <interface/interface.h>
#include <interface/message.h>

namespace fawkes {

class SpeechSynthInterface : public Interface
{
 /// @cond INTERNALS
 INTERFACE_MGMT_FRIENDS(SpeechSynthInterface)
 /// @endcond
 public:
  /* constants */

 private:
  /** Internal data storage, do NOT modify! */
  typedef struct {
    char text[1024]; /**< 
      Last spoken string. Must be properly null-terminated.
     */
  } SpeechSynthInterface_data_t;

  SpeechSynthInterface_data_t *data;

 public:
  /* messages */
  class SayMessage : public Message
  {
   private:
    /** Internal data storage, do NOT modify! */
    typedef struct {
      char text[1024]; /**< 
      Last spoken string. Must be properly null-terminated.
     */
    } SayMessage_data_t;

    SayMessage_data_t *data;

   public:
    SayMessage(const char * ini_text);
    SayMessage();
    ~SayMessage();

    SayMessage(const SayMessage *m);
    /* Methods */
    char * text() const;
    void set_text(const char * new_text);
    size_t maxlenof_text() const;
    virtual Message * clone() const;
  };

  virtual bool message_valid(const Message *message) const;
 private:
  SpeechSynthInterface();
  ~SpeechSynthInterface();

 public:
  virtual Message * create_message(const char *type) const;

  /* Methods */
  char * text() const;
  void set_text(const char * new_text);
  size_t maxlenof_text() const;

};

} // end namespace fawkes

#endif