
/***************************************************************************
 *  beams.h - Scanline model implementation: beams
 *
 *  Created: Tue Apr 17 20:59:58 2007
 *  Copyright  2005-2007  Tim Niemueller [www.niemueller.de]
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

#ifndef __FIREVISION_SCANLINE_BEAMS_H_
#define __FIREVISION_SCANLINE_BEAMS_H_

#include <models/scanlines/scanlinemodel.h>
#include <fvutils/types.h>

#include <vector>

class ScanlineBeams : public ScanlineModel
{

 public:

  ScanlineBeams(unsigned int image_width, unsigned int image_height,
		unsigned int start_x, unsigned int start_y,
		unsigned int stop_y, unsigned int offset_y,
                bool distribute_start_x,
		float angle_from, float angle_range, unsigned int num_beams);

  point_t  operator*();
  point_t* operator->();
  point_t* operator++();
  point_t* operator++(int); 

  bool          finished();
  void          reset();
  const char *  getName();
  unsigned int  getMargin();

  virtual void  setRobotPose(float x, float y, float ori) {}
  virtual void  setPanTilt(float pan, float tilt) {}

 private:
  void advance();


  bool _finished;

  std::vector<point_t> beam_current_pos;
  std::vector<point_t> beam_end_pos;

  unsigned int start_x;
  unsigned int start_y;
  float angle_from;
  float angle_range;
  unsigned int num_beams;
  unsigned int stop_y;
  unsigned int offset_y;
  unsigned int image_width;
  unsigned int image_height;
  bool distribute_start_x;

  point_t coord;
  point_t tmp_coord;

  unsigned int next_beam;
  unsigned int first_beam;
  unsigned int last_beam;
};

#endif
