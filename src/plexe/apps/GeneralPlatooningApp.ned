//
// Copyright (C) 2012-2021 Michele Segata <segata@ccs-labs.org>
// Copyright (C) 2018-2021 Julian Heinovski <julian.heinovski@ccs-labs.org>
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

package org.car2x.plexe.apps;

import org.car2x.plexe.apps.BaseApp;

simple GeneralPlatooningApp like BaseApp {

parameters:

    // implementation of the maneuver to be used
    string maneuver;
    double soglia = default(3);
    double tAttesaRisposta = default(0.01);
    double tRadar = default(0.5);
    double tNoRadar = default(10);

    int headerLength @unit("bit") = default(0 bit);
    @display("i=block/app2");
    @class(plexe::GeneralPlatooningApp);

gates:
    input lowerLayerIn;
    output lowerLayerOut;
    input lowerControlIn;
    output lowerControlOut;
}
