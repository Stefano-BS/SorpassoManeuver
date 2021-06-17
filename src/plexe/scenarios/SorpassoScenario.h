#ifndef SORPASSOSCENARIO_H_
#define SORPASSOSCENARIO_H_

#include "plexe/scenarios/BaseScenario.h"
#include "plexe/apps/GeneralPlatooningApp.h"
#include "plexe/messages/ManeuverMessage_m.h"

namespace plexe {

    class SorpassoScenario : public BaseScenario {
        public:
            virtual void initialize(int stage) override;

        protected:
            double leaderSpeed; // leader average speed
            GeneralPlatooningApp* appl; // application layer, used to stop the simulation
            cMessage *controlloLentoLeader;

        public:
            static const int MANEUVER_TYPE = 12347;

        public:
            SorpassoScenario() {
                    appl = nullptr;
                    controlloLentoLeader = nullptr;
                }
                virtual ~SorpassoScenario();
        protected:
            virtual void handleSelfMsg(cMessage* msg) override;
    };

}

#endif
