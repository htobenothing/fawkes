
/***************************************************************************
 *  navigator_net_thread.cpp - Navigator Plugin Network Thread
 *
 *  Generated: Thu May 31 20:38:50 2007
 *  Copyright  2007  Martin Liebenberg
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <plugins/navigator/navigator_net_thread.h>
#include <plugins/navigator/libnavi/navigator_messages.h>
#include <plugins/navigator/libnavi/npoint.h>
#include <plugins/navigator/libnavi/nline.h>
#include <netcomm/fawkes/component_ids.h>
#include <plugins/navigator/joystick_control.h>
#include <interfaces/motor.h>
#include <interfaces/navigator.h>
#include <interfaces/kicker.h>
#include <plugins/navigator/navigator_thread.h>

#include <netcomm/socket/datagram.h>

#include <cstdlib>
#include <unistd.h>
#include <algorithm>
#include <list>
#include <arpa/inet.h>

/** @class NavigatorNetworkThread plugins/navigator/navigator_net_thread.h
 * Network thread of the navigator plugin.
 * 
 * @author Martin Liebenberg
 */
/** @var NavigatorNetworkThread::motor_interface
 *   The interface to tell the motor thread what is allowed to control the motors.
 */

/** Constructor. 
 * @param navigator_thread the navigator thread to get path informations directly from
 * the navigator
 */
NavigatorNetworkThread::NavigatorNetworkThread(NavigatorThread *navigator_thread)
  : Thread("NavigatorNetworkThread", Thread::OPMODE_WAITFORWAKEUP),
    BlockedTimingAspect(BlockedTimingAspect::WAKEUP_HOOK_POST_LOOP),
    FawkesNetworkHandler(FAWKES_CID_NAVIGATOR_PLUGIN)
{
  this->navigator_thread = navigator_thread;
  connected_control_client = 0;
  logger_modulo_counter = 0;

  last_motor_control_thread_id = 0;
  last_motor_control_thread_name = NULL;

  blocked_by_udp = false;

  kicker_interface = NULL;
  motor_interface = NULL;
  navigator_interface = NULL;
  joystick_control = NULL;
}


/** Destructor. */
NavigatorNetworkThread::~NavigatorNetworkThread()
{
  if ( last_motor_control_thread_name != NULL ) {
    free( last_motor_control_thread_name );
  }
}


void
NavigatorNetworkThread::finalize()
{
  logger->log_info("NavigatorNetworkThread", "Finalizing thread %s", name());

  logger->log_info("NavigatorNetworkThread", "Removing this thread from list of Fawkes network hub handlers");
  fnethub->remove_handler( this );
  
  delete joystick_control;
  
  try 
    {
      interface_manager->close(motor_interface);
      interface_manager->close(navigator_interface);
      interface_manager->close(kicker_interface);
    }
  catch (Exception& e)
    {
      logger->log_error("NavigatorNetworkThread", "Closing interface failed!");
      logger->log_error("NavigatorNetworkThread", e);
    }
}

void
NavigatorNetworkThread::init()
{

  try 
    {
      motor_interface = interface_manager->open_for_reading<MotorInterface>("Motor");
    }
  catch (Exception& e)
    {
      e.append("%s initialization failed, could not open motor interface for reading", name());
      logger->log_error("NavigatorNetworkThread", "Opening interface for reading failed!");
      logger->log_error("NavigatorNetworkThread", e);
      throw;
    }
    
  try 
    {
      kicker_interface = interface_manager->open_for_reading<KickerInterface>("Kicker");
    }
  catch (Exception& e)
    {
      e.append("%s initialization failed, could not open kicker interface for reading", name());
      logger->log_error("NavigatorNetworkThread", "Opening interface for reading failed!");
      logger->log_error("NavigatorNetworkThread", e);
      throw;
    }
    
  try 
    {
      navigator_interface = interface_manager->open_for_reading<NavigatorInterface>("Navigator");
    }
  catch (Exception& e)
    {
      e.append("%s initialization failed, could not open navigator interface for reading", name());
      logger->log_error("NavigatorNetworkThread", "Opening interface for reading failed!");
      logger->log_error("NavigatorNetworkThread", e);
      throw;
    }

  try
    {
      joystick_control = new JoystickControl(motor_interface, kicker_interface, logger, config, clock);
    }
  catch (Exception &e) {
    e.append("NavigatorNetworkThread could not initialize JoystickControl");
    throw;
  }


  unsigned int port = 0;
  try {
    port = config->get_uint("navigator", "/network/joystick_port");
    if ( port > 0xFFFF ) {
      throw Exception("Navigator joystick port out of range");
    }
  } catch (Exception &e) {
    throw;
  }

  try {
    datagram_socket = new DatagramSocket(0.1);
    datagram_socket->bind(port);
  } catch (Exception &e) {
    e.append("Could not create navigator joystick socket");
    throw e;
  }

  // logger->log_debug("NavigatorNetworkThread", "Adding network handler");
  fnethub->add_handler( this );  
}

void
NavigatorNetworkThread::process_udp_message(void *buf, size_t buflen)
{
  fawkes_message_t *fmsg = (fawkes_message_t *)buf;
  if ( ntohs(fmsg->header.cid) != FAWKES_CID_NAVIGATOR_PLUGIN ) {
    logger->log_warn(name(), "Received message for other component: %u (ignored)", ntohs(fmsg->header.cid));
    return;
  }

  FawkesNetworkMessage *m = new FawkesNetworkMessage(*fmsg);
  inbound_queue.push_locked(m);
}


void
NavigatorNetworkThread::process_network_message(FawkesNetworkMessage *msg)
{
  if(msg->msgid() == NAVIGATOR_MSGTYPE_JOYSTICK )
    {
      if (connected_control_client == msg->clid()) {
	// logger->log_debug(name(), "Enqueuing joystick message");
	navigator_joystick_message_t *u = (navigator_joystick_message_t *)msg->payload();
	
	if ( motor_interface->controller_thread_id() == thread_id() ) { 
	  joystick_control->enqueueCommand(u->forward, u->sideward, u->rotation, u->speed);
	} else {
	  logger->log_warn("NavigatorNetworkThread", "Thread %s (%lu) stole our motor control!",
			   motor_interface->controller_thread_name(),
			   motor_interface->controller_thread_id());
	}
      } else {
	logger->log_warn("NavigatorNetworkThread", "Client %u sent joystick message while not subscribed", msg->clid());
      }
    }
  else if(msg->msgid() == NAVIGATOR_MSGTYPE_SUBSCRIBE)
    {
      navigator_subscribe_message_t *u = (navigator_subscribe_message_t *)msg->payload();
        
      if(u->sub_type_points_and_lines)
        {
          connected_points_and_lines_clients.push_back_locked(msg->clid());
        }
      else if(u->sub_type_control && connected_control_client == 0 && ! blocked_by_udp)
        {
          connected_control_client = msg->clid();
          motor_interface->read();
          last_motor_control_thread_id = motor_interface->controller_thread_id();
	  // this needs to be a strncpy, but currently the interfaces do not provide enough
	  // functionality
	  if ( last_motor_control_thread_name != NULL ) {
	    free(last_motor_control_thread_name);
	  }
	  last_motor_control_thread_name = strdup(motor_interface->controller_thread_name());

          MotorInterface::AquireControlMessage* msg = new  MotorInterface::AquireControlMessage();
	  msg->set_thread_id(thread_id());
	  msg->set_thread_name(name());
          motor_interface->msgq_enqueue(msg); 
       
          logger->log_debug("NavigatorNetworkThread", "Client %u subscribed as controller", connected_control_client);
        }
      else if(u->sub_type_control && (connected_control_client != 0 || blocked_by_udp) )
        {
          logger->log_warn("NavigatorNetworkThread", "Client %u tried to subscribe but there is already another subscriber (%s)",
			   msg->clid(), blocked_by_udp ? "UDP" : "Fawkes");
          fnethub->send(msg->clid(), 
                        FAWKES_CID_NAVIGATOR_PLUGIN, 
                        NAVIGATOR_MSGTYPE_CONTROL_SUBERR);
        }
    }
  else if(msg->msgid() == NAVIGATOR_MSGTYPE_UNSUBSCRIBE)
    {
      if ( msg->clid() == connected_control_client ) {
	logger->log_error("NavigatorNetworkThread", "Message of type unsubscribe message received");
	navigator_unsubscribe_message_t *u = (navigator_unsubscribe_message_t *)msg->payload();
        
	if(u->unsub_type_points_and_lines)
	  {
	    connected_points_and_lines_clients.remove_locked(msg->clid());
	  }
	else if(u->unsub_type_control)
	  {
	    connected_control_client = 0;
	    MotorInterface::AquireControlMessage* msg = new  MotorInterface::AquireControlMessage();
	    msg->set_thread_id(last_motor_control_thread_id);
	    msg->set_thread_name(last_motor_control_thread_name);
	    motor_interface->msgq_enqueue(msg); 
	  }
      } else {
	logger->log_error(name(), "Client %u tried to unsubscribe while not subscribed");
      }
    }
  else if(msg->msgid() == NAVIGATOR_MSGTYPE_TARGET
          && msg->clid() == connected_control_client)
    {
      navigator_target_message_t *u = (navigator_target_message_t *)msg->payload();
      // logger->log_error("NavigatorNetworkThread", "payload: %f, %f", u->x, u->y);
      NavigatorInterface::TargetMessage* msg = new  NavigatorInterface::TargetMessage();
  
      msg->set_x(u->x);
      msg->set_y(u->y);
    
      navigator_interface->msgq_enqueue(msg); 
    }
  else if(msg->msgid() == NAVIGATOR_MSGTYPE_VELOCITY 
          && msg->clid() == connected_control_client)
    {
      logger->log_error("NavigatorNetworkThread", "Message of type velocity message received");
      navigator_velocity_message_t *u = (navigator_velocity_message_t *)msg->payload();
      logger->log_error("NavigatorNetworkThread", "payload: %f", u->value);
      NavigatorInterface::VelocityMessage* msg = new  NavigatorInterface::VelocityMessage();
      msg->set_velocity(u->value);
      navigator_interface->msgq_enqueue(msg); 
    }
  else if(msg->msgid() == NAVIGATOR_MSGTYPE_KICK 
          && msg->clid() == connected_control_client)
    {
      navigator_kick_message_t *u = (navigator_kick_message_t *)msg->payload();
    //  logger->log_info("NavigatorNetworkThread", "kick message: %i, %i, %i", u->left, u->center, u->right);
      joystick_control->enqueueKick(u->left, u->center, u->right);
    }
  else
    {
      logger->log_error("NavigatorNetworkThread", "Message of invalid type received");
    }

}


void
NavigatorNetworkThread::loop()
{
  motor_interface->read();

  inbound_queue.lock();
  while ( ! inbound_queue.empty() ) {
    FawkesNetworkMessage *msg = inbound_queue.front();
    process_network_message(msg);
    msg->unref();
    inbound_queue.pop();
  }
  inbound_queue.unlock();

  /*
    int rpm1 = motor_interface->getRPM1();
    int rpm2 = motor_interface->getRPM2();
    int rpm3 = motor_interface->getRPM3();
  */
  
  for(std::list<unsigned int>::iterator iterator = connected_points_and_lines_clients.begin(); 
      iterator != connected_points_and_lines_clients.end(); 
      iterator++ ) 
    {
      //send points
      std::list<NPoint *> *points = navigator_thread->get_surface_points();

      NavigatorNodesListMessage *nodes_list_msg = new NavigatorNodesListMessage(points);
      fnethub->send(*iterator, FAWKES_CID_NAVIGATOR_PLUGIN, NAVIGATOR_MSGTYPE_NODES, nodes_list_msg);
      //   logger->log_info("NavigatorNetworkThread", "send points; connected clients: %i", connected_points_and_lines_clients.size());
    
      //send lines
      std::list<NLine *> *lines = navigator_thread->get_surface_lines();
   
      NavigatorLinesListMessage *lines_list_msg = new NavigatorLinesListMessage(lines);
      fnethub->send(*iterator, FAWKES_CID_NAVIGATOR_PLUGIN, NAVIGATOR_MSGTYPE_LINES, lines_list_msg);
      logger->log_info("NavigatorNetworkThread", "send lines; connected clients: %i", connected_points_and_lines_clients.size());
   
   
   
      //delete points
      for(std::list<NPoint *>::iterator point_iterator = points->begin(); 
          point_iterator != points->end(); 
          point_iterator++) 
        {
          delete *point_iterator;
        }
      
      points->clear();
     
      //delete lines
      for(std::list<NLine *>::iterator line_iterator = lines->begin(); 
          line_iterator != lines->end(); 
          line_iterator++) 
        {
          delete *line_iterator;
        }
      lines->clear();
    }

  if ( datagram_socket->available() ) {
    // there is data that can be read from the socket
    udp_client_addrlen = sizeof(udp_client_addr);
    try {
      unsigned int i = 0;
      struct sockaddr_in addr;
      socklen_t addr_len = sizeof(addr);
      size_t buflen = 0;
      while ( (++i < 10) &&
	      ((buflen = datagram_socket->recv(udp_tmpbuf, sizeof(udp_tmpbuf),
					       (struct sockaddr *)&addr,
					       &addr_len)) > 0) ) {
	if ( connected_control_client == 0 ) {
	  if ( blocked_by_udp) {
	    // already received UDP traffic, make sure it came from the very same host!
	    if ( addr.sin_addr.s_addr == udp_client_addr.sin_addr.s_addr ) {
	      // accepted, process
	      process_udp_message(udp_tmpbuf, buflen);
	    } else {
	      char tmp[INET_ADDRSTRLEN];
	      inet_ntop(AF_INET, &(addr.sin_addr), tmp, INET_ADDRSTRLEN);
	      logger->log_warn(name(), "Received UDP message from non-subscribed sender (%s)", tmp);
	    }
	  }
	  blocked_by_udp = true;
	  
	} // else ignored, there is a subscriber on the Fawkes line
      }
    } catch (Exception &e) {
      logger->log_warn(name(), e);
    }
  }

  // logger->log_info("NavigatorNetworkThread", "send; connected clients: %i", connected_points_and_lines_clients.size());

}


void
NavigatorNetworkThread::handle_network_message(FawkesNetworkMessage *msg)
{
  //logger->log_info("NavigatorNetworkThread", "Message of type %i received", msg->msgid());
  msg->ref();
  inbound_queue.push_locked(msg);
}


/** Client connected.
 * Ignored.
 * @param clid client ID
 */
void
NavigatorNetworkThread::client_connected(unsigned int clid)
{
  // logger->log_info("NavigatorNetworkThread", "Client %u connected", clid);
}


/** Client disconnected.
 * If the client was a subscriber it is removed.
 * @param clid client ID
 */
void
NavigatorNetworkThread::client_disconnected(unsigned int clid)
{
  // logger->log_info("NavigatorNetworkThread", "Client %u disconnected", clid);
  
  connected_points_and_lines_clients.lock();
  std::list<unsigned int>::iterator result;
  result = find( connected_points_and_lines_clients.begin(), 
                 connected_points_and_lines_clients.end()  , clid );
                        
  if(result != connected_points_and_lines_clients.end())
    {
      connected_points_and_lines_clients.remove(clid);
    }
  connected_points_and_lines_clients.unlock();
  
  if(connected_control_client == clid)
    {
      connected_control_client = 0;
      logger->log_debug("NavigatorNetworkThread", "Client %u unsubscribed as controller", clid);

      MotorInterface::AquireControlMessage* msg = new  MotorInterface::AquireControlMessage();
      msg->set_thread_id(last_motor_control_thread_id);
      msg->set_thread_name(last_motor_control_thread_name);
      motor_interface->msgq_enqueue(msg); 
    }
}

/** Process all network messages that have been received.
 * Nothing to do for example network thread.
 */
void
NavigatorNetworkThread::process_after_loop()
{
}
