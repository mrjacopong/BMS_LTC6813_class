#include "elementi.h"
#include "libreriafunzioni.h"

Cella::Cella(){
    tempo=0;
    flag_error=0;
}

bool Cella::error_check(cell_asic bms_ic[]){
    for(uint8_t current_ic=0;current_ic<TOTAL_IC;current_ic++){
		for(uint8_t current_ch=0;current_ch<TOTAL_CH;current_ch++){
        /**overcurrent**/
        //salta la cella  unused_ch_1=9 e unused_ch_2=18
        if ((current_ch == unused_ch_1) || (current_ch == unused_ch_2)){}
        if(bms_ic[current_ic].cells.c_codes[current_ch]> MAXVOLTAGE){
            if (flag_error==false){           //si triggera l'if se c'è
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
        }
    }
    return false;
}

bool Cella::carica(cell_asic bms_ic[]){

}