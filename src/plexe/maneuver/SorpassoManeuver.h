#ifndef SORPASSOMANEUVER_H_
#define SORPASSOMANEUVER_H_

#include <algorithm>

#include "plexe/maneuver/Maneuver.h"
#include "plexe/messages/ManeuverMessage_m.h"
#include "plexe/messages/MessaggioSorpasso_m.h"
#include "plexe/messages/MessaggioSorpasso_m.h"
#include "plexe/utilities/BasePositionHelper.h"
#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "veins/base/utils/Coord.h"
#include "plexe/apps/BaseApp.h"

using namespace veins;

namespace plexe {

struct SorpassoManeuverParameters {
    double soglia, tAttesaRisposta, tRadar, tNoRadar;
};

enum class StatoSorpasso {
        ATTESARISPOSTE, RISULTATO, SORPASSO, QUIETE
};

class SorpassoManeuver : public Maneuver {
    public:
        const char NOMI_MESSAGGI[5][15] = {"INVIORICHIESTE", "MISURAZIONE", "ATTESARISPOSTE", "RISULTATO", "RIENTRO"};
        SorpassoManeuver(GeneralPlatooningApp* app);
        virtual ~SorpassoManeuver(){};

        void onManeuverMessage(const ManeuverMessage* mm) override;
        void startManeuver(const void* parameters) override;
        void abortManeuver() override;
        void onPlatoonBeacon(const PlatooningBeacon* pb) override;
        void onFailedTransmissionAttempt(const ManeuverMessage* mm) override;
        bool manovrando();
        void processaTimer (short int kind);
        cMessage *timer;
    protected:
        StatoSorpasso STATO = StatoSorpasso::QUIETE;
        bool capo = false;
        int risposte = 0;
        simtime_t *tempiConferma  = nullptr;
        void mandaFollowers(int tipo, bool risposta, bool ack);
        void mandaLeader(int tipo, bool risposta);
        bool radar(bool destra);
};

} // namespace plexe

#endif
