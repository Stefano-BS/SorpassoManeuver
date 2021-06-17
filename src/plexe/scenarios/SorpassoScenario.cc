#include "plexe/scenarios/SorpassoScenario.h"

using namespace veins;

namespace plexe {
    Define_Module(SorpassoScenario);

    void SorpassoScenario::initialize(int stage) {
        BaseScenario::initialize(stage);
        if (stage == 2) {
            //srand(time(0));
            appl = FindModule<GeneralPlatooningApp*>::findSubModule(getParentModule()); // get pointer to application
            if(!traciVehicle->getVType().compare("lento")) { //CAMION
                plexeTraciVehicle->changeLane(0x0, 0);
                plexeTraciVehicle->setFixedLane(0x0, 0);
                plexeTraciVehicle->setActiveController(false);
                plexeTraciVehicle->enableAutoLaneChanging(false);
                plexeTraciVehicle->setFixedAcceleration(0, 0);
                plexeTraciVehicle->setLaneChangeMode(0b001000000000);
            }
            else if (!traciVehicle->getVType().compare("veloce")) { //Altre auto
                plexeTraciVehicle->setFixedLane(0x1, 0);
                plexeTraciVehicle->setActiveController(false);
            }
            else { // Platoon
                leaderSpeed = par("leaderSpeed").doubleValue() / 3.6;
                if (positionHelper->isLeader()) {
                    plexeTraciVehicle->setCruiseControlDesiredSpeed(leaderSpeed);
                    controlloLentoLeader = new cMessage("");
                    scheduleAt(simTime()+1, controlloLentoLeader);
                }
                else plexeTraciVehicle->setCruiseControlDesiredSpeed(leaderSpeed + 5);
            }
        }
    }

    void SorpassoScenario::handleSelfMsg(cMessage* msg) {
        BaseScenario::handleSelfMsg(msg);
        if (msg == controlloLentoLeader && positionHelper->isLeader()) {
            double distance, relativeSpeed, deltav;
            plexeTraciVehicle->getRadarMeasurements(distance, relativeSpeed);
            deltav = traciVehicle->getSpeed() - leaderSpeed;
            if (-(deltav + relativeSpeed) / distance > 0.1) appl->iniziaSorpasso(positionHelper->getPlatoonId(),positionHelper->getPosition(),positionHelper->getPosition());
            scheduleAt(simTime()+1, controlloLentoLeader);
        }
    }

    SorpassoScenario::~SorpassoScenario() {
        cancelAndDelete(controlloLentoLeader);
        controlloLentoLeader = nullptr;
    }
}
