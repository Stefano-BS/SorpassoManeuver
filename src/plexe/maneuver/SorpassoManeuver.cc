#include "plexe/maneuver/SorpassoManeuver.h"
#include "plexe/apps/GeneralPlatooningApp.h"

#define cambiaColore traciVehicle->setColor(TraCIColor(rand()%255, rand()%255, rand()%255, 255));
#define soglia 2

namespace plexe {

    SorpassoManeuver::SorpassoManeuver(GeneralPlatooningApp* app)
        : Maneuver(app) {
        capo = false;
        STATO = StatoSorpasso::QUIETE;
        risposte = 0;
    }

    void SorpassoManeuver::startManeuver(const void* parameters) {
        if (capo = positionHelper->isLeader()) {
            //SorpassoManeuverParameters* pars = (SorpassoManeuverParameters*) parameters;
            cambiaColore

            if (!radar(false)) return;

            mandaFollowers(1, true, true);
            STATO = StatoSorpasso::ATTESARISPOSTE;
            // TIMER: Se qualcuno non risponde si torna in quiete
            cMessage * msg = new cMessage("timer");
            msg->setKind(1);
            app->schtimer(simTime()+1, msg);
        }
    }

    void SorpassoManeuver::onManeuverMessage(const ManeuverMessage* mm) {
        const MessaggioSorpasso* m = dynamic_cast<const MessaggioSorpasso*>(mm);

        if (mm->getPlatoonId() != positionHelper->getPlatoonId()) return;

        if (capo) { // FSM Leader
            if (STATO == StatoSorpasso::ATTESARISPOSTE && m->getTipo() == 2) {
                if (!m->getRisposta()) {
                    // ANNULLO MANOVRA
                    risposte = 0;
                    STATO = StatoSorpasso::QUIETE;
                } else {
                    if (++risposte >= positionHelper->getPlatoonSize()-1) {
                        // ESECUZIONE MANOVRA
                        // Invio conferma a tutti (tipo 3), esecuzione cambio corsia
                        risposte = 0;
                        mandaFollowers(3, true, true);
                        int corsiaSorpasso = traciVehicle->getLaneIndex()+1;
                        plexeTraciVehicle->changeLane(corsiaSorpasso, 0);
                        positionHelper->setPlatoonLane(corsiaSorpasso);
                        STATO = StatoSorpasso::SORPASSO;
                        tempiConferma = (simtime_t*)calloc(positionHelper->getPlatoonSize()-1, sizeof(simtime_t));
                    }
                }
            }
            else if (STATO == StatoSorpasso::SORPASSO && m->getTipo() == 4) {
                simtime_t ora = simTime();
                int dimPlatoon = positionHelper->getPlatoonSize(), i=0;
                tempiConferma[m->getDa()-1] = ora;
                for (simtime_t t=tempiConferma[i]; i<dimPlatoon-1; t=tempiConferma[++i])
                    if (ora.dbl() - t.dbl() > 1) return;    // Confronto da controllare
                // Hanno detto tutti che c'è la strada libera
                bool risultatoMisurazione = radar(true); // MISURAZIONE RADAR
                if (risultatoMisurazione) {
                    mandaFollowers(5, true, true);
                    int corsiaRientro = traciVehicle->getLaneIndex() >0 ? traciVehicle->getLaneIndex()-1 : 0;
                    plexeTraciVehicle->changeLane(corsiaRientro, 0);
                    positionHelper->setPlatoonLane(corsiaRientro);
                    STATO = StatoSorpasso::QUIETE;
                    free(tempiConferma);
                    tempiConferma = nullptr;
                }
            }
        }
        else {    // FSM Follower
            if (STATO == StatoSorpasso::QUIETE && m->getTipo() == 1) {
                bool risultatoMisurazione = radar(false);
                mandaLeader(2, risultatoMisurazione);
                if (risultatoMisurazione) {
                    STATO = StatoSorpasso::RISULTATO;
                    // TIMER: Se il leader non conferma in tempo pazienza, la manovra verrà reinnescata da fuori
                    cMessage * msg = new cMessage("timer");
                    msg->setKind(1);
                    app->schtimer(simTime()+0.1, msg);
                }
                else STATO = StatoSorpasso::QUIETE;
            }
            else if (STATO == StatoSorpasso::RISULTATO && m->getTipo() == 3) {
                // ESECUZIONE MANOVRA
                plexeTraciVehicle->changeLane(traciVehicle->getLaneIndex()+1, 0);
                STATO = StatoSorpasso::SORPASSO;
                // TIMER: Al termine del quale fare la prima misurazione radar
                cMessage * msg = new cMessage("timer");
                msg->setKind(0);
                app->schtimer(simTime()+10, msg);
            }
            else if (STATO == StatoSorpasso::SORPASSO && m->getTipo() == 5) {
                // ESECUZIONE RIENTRO
                plexeTraciVehicle->changeLane(traciVehicle->getLaneIndex() > 0 ? traciVehicle->getLaneIndex()-1 : 0, 0);
                STATO = StatoSorpasso::QUIETE;
            }
        }
    }

    void SorpassoManeuver::processaTimer (short int kind) {
        if (kind == 0) { // Prima misurazione radar
            if (STATO == StatoSorpasso::SORPASSO && !capo) {
                bool risultatoMisurazione = radar(true);
                if (risultatoMisurazione)
                    mandaLeader(4, true);
                // TIMER: Al termine del quale fare un'altra misurazione radar
                cMessage * msg = new cMessage("timer");
                msg->setKind(0);
                app->schtimer(simTime()+1, msg);
            }
        }
        else if (kind == 1) { // Timeout inizio manovra
            if (STATO == StatoSorpasso::RISULTATO || STATO == StatoSorpasso::ATTESARISPOSTE) STATO = StatoSorpasso::QUIETE;
        }
    }

    void SorpassoManeuver::onPlatoonBeacon(const PlatooningBeacon* pb) {
        double accel = traciVehicle->getAcceleration()*100;
        if (accel != 0 && manovrando()) positionHelper->statistica(accel);
    }
    void SorpassoManeuver::abortManeuver() {}
    void SorpassoManeuver::onFailedTransmissionAttempt(const ManeuverMessage* mm) {
        throw cRuntimeError("Impossibile mandare PACCHETTO %s. Maximum number of unicast retries reached", mm->getName());
    }

    bool SorpassoManeuver::manovrando() {
        return STATO != StatoSorpasso::QUIETE;
    }

    void SorpassoManeuver::mandaFollowers(int tipo, bool risposta, bool ack) {
        if (ack) {
            std::vector<int> elementi = positionHelper->getPlatoonFormation();
            for (auto it = elementi.begin(); it != elementi.end(); ++it) {
                if (*it == positionHelper->getLeaderId()) continue;

                MessaggioSorpasso * req = new MessaggioSorpasso(NOMI_MESSAGGI[tipo], MANEUVER_TYPE);
                req->setTipo(tipo);
                req->setRisposta(risposta);
                req->setDa(positionHelper->getPosition());
                req->setPlatoonId(positionHelper->getPlatoonId());
                app->sendUnicast(req, *it);
            }
         } else {
            MessaggioSorpasso * req = new MessaggioSorpasso(NOMI_MESSAGGI[tipo], MANEUVER_TYPE);
            req->setTipo(tipo);
            req->setRisposta(risposta);
            req->setDa(positionHelper->getPosition());
            req->setPlatoonId(positionHelper->getPlatoonId());
            app->sendUnicast(req, -1);
        }
    }

    void SorpassoManeuver::mandaLeader(int tipo, bool risposta) {
        MessaggioSorpasso * req = new MessaggioSorpasso(NOMI_MESSAGGI[tipo], MANEUVER_TYPE);
        req->setTipo(tipo);
        req->setRisposta(risposta);
        req->setDa(positionHelper->getPosition());
        req->setPlatoonId(positionHelper->getPlatoonId());
        app->sendUnicast(req, positionHelper->getLeaderId());
    }

    bool SorpassoManeuver::radar(bool destra) {
        for (int indietro=0; indietro<2; indietro++) {
            double *r = plexeTraciVehicle->getRadarString(destra, indietro);

            int i = 0;
            if (r[0] != -1) {
                double p1l = r[0], p2l = r[1], v = r[2];
                i = 3;
                while (r[i] != -1) {
                    //if ((p1l-r[i])*(p1l-r[i]) + (p2l-r[i+1])*(p2l-r[i+1]) < 500) return false; funzione che impiega solo distanza euclidea
                    // Serve che la (stima della) distanza sia tale da non essere coperta in due secondi (accettabile?)
                    double distanza = pow(pow(p1l-r[i], 2) + pow(p2l-r[i+1], 2), 0.5) - traciVehicle->getLength()*1.5;
                    double dt = distanza / (v - r[i+2]);
                    if (distanza<0 || (indietro && dt<0 && dt>-soglia) || (!indietro && dt>0 && dt < soglia)) {
                        free(r);
                        return false;
                    }
                    i+=3;
                }
            }
            free(r);
        }
        return true;
    }
}
