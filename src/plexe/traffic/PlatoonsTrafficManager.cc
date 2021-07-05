#include "PlatoonsTrafficManager.h"

namespace plexe {

Define_Module(PlatoonsTrafficManager);

void PlatoonsTrafficManager::initialize(int stage)
{

    TraCIBaseTrafficManager::initialize(stage);

    if (stage == 0) {

        nCars = par("nCars");
        platoonSize = par("platoonSize");
        nLanes = par("nLanes");
        platoonInsertTime = SimTime(par("platoonInsertTime").doubleValue());
        platoonInsertSpeed = par("platoonInsertSpeed").doubleValue();
        platoonInsertDistance = par("platoonInsertDistance").doubleValue();
        platoonInsertHeadway = par("platoonInsertHeadway").doubleValue();
        platoonLeaderHeadway = par("platoonLeaderHeadway").doubleValue();
        platoonAdditionalDistance = par("platoonAdditionalDistance").doubleValue();
        platooningVType = par("platooningVType").stdstringValue();
        
        traffico = par("traffico");
        platooningVType = par("platooningVType").stdstringValue();
        camionVType = par("camionVType").stdstringValue();
        
        insertPlatoonMessage = new cMessage("");
        scheduleAt(platoonInsertTime, insertPlatoonMessage);
    }
}

void PlatoonsTrafficManager::scenarioLoaded()
{
    automated.id = findVehicleTypeIndex(platooningVType);
    automated.lane = -1;
    automated.position = 0;
    automated.speed = platoonInsertSpeed / 3.6;
}

void PlatoonsTrafficManager::handleSelfMsg(cMessage* msg)
{

    TraCIBaseTrafficManager::handleSelfMsg(msg);

    if (msg == insertPlatoonMessage) {
        insertPlatoons();
        nuovaAutoMessage = new cMessage("");
        if (traffico>1) scheduleAt(simTime()+1, nuovaAutoMessage);
    } else if (msg == nuovaAutoMessage) {
        vveloce.id = findVehicleTypeIndex("veloce");
        vveloce.lane = 1;
        vveloce.position = 10;
        vveloce.speed = traffico>2 ? 40: 0;
        addVehicleToQueue(0, vveloce);
        scheduleAt(simTime()+2+rand()%4, nuovaAutoMessage);
    }
}

void PlatoonsTrafficManager::insertPlatoons()
{

    // compute intervehicle distance
    double distance = platoonInsertSpeed / 3.6 * platoonInsertHeadway + platoonInsertDistance;
    // total number of platoons per lane
    int nPlatoons = nCars / platoonSize / nLanes;
    // length of 1 platoon
    double platoonLength = platoonSize * 4 + (platoonSize - 1) * distance;
    // inter-platoon distance
    double platoonDistance = platoonInsertSpeed / 3.6 * platoonLeaderHeadway + platoonAdditionalDistance;
    // total length for one lane
    double totalLength = nPlatoons * platoonLength + (nPlatoons - 1) * platoonDistance;

    // for each lane, we create an offset to have misaligned platoons
    double* laneOffset = new double[nLanes];
    for (int l = 0; l < nLanes; l++) laneOffset[l] = uniform(0, 20);

    double currentRoadPosition = totalLength;
    int currentVehiclePosition = 0;
    int currentVehicleId = 0;
    int basePlatoonId = 0;
    
    if (traffico>0) {
        vcamion.id = findVehicleTypeIndex(camionVType);
        vcamion.lane = 0;
        vcamion.speed = 25;
        vcamion.position = currentRoadPosition+150;
        addVehicleToQueue(0, vcamion);
        vcamion.position = currentRoadPosition + 500;
        addVehicleToQueue(0, vcamion);
        vcamion.position = currentRoadPosition + 590;
        addVehicleToQueue(0, vcamion);
    }
    
    for (int i = 0; i < nCars / nLanes; i++) {
        for (int l = 0; l < nLanes; l++) {
            automated.position = currentRoadPosition + laneOffset[l];
            automated.lane = l;
            addVehicleToQueue(0, automated);
            positions.addVehicleToPlatoon(currentVehicleId, currentVehiclePosition, basePlatoonId + l);
            currentVehicleId++;
            if (currentVehiclePosition == 0) {
                PlatoonInfo info;
                info.speed = automated.speed;
                info.lane = automated.lane;
                positions.setPlatoonInformation(basePlatoonId + l, info);
            }
        }
        currentVehiclePosition++;
        if (currentVehiclePosition == platoonSize) {
            currentVehiclePosition = 0;
            // add inter platoon gap
            currentRoadPosition -= (platoonDistance + 4);
            basePlatoonId += nLanes;
        }
        else {
            // add intra platoon gap
            currentRoadPosition -= (4 + distance);
        }
    }

    delete[] laneOffset;
}

PlatoonsTrafficManager::~PlatoonsTrafficManager()
{
    cancelAndDelete(insertPlatoonMessage);
    insertPlatoonMessage = nullptr;
}

} // namespace plexe
