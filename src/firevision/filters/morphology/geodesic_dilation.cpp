
/***************************************************************************
 *  geodesic_dilation.cpp - implementation of morphological geodesic dilation
 *
 *  Generated: Sat Jun 10 16:21:30 2006
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

#include <core/exception.h>

#include <filters/morphology/geodesic_dilation.h>
#include <filters/morphology/segenerator.h>
#include <filters/morphology/dilation.h>
#include <filters/min.h>

#include <fvutils/statistical/imagediff.h>
#include <fvutils/color/colorspaces.h>
#include <fvutils/base/roi.h>

/** Marker */
const unsigned int FilterGeodesicDilation::MARKER = 0;
/** Mask */
const unsigned int FilterGeodesicDilation::MASK   = 1;

#define ERROR(m) {							\
    Exception e("FilterGeodesicDilation failed");			\
    e.append("Function: %s", __FUNCTION__);				\
    e.append("Message:  %s", m);					\
    throw e;								\
  }


/** @class FilterGeodesicDilation <filters/morphology/geodesic_dilation.h>
 * Morphological geodesic dilation.
 * @author Tim Niemueller
 */

/** Constructor.
 * @param se_size Structuring element size.
 */
FilterGeodesicDilation::FilterGeodesicDilation(unsigned int se_size)
{
  this->se_size  = (se_size > 0) ? se_size : 1;
  iterations = 0;

  dilate = new FilterDilation();
  min    = new FilterMin();
  diff   = new ImageDiff();

  isotropic_se = SEGenerator::square(this->se_size, this->se_size);

  dilate->setStructuringElement( isotropic_se, se_size, se_size, se_size / 2, se_size / 2 );

  src[MARKER] = src[MASK] = dst = NULL;
  src_roi[MARKER] = src_roi[MASK] = dst_roi = NULL;
}


/** Destructor. */
FilterGeodesicDilation::~FilterGeodesicDilation()
{
  delete dilate;
  delete min;
  delete diff;
  free( isotropic_se );
}


void
FilterGeodesicDilation::setSrcBuffer(unsigned char *buf, ROI *roi,
				    orientation_t ori, unsigned int buffer_num)
{
  setSrcBuffer(buf, roi, buffer_num);
}


void
FilterGeodesicDilation::setSrcBuffer(unsigned char *buf, ROI *roi, unsigned int buffer_num)
{
  if ( buffer_num >= 2 ) ERROR("Invalid buffer number");

  src[buffer_num] = buf;
  src_roi[buffer_num] = roi;
}


void
FilterGeodesicDilation::setDstBuffer(unsigned char *buf, ROI *roi, orientation_t ori)
{
  dst = buf;
  dst_roi = roi;
}


void
FilterGeodesicDilation::setStructuringElement(unsigned char *se,
					     unsigned int se_width, unsigned int se_height,
					     unsigned int se_anchor_x, unsigned int se_anchor_y)
{
  // We don't care, only use a squared isotropic element for geodesic reconstruction
}


void
FilterGeodesicDilation::setOrientation(orientation_t ori)
{
}


const char *
FilterGeodesicDilation::getName()
{
  return "FilterGeodesicDilation";
}


void
FilterGeodesicDilation::apply()
{
  if ( dst == NULL ) ERROR("dst == NULL");
  if ( src[MASK] == NULL ) ERROR("src[MASK] == NULL");
  if ( src[MARKER] == NULL ) ERROR("src[MARKER] == NULL");
  if ( *(src_roi[MASK]) != *(src_roi[MARKER]) ) ERROR("marker and mask ROI differ");

  unsigned char *tmp = (unsigned char *)malloc(colorspace_buffer_size(YUV422_PLANAR, src_roi[MARKER]->image_width, src_roi[MARKER]->image_height) );
  memcpy( tmp, src[MARKER], colorspace_buffer_size(YUV422_PLANAR, src_roi[MARKER]->image_width, src_roi[MARKER]->image_height) );

  diff->setBufferA( tmp, src_roi[MARKER]->image_width, src_roi[MARKER]->image_height );
  diff->setBufferB( dst, dst_roi->image_width, dst_roi->image_height );

  dilate->setSrcBuffer( tmp, src_roi[MARKER] );

  min->setSrcBuffer( src[MASK], src_roi[MASK], 0 );
  min->setSrcBuffer( tmp, src_roi[MARKER], 1 );
  min->setDstBuffer( tmp, src_roi[MARKER] );


  iterations = 0;
  do {
    memcpy(dst, tmp, colorspace_buffer_size(YUV422_PLANAR, dst_roi->image_width, dst_roi->image_height) );
    dilate->apply();
    min->apply();
  } while (diff->different() && ( ++iterations < 255) );

  // std::cout << i << " iterations done for geodesic dilation" << std::endl;

  free( tmp );

}


/** Get the number of iterations.
 * @return the number of iterations that were necessary to get a stable result in the
 * last call to apply().
 */
unsigned int
FilterGeodesicDilation::getNumIterations()
{
  return iterations;
}
