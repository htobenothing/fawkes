
----------------------------------------------------------------------------
--  throwin.lua - Mid-size league reactive agent: throw-in
--
--  Created: Tue Apr 15 15:43:00 2008
--  Copyright  2008  Tim Niemueller [www.niemueller.de]
--
--  $Id$
--
----------------------------------------------------------------------------

--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU Library General Public License for more details.
--
--  Read the full text in the LICENSE.GPL file in the doc directory.

require("midsize");
module("midsize.agent.throwin", midsize.module_init);

--- Execute code for GS_THROW_IN
-- @param role role
-- @param state_team active team
function exec(role, state_team)
   print_debug("Throw-in");
   if state_team == gamestate.TEAM_CYAN then
      print_debug("Throw-in team: cyan");
   else
      print_debug("Throw-in team: magenta");
   end
end