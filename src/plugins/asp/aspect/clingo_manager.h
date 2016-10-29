/***************************************************************************
 *  clingo_manager.h - Clingo manager aspect for Fawkes
 *
 *  Created: Sat Oct 29 11:30:07 2016
 *  Copyright  2016 Björn Schäpers
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

#ifndef __PLUGINS_ASP_ASPECT_CLINGO_MANAGER_H_
#define __PLUGINS_ASP_ASPECT_CLINGO_MANAGER_H_

#include <aspect/aspect.h>
#include <core/utils/lockptr.h>

#include "clingo_control_manager.h"

namespace fawkes {
#if 0 /* just to make Emacs auto-indent happy */
}
#endif

class ClingoManagerAspect : public virtual Aspect
{
	protected:
	LockPtr<ClingoControlManager> ClingoCtrlMgr;

	public:
	ClingoManagerAspect(void);
	virtual ~ClingoManagerAspect(void);

	friend class ClingoManagerAspectIniFin;
};

} // end namespace fawkes

#endif
