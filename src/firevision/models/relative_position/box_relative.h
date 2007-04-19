
/***************************************************************************
 *  box_relative.h - A simple implementation of a relative position model
 *                   for boxes
 *
 *  Generated: Thu Jun 08 19:21:35 2006
 *  Copyright  2005-2006  Tim Niemueller [www.niemueller.de]
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

#ifndef __FIREVISION_MODELS_RELPOS_BOX_H_
#define __FIREVISION_MODELS_RELPOS_BOX_H_

#include <models/relative_position/relativepositionmodel.h>

// include <utils/kalman_filter/ckalman_filter_2dim.h>

class BoxRelative : public RelativePositionModel
{
 public:
  BoxRelative(unsigned int image_width, unsigned int image_height,
	      float camera_height,
	      float camera_offset_x, float camera_offset_y,
	      float camera_ori,
	      float horizontal_angle, float vertical_angle
	      );

  virtual const char *	getName() const;
  virtual void		setCenter(float x, float y);
  virtual void		setCenter(const center_in_roi_t& c);
  virtual void          setRadius(float r);

  virtual void		setPanTilt(float pan = 0.0f, float tilt = 0.0f);
  virtual void          getPanTilt(float *pan, float *tilt) const;

  virtual void          setHorizontalAngle(float angle_deg);
  virtual void          setVerticalAngle(float angle_deg);

  virtual float		getDistance() const;

  virtual float		getX() const;
  virtual float		getY() const;

  virtual float		getBearing() const;
  virtual float		getSlope() const;

  virtual void          calc();
  virtual void          calc_unfiltered();
  virtual void          reset();

  virtual bool          isPosValid() const;

private:
  float                 DEFAULT_X_VARIANCE;
  float                 DEFAULT_Y_VARIANCE;

  float	                pan_rad_per_pixel;
  float	                tilt_rad_per_pixel;

  center_in_roi_t       center;
  float			pan;
  float			tilt;

  float                 horizontal_angle;
  float                 vertical_angle;

  unsigned int          image_width;
  unsigned int          image_height;

  float                 camera_height;
  float                 camera_offset_x;
  float                 camera_offset_y;
  float                 camera_orientation;

  float                 last_x;
  float                 last_y;
  bool                  last_available;
  float                 box_x;
  float                 box_y;
  float                 bearing;
  float                 slope;
  float                 distance_box_motor;
  float                 distance_box_cam;

  /*
  float                 var_proc_x;
  float                 var_proc_y;
  float                 var_meas_x;
  float                 var_meas_y;
  kalmanFilter2Dim     *kalman_filter;

  void                  applyKalmanFilter();
  */
};

#endif // __FIREVISION_MODELS_RELPOS_BOX_H_

