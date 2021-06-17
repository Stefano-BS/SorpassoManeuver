#ifndef PLATOONSTRAFFICMANAGER_H_
#define PLATOONSTRAFFICMANAGER_H_

#include "plexe/mobility/TraCIBaseTrafficManager.h"

namespace plexe {

class PlatoonsTrafficManager : public TraCIBaseTrafficManager {

public:
    virtual void initialize(int stage);

    PlatoonsTrafficManager()
    {
        insertPlatoonMessage = nullptr;
        platoonInsertDistance = 0;
        platoonInsertHeadway = 0;
        platoonInsertSpeed = 0;
        platoonInsertTime = SimTime(0);
        platoonLeaderHeadway = 0;
        platoonAdditionalDistance = 0;
        platoonSize = 0;
        nCars = 0;
        nLanes = 0;
        traffico = 0;
    }
    virtual ~PlatoonsTrafficManager();

protected:
    // this is used to start traffic generation
    cMessage* insertPlatoonMessage, *nuovaAutoMessage;

    void insertPlatoons();

    virtual void handleSelfMsg(cMessage* msg);

    SimTime platoonInsertTime;
    double platoonInsertSpeed;
    // vehicles to be inserted
    struct Vehicle automated, vcamion, vveloce;

    int nCars, platoonSize, nLanes, traffico;
    
    // insert distance
    double platoonInsertDistance;
    // insert headway
    double platoonInsertHeadway;
    // headway for leader vehicles
    double platoonLeaderHeadway;
    // additional distance between consecutive platoons
    double platoonAdditionalDistance;
    // sumo vehicle type of platooning cars
    std::string platooningVType, camionVType;

    virtual void scenarioLoaded();
};

} // namespace plexe

#endif
