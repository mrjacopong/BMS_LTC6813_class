#include "elementi.h"
#include "libreriafunzioni.h"
Ntc::Ntc(){
    tempo=0;
    flag_error=false;
}

bool Ntc::ErrorCheck(uint16_t temperatura){
  if(temperatura  > MAXTEMP){
    if (flag_error==false){                    //si triggera l'if se c'è  
      flag_error=true;                         //un error_OT nuovo lo segno 
      tempo=millis();                          //e inizializzo il tempo.
    }
    /*se un error_OV è vecchio non c'è bisogno di flaggarlo
    ma bisogna controllare che il tempo non ecceda il limite
    questo avviene nell'else*/
    else{
      if(TimeCheck(tempo, OT_TIME_LIMIT))
        ShoutdownError();
        return true;
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