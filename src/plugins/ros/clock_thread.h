/***************************************************************************
 *  clock_thread.h - Thread to publish clock to ROS
 *
 *  Created: Sun Jul 12 16:13:07 2015
 *  Copyright  2015  Tim Niemueller
 ****************************************************************************/

/*  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  Read the full text in the LICENSE.GPL file in the doc directory.
 */

#ifndef _PLUGINS_ROS_CLOCK_THREAD_H_
#define _PLUGINS_ROS_CLOCK_THREAD_H_

#include <aspect/blackboard.h>
#include <aspect/clock.h>
#include <aspect/configurable.h>
#include <aspect/logging.h>
#include <core/threading/thread.h>
#include <plugins/ros/aspect/ros.h>
#include <ros/node_handle.h>

namespace fawkes {
class TimeWait;
}

class RosClockThread : public fawkes::Thread,
                       public fawkes::LoggingAspect,
                       public fawkes::ConfigurableAspect,
                       public fawkes::BlackBoardAspect,
                       public fawkes::ClockAspect,
                       public fawkes::ROSAspect
{
public:
	RosClockThread();

	virtual void init();
	virtual void loop();
	virtual void finalize();

	/** Stub to see name in backtrace for easier debugging. @see Thread::run() */
protected:
	virtual void
	run()
	{
		Thread::run();
	}

private:
	void publish_clock();

private:
	ros::Publisher pub_;
	bool           set_local_;
	unsigned int   cfg_freq_;

	fawkes::TimeWait *time_wait_;
};

#endif
