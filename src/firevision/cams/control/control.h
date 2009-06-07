
/***************************************************************************
 *  cameracontrol.h - Base class for camera controllers
 *
 *  Created: Tue Jun 07 15:45:57 2005
 *  Copyright  2005-2009  Tim Niemueller [www.niemueller.de]
 *             2009       Tobias Kellner
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

#ifndef __FIREVISION_CAMERACONTROL_H_
#define __FIREVISION_CAMERACONTROL_H_

class CameraControl
{
 public:
  /** Control Type ID */
  typedef enum {
    CTRL_TYPE_COLOR,	/**< CameraControlColor */
    CTRL_TYPE_IMAGE,	/**< CameraControlImage */
    CTRL_TYPE_PANTILT,	/**< CameraControlPanTilt */
    CTRL_TYPE_FOCUS,	/**< CameraControlFocus */
    CTRL_TYPE_ZOOM,	/**< CameraControlZoom */
    CTRL_TYPE_EFFECT	/**< CameraControlEffect */
  } TypeID;

  virtual ~CameraControl();
};

#endif