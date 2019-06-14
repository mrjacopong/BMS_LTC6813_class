#include "elementi.h"
#include "libreriafunzioni.h"

Cella::Cella(){
    tempo=0;
    flag_error=0;
}

bool Cella::error_check(uint16_t tensione){
    if(tensione> MAXVOLTAGE){
        if (flag_error==false){                                      //si triggera l'if se c'è
            tempo=millis();                                          //un error_OT nuovo lo segno
            flag_error=true;                                         //e inizializzo il tempo.
        }
    /*se un error_OV è vecchio non c'è bisogno di flaggarlo
    ma bisogna controllare che il tempo non ecceda il limite
    questo avviene nell'else*/
    else{
        if(time_check(tempo, OT_TIME_LIMIT))
            shoutdown_error(RelayPin); 
            return true;
        }
    }
    else flag_error=false;                                             //in asssenza di error_OV il flag è diasttivato
    
    return false;
}

bool Cella::carica(cell_asic bms_ic[]){

}