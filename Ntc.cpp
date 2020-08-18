#include "elementi.h"
#include "libreriafunzioni.h"
Ntc::Ntc(){
    tempo=0;
    flag_error=false;
}

bool Ntc::ErrorCheck(cell_asic bms_ic[],uint16_t temperatura,uint8_t current_IC,uint8_t current_NTC){
  if(temperatura  > MAXTEMP){
    if (flag_error==false){                    //si triggera l'if se c'è  
      flag_error=true;                         //un error_OT nuovo lo segno 
      tempo=millis();                          //e inizializzo il tempo.
    }
    /*se un error_OV è vecchio non c'è bisogno di flaggarlo
    ma bisogna controllare che il tempo non ecceda il limite
    questo avviene nell'else*/
    else{
      if(TimeCheck(tempo, OT_TIME_LIMIT)){
        ShoutdownError(bms_ic,current_IC,current_NTC+19); //NTC19->NTC1, NTC20->NTC2...
        return true;
      }
    }
  }
  else flag_error=false;                       //in asssenza di error_OV il flag è diasttivato
  return false;
}

bool Ntc::GetFlag(){
  return flag_error;
}

unsigned long Ntc::GetTempo(){
  return tempo;
}