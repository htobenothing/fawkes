
/***************************************************************************
 *  segment.h - Header of segmentation filter
 *
 *  Generated: Mon Jun 27 11:36:25 2005
 *  Copyright  2005  Tim Niemueller [www.niemueller.de]
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

#ifndef __FIREVISION_FILTER_SEGMENT_H_
#define __FIREVISION_FILTER_SEGMENT_H_

#include <filters/filter.h>
#include <fvutils/base/types.h>

class ColorModel;

class FilterSegment : public Filter
{

 public:

  FilterSegment(ColorModel *cm, color_t what);


  virtual void setSrcBuffer(unsigned char *buf,
			    ROI *roi,
			    orientation_t ori = ORI_HORIZONTAL,
			    unsigned int buffer_num = 0);

  virtual void setSrcBuffer(unsigned char *buf,
			    ROI *roi,
			    unsigned int buffer_num);

  virtual void setDstBuffer(unsigned char *buf,
			    ROI *roi,
			    orientation_t ori = ORI_HORIZONTAL);

  virtual void setOrientation(orientation_t ori);

  virtual void apply();

  virtual const char *  getName();

 private:
  unsigned char *src;
  unsigned char *dst;

  ROI           *src_roi;
  ROI           *dst_roi;

  ColorModel    *cm;
  color_t        what;

};

#endif
