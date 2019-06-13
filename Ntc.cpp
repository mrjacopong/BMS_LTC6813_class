#include "elementi.h"
#include "libreriafunzioni.h"
Ntc::Ntc(){
    tempo=0;
    flag_error=0;
}

bool Ntc::error_check(cell_asic bms_ic[]){
    for(uint8_t current_ic=0;current_ic<TOTAL_IC;current_ic++){
for(uint8_t current_ntc=0;current_ntc<TOTAL_NTC;current_ntc++){
      if(bms_ic[current_ic].aux.a_codes[current_ntc]  > MAXTEMP){
        if (flag_error==false){                                       //si triggera l'if se c'è  
          flag_error=true;                                            //un error_OT nuovo lo segno 
          tempo=millis();                                             //e inizializzo il tempo.
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
      else flag_error=false;            //in asssenza di error_OV il flag è diasttivato
     }
return false;
}
}

