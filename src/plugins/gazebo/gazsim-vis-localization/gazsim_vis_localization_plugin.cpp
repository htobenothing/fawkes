/***************************************************************************
 *  gazsim_vis_localization_plugin.cpp - Plugin visualizes the localization
 *
 *  Created: Tue Sep 17 15:34:44 2013
 *  Copyright  2013 Frederik Zwilling
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

#include "gazsim_vis_localization_thread.h"

#include <core/plugin.h>

using namespace fawkes;

/** Plugin visualizes the localization
 *
 *
 * @author Frederik Zwilling
 */
class GazsimVisLocalizationPlugin : public fawkes::Plugin
{
public:
	/** Constructor.
   * @param config Fawkes configuration
   */
	explicit GazsimVisLocalizationPlugin(Configuration *config) : Plugin(config)
	{
		thread_list.push_back(new VisLocalizationThread());
	}
};

PLUGIN_DESCRIPTION("Visualization of the Localization in Gazebo")
EXPORT_PLUGIN(GazsimVisLocalizationPlugin)
