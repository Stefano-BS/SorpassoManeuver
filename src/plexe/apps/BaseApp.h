#ifndef BASEAPP_H_
#define BASEAPP_H_

#include "veins/base/modules/BaseApplLayer.h"
#include "veins/modules/mobility/traci/TraCIMobility.h"

#include "plexe/CC_Const.h"
#include "plexe/messages/PlatooningBeacon_m.h"
#include "plexe/mobility/CommandInterface.h"
#include "plexe/utilities/BasePositionHelper.h"

namespace plexe {

class BaseProtocol;

class BaseApp : public veins::BaseApplLayer {

public:
    virtual void initialize(int stage) override;

protected:
    // id of this vehicle
    int myId;

    veins::TraCIMobility* mobility;
    veins::TraCICommandInterface* traci;
    veins::TraCICommandInterface::Vehicle* traciVehicle;
    traci::CommandInterface* plexeTraci;
    std::unique_ptr<traci::CommandInterface::Vehicle> plexeTraciVehicle;

    // determines position and role of each vehicle
    BasePositionHelper* positionHelper;

    // lower layer protocol
    BaseProtocol* protocol;

    /**
     * Log data about vehicle
     */
    virtual void logVehicleData(bool crashed = false);

    // output vectors for mobility stats
    // id of the vehicle
    cOutVector nodeIdOut;
    // distance and relative speed
    cOutVector distanceOut, relSpeedOut;
    // speed and position
    cOutVector speedOut, posxOut, posyOut;
    // real acceleration and controller acceleration
    cOutVector accelerationOut, controllerAccelerationOut;

    // messages for scheduleAt
    cMessage* recordData;
    // message to stop the simulation in case of collision
    cMessage* stopSimulation;

public:
    BaseApp()
    {
        recordData = 0;
        stopSimulation = nullptr;
    }
    virtual ~BaseApp();

    /**
     * Sends a frame
     *
     * @param msg message to be encapsulated into the frame
     * @param destination id of the destination
     */
    void sendFrame(cPacket* msg, int destination);
    void schtimer(simtime_t t, cMessage *msg);

protected:
    virtual void handleLowerMsg(cMessage* msg) override;
    virtual void handleSelfMsg(cMessage* msg) override;
    virtual void handleLowerControl(cMessage* msg) override;

    /**
     * Handles PlatoonBeacons
     */
    virtual void onPlatoonBeacon(const PlatooningBeacon* pb);
};

} // namespace plexe

#endif /* BASEAPP_H_ */
