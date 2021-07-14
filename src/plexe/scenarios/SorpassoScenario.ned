package org.car2x.plexe.scenarios;

import org.car2x.plexe.scenarios.BBaseScenario;

simple SorpassoScenario extends BBaseScenario {
    parameters:
        int nLanes;
        double tRadar = default(0.5);
        @display("i=block/app2");
        @class(plexe::SorpassoScenario);

}
