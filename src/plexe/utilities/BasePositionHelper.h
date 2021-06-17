#ifndef BASEPOSITIONHELPER_H_
#define BASEPOSITIONHELPER_H_

#include "plexe/utilities/DynamicPositionManager.h"
#include <string>
#include "veins/modules/mobility/traci/TraCIMobility.h"

#define INVALID_PLATOON_ID -99

namespace plexe {

class BasePositionHelper : public cSimpleModule {

public:
    void statistica(long val);
    cLongHistogram statistiche;
    cOutVector vettoreStat;
    
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override;

    /**
     * Returns the traci external id of this car
     */
    std::string getExternalId() const;

    /**
     * Returns the id of this car depending on its name
     */
    static int getIdFromExternalId(const std::string externalId);

    /**
     * Returns the numeric id of this car
     */
    virtual int getId() const;

    /**
     * Returns the position of this vehicle within the platoon
     */
    virtual int getPosition() const;

    /**
     * Returns the id of the i-th vehicle of the own platoon
     */
    virtual int getMemberId(int position) const;

    /**
     * Returns the position of a vehicle of the own platoon
     */
    virtual int getMemberPosition(int vehicleId) const;

    /**
     * Returns the id of the leader of the own platoon
     */
    virtual int getLeaderId() const;

    /**
     * Returns whether this vehicle is the leader of the platoon
     */
    virtual bool isLeader() const;

    /**
     * Returns whether this vehicle is the last of the platoon
     */
    virtual bool isLast() const;

    /**
     * Returns the id of the vehicle in front of me
     */
    virtual int getFrontId() const;

    /**
     * Retuns the id of the vehicle in the back of me
     */
    virtual int getBackId() const;

    /**
     * Returns the id of the platoon
     */
    virtual int getPlatoonId() const;

    /**
     * Returns the lane the platoon is traveling on
     */
    virtual int getPlatoonLane() const;

    /**
     * Returns the speed the platoons is cruising at
     */
    virtual double getPlatoonSpeed() const;

    /**
     * Returns whether a vehicle is part of my platoon
     */
    virtual bool isInSamePlatoon(int vehicleId) const;

    /**
     * Returns the platoon size
     */
    virtual int getPlatoonSize() const;

    /**
     * Sets the id of this car
     */
    virtual void setId(int id);

    /**
     * Sets the id of the platoon
     */
    virtual void setPlatoonId(int id);

    /**
     * Sets the lane the platoon is traveling on
     */
    virtual void setPlatoonLane(int lane);

    /**
     * Sets the cruising speed of this platoon
     */
    virtual void setPlatoonSpeed(double speed);

    /**
     * Returns the platoon formation
     */
    virtual const std::vector<int>& getPlatoonFormation() const;

    /**
     * Sets the platoon formation
     */
    virtual void setPlatoonFormation(const std::vector<int>& formation);

    /**
     * Writes a dump of the variables of this vehicle for debug purposes
     */
    virtual void dumpVehicleData() const;

protected:
    veins::TraCIMobility* mobility;
    veins::TraCICommandInterface* traci;
    veins::TraCICommandInterface::Vehicle* traciVehicle;

    // id of this vehicle
    int myId;
    // id of the leader of the platoon
    int leaderId;
    // id of the vehicle in front of me
    int frontId;
    // id of the vehicle in the back of me
    int backId;
    // my position within the platoon
    int position;
    // id of this car's platoon
    int platoonId;
    // lane of this car's platoon
    int platoonLane;
    // speed of the platoon
    double platoonSpeed;

    /** Stores the IDs of vehicles currently in the platoon.
     * The values' order corresponds to that of the platoon.
     */
    std::vector<int> formation;

    /** Maps the IDs of the vehicles to their position in the formation.
     * This is useful to search for members without going through the complete formation vector.
     */
    std::map<int, int> memberToPosition;

    // used to retrieve the initial formation setup
    DynamicPositionManager& positions;

    virtual void setVariablesAfterFormationChange();

    virtual void colorVehicle();

public:
    BasePositionHelper()
        : mobility(nullptr)
        , traci(nullptr)
        , traciVehicle(nullptr)
        , myId(INVALID_PLATOON_ID)
        , leaderId(INVALID_PLATOON_ID)
        , frontId(INVALID_PLATOON_ID)
        , backId(INVALID_PLATOON_ID)
        , position(-1)
        , platoonId(INVALID_PLATOON_ID)
        , platoonLane(-1)
        , platoonSpeed(-1)
        , positions(DynamicPositionManager::getInstance())
    {
    }
};

} // namespace plexe

#endif
