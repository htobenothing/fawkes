
/***************************************************************************
 *  clips_inifin.h - Fawkes CLIPSAspect initializer/finalizer
 *
 *  Created: Sat Jun 16 14:34:01 2012
 *  Copyright  2006-2012  Tim Niemueller [www.niemueller.de]
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

#ifndef _PLUGINS_CLIPS_ASPECT_CLIPS_INIFIN_H_
#define _PLUGINS_CLIPS_ASPECT_CLIPS_INIFIN_H_

#include <aspect/inifins/inifin.h>
#include <plugins/clips/aspect/clips.h>

#include <map>
#include <string>

namespace fawkes {

class Logger;
class CLIPSEnvManager;

class CLIPSAspectIniFin : public AspectIniFin
{
public:
	CLIPSAspectIniFin();
	~CLIPSAspectIniFin();

	virtual void init(Thread *thread);
	virtual void finalize(Thread *thread);

	void set_manager(LockPtr<CLIPSEnvManager> &clips_env_mgr);

private:
	LockPtr<CLIPSEnvManager> clips_env_mgr_;
};

} // end namespace fawkes

#endif
