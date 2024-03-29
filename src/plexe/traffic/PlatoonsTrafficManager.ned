//
// Copyright (C) 2014-2021 Michele Segata <segata@ccs-labs.org>
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

package org.car2x.plexe.traffic;

import org.car2x.plexe.mobility.TraCIBaseTrafficManager;

simple PlatoonsTrafficManager like TraCIBaseTrafficManager {

    parameters:
        //platoon insertion time
        double platoonInsertTime @unit("s");
        //platoon insertion speed
        double platoonInsertSpeed @unit("kmph");
        //how many vehicles to inject
        int nCars;
        //platoon size
        int platoonSize;
        //number of lanes
        int nLanes;
        
        int traffico = default(0);
        
        //sumo vehicle type for platooning cars
        string platooningVType;
        string camionVType;
        //insert distance and headway. distance is computed as:
        //dist = insertHeadway * insertSpeed + insertDistance
        double platoonInsertDistance @unit("m") = default(5m);
        double platoonInsertHeadway @unit("s") = default(0s);
        double platoonLeaderHeadway @unit("s") = default(1.2s);
        double platoonAdditionalDistance @unit("m") = default(0m);
        @class(plexe::PlatoonsTrafficManager);
}
