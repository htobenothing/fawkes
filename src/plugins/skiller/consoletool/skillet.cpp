
/***************************************************************************
 *  skillet.cpp - Skiller console tool
 *
 *  Created: Sat Mar 15 13:57:22 2008
 *  Copyright  2006-2008  Tim Niemueller [www.niemueller.de]
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

#include <netcomm/fawkes/client.h>
#include <blackboard/remote.h>
#include <utils/system/argparser.h>
#include <utils/system/signal.h>
#include <core/threading/thread.h>
#include <netcomm/fawkes/client_handler.h>

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <csignal>
#include <string>

#include <readline/readline.h>
#include <readline/history.h>

#include <interfaces/skiller.h>

void
print_usage(const char *program_name)
{
}

static int
event_hook()
{
  return 0;
}


/** Skill shell thread.
 * This thread opens a network connection to a host and uses a RemoteBlackBoard
 * connection to send skill strings for execution. It also shows Skiller log messages
 * and uses the skiller network protocol.
 * @author Tim Niemueller
 */
class SkillShellThread : public Thread, public FawkesNetworkClientHandler
{
 public:
  /** Constructor.
   * @param argp argument parser
   */
  SkillShellThread(ArgumentParser *argp)
    : Thread("SkillShellThread", Thread::OPMODE_CONTINUOUS)
  {
    this->argp = argp;
    prompt = "-# ";
    just_connected = true;
    connection_died_recently = false;

    sif = NULL;
    using_history();
    // this is needed to get rl_done working
    rl_event_hook = event_hook;

    c = new FawkesNetworkClient("localhost", 1910);
    c->register_handler(this, FAWKES_CID_SKILLER_PLUGIN);
    c->connect();
  }

  /** Destructor. */
  ~SkillShellThread()
  {
    printf("Finalizing\n");

    SkillerInterface::ReleaseControlMessage *rcm = new SkillerInterface::ReleaseControlMessage();
    sif->msgq_enqueue(rcm);

    usleep(500000);

    rbb->close(sif);
    delete rbb;
    rbb = NULL;

    c->deregister_handler(FAWKES_CID_SKILLER_PLUGIN);
    c->disconnect();
    delete c;
  }


  virtual void loop()
  {
    if ( c->connected() ) {
      if ( just_connected ) {
	just_connected = false;
	try {
	  rbb = new RemoteBlackBoard(c);
	  sif = rbb->open_for_reading<SkillerInterface>("Skiller");
	  SkillerInterface::AcquireControlMessage *aqm = new SkillerInterface::AcquireControlMessage();
	  sif->msgq_enqueue(aqm);
	  usleep(100000);
	} catch (Exception &e) {
	  e.print_trace();
	  return;
	}
      }

      if ( argp->num_items() > 0 ) {
	std::string sks = "";
	const std::vector< const char * > & items = argp->items();

	std::vector< const char * >::const_iterator i = items.begin();
	sks = *i;
	++i;
	for (; i != items.end(); ++i) {
	  sks += " ";
	  sks += *i;
	}

	SkillerInterface::ExecSkillMessage *esm = new SkillerInterface::ExecSkillMessage(sks.c_str());
	sif->msgq_enqueue(esm);

	usleep(100000);
	exit();
      } else {
	char *line = NULL;

	line = readline(prompt);
	if ( line ) {
	  if (strcmp(line, "") != 0) {
	    printf("Executing: %s\n", line);
	    add_history(line);

	    SkillerInterface::ExecSkillMessage *esm = new SkillerInterface::ExecSkillMessage(line);
	    sif->msgq_enqueue(esm);
	  }
	} else {
	  if ( ! connection_died_recently ) {
	    exit();
	  }
	}
      }
    } else {
      if ( connection_died_recently ) {
	connection_died_recently = false;
	printf("Connection died\n");
	c->disconnect();
      }
      try {
	c->connect();
      } catch (Exception &e) {
	printf(".");
	fflush(stdout);
	sleep(1);
      }
    }
  }

  
  virtual void deregistered() throw()
  {
  }


  virtual void inbound_received(FawkesNetworkMessage *m) throw()
  {
  }


  virtual void connection_died() throw()
  {
    prompt = "-# ";

    rbb->close(sif);
    delete rbb;
    rbb = NULL;
    sif = NULL;

    connection_died_recently = true;

    //fprintf(stdin, "\n");
    //kill(SIGINT);
    rl_done = 1;
  }


  virtual void connection_established() throw()
  {
    printf("Connection established\n");
    just_connected = true;
    prompt = "+# ";
  }


 private:
  ArgumentParser *argp;
  FawkesNetworkClient *c;
  RemoteBlackBoard *rbb;
  SkillerInterface *sif;
  const char *prompt;
  bool just_connected;
  bool connection_died_recently;
};


/** Config tool main.
 * @param argc argument count
 * @param argv arguments
 */
int
main(int argc, char **argv)
{
  ArgumentParser argp(argc, argv, "h");

  if ( argp.has_arg("h") ) {
    print_usage(argv[0]);
    exit(0);
  }

  SkillShellThread sst(&argp);
  sst.start();
  sst.join();

  return 0;
}