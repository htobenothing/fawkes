
------------------------------------------------------------------------
--  math.lua - Lua math helpers
--
--  Created: Wed Jul 16 20:02:24 2008
--  Copyright  2008  Tim Niemueller [www.niemueller.de]
--
--  $Id: fsm.lua 1219 2008-07-14 07:41:44Z tim $
--
------------------------------------------------------------------------

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

require("fawkes.modinit")

--- This module provides a generic finite state machine (FSM).
-- @author Tim Niemueller
module(..., fawkes.modinit.register_all)


--- Transform 2D cartesian coordinates to 2D polar coordinates
-- @param cart_x X component of the cartesian coordinate
-- @param cart_y Y component of the cartesian coordinate
-- @return two floats, angle and distance (in that order) of the polar coordinate
function math.cart2polar2d(cart_x, cart_y)
   return  math.atan2(cart_y, cart_x), math.sqrt(cart_x * cart_x + cart_y * cart_y);
end


--- Transform 2D polar coordinates to 2D cartesian coordinates
-- @param polar_phi angle of the polar coordinate
-- @param polar_dist distance of the polar coordinate
-- @return two floats, x and y components of the cartesian coordinate (in that order)
function math.polar2cart2d(polar_phi, polar_dist)
   return polar_dist * math.cos(polar_phi), polar_dist * math.sin(polar_phi);
end


function math.round(r)
  if r >= 0 then
    local rf = math.floor(r)
    if r - rf >= 0.5 then
      return rf + 1
    else
      return rf
    end
  else
    local rf = math.ceil(r)
    if r - rf < -0.5 then
      return rf + 1
    else
      return rf
    end
  end
end

function math.normalize_mirror_rad(angle_rad)
  if angle_rad < -math.pi or angle_rad > math.pi then
    return angle_rad - 2 * math.pi * math.round(angle_rad / (2 * math.pi))
  else
    return angle_rad
  end
end