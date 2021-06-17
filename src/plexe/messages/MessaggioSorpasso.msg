cplusplus{{
#include "ManeuverMessage_m.h"
}};

packet ManeuverMessage;

packet MessaggioSorpasso extends ManeuverMessage {
    int tipo;
    int da;
    bool risposta;
}
