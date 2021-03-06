
/***************************************************************************
 *  geodesic_dilation.cpp - implementation of morphological geodesic dilation
 *
 *  Created: Sat Jun 10 16:21:30 2006
 *  Copyright  2005-2007  Tim Niemueller [www.niemueller.de]
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

#include <core/exception.h>
#include <fvfilters/min.h>
#include <fvfilters/morphology/dilation.h>
#include <fvfilters/morphology/geodesic_dilation.h>
#include <fvfilters/morphology/segenerator.h>
#include <fvutils/base/roi.h>
#include <fvutils/color/colorspaces.h>
#include <fvutils/statistical/imagediff.h>

#include <cstdlib>
#include <cstring>

namespace firevision {

/** Marker */
const unsigned int FilterGeodesicDilation::MARKER = 0;
/** Mask */
const unsigned int FilterGeodesicDilation::MASK = 1;

#define ERROR(m)                                          \
	{                                                       \
		fawkes::Exception e("FilterGeodesicDilation failed"); \
		e.append("Function: %s", __FUNCTION__);               \
		e.append("Message:  %s", m);                          \
		throw e;                                              \
	}

/** @class FilterGeodesicDilation <fvfilters/morphology/geodesic_dilation.h>
 * Morphological geodesic dilation.
 * @author Tim Niemueller
 */

/** Constructor.
 * @param se_size Structuring element size.
 */
FilterGeodesicDilation::FilterGeodesicDilation(unsigned int se_size)
: MorphologicalFilter("Morphological Geodesic Dilation", 2)
{
	this->se_size = (se_size > 0) ? se_size : 1;
	iterations    = 0;

	dilate = new FilterDilation();
	min    = new FilterMin();
	diff   = new ImageDiff();

	isotropic_se = SEGenerator::square(this->se_size, this->se_size);

	dilate->set_structuring_element(isotropic_se, se_size, se_size, se_size / 2, se_size / 2);

	src[MARKER] = src[MASK] = dst = NULL;
	src_roi[MARKER] = src_roi[MASK] = dst_roi = NULL;
}

/** Destructor. */
FilterGeodesicDilation::~FilterGeodesicDilation()
{
	delete dilate;
	delete min;
	delete diff;
	free(isotropic_se);
}

void
FilterGeodesicDilation::apply()
{
	if (dst == NULL)
		ERROR("dst == NULL");
	if (src[MASK] == NULL)
		ERROR("src[MASK] == NULL");
	if (src[MARKER] == NULL)
		ERROR("src[MARKER] == NULL");
	if (*(src_roi[MASK]) != *(src_roi[MARKER]))
		ERROR("marker and mask ROI differ");

	unsigned char *tmp =
	  (unsigned char *)malloc(colorspace_buffer_size(YUV422_PLANAR,
	                                                 src_roi[MARKER]->image_width,
	                                                 src_roi[MARKER]->image_height));
	memcpy(tmp,
	       src[MARKER],
	       colorspace_buffer_size(YUV422_PLANAR,
	                              src_roi[MARKER]->image_width,
	                              src_roi[MARKER]->image_height));

	diff->setBufferA(tmp, src_roi[MARKER]->image_width, src_roi[MARKER]->image_height);
	diff->setBufferB(dst, dst_roi->image_width, dst_roi->image_height);

	dilate->set_src_buffer(tmp, src_roi[MARKER]);

	min->set_src_buffer(src[MASK], src_roi[MASK], 0);
	min->set_src_buffer(tmp, src_roi[MARKER], 1);
	min->set_dst_buffer(tmp, src_roi[MARKER]);

	iterations = 0;
	do {
		memcpy(dst,
		       tmp,
		       colorspace_buffer_size(YUV422_PLANAR, dst_roi->image_width, dst_roi->image_height));
		dilate->apply();
		min->apply();
	} while (diff->different() && (++iterations < 255));

	// std::cout << i << " iterations done for geodesic dilation" << std::endl;

	free(tmp);
}

/** Get the number of iterations.
 * @return the number of iterations that were necessary to get a stable result in the
 * last call to apply().
 */
unsigned int
FilterGeodesicDilation::num_iterations()
{
	return iterations;
}

} // end namespace firevision
