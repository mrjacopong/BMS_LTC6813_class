#include "elementi.h"
#include "libreriafunzioni.h"

Cella::Cella(){
    tempo=0;
    flag_error=false;
    flag_inScarica=false;
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
        shoutdown_error(); 
      return true;
    }
  }
  else flag_error=false;                                             //in asssenza di error_OV il flag è diasttivato
    
  return false;
}

bool Cella::carica(uint16_t tensione,cell_asic bms_ic[],uint16_t low_voltage,uint8_t modulo_corrente,uint8_t cella_corrente,unsigned long *tempoIniziale){
  /*controllo se la cella è carica*/
  if( tensione >= SogliaCarica){
    final_balance(low_voltage,tensione,RelayPin,bms_ic,modulo_corrente,cella_corrente);
    Serial.println("la cella è carica");
    return true;  //true -> la cella è carica
    flag_inScarica=false;
  }

  if(tensione-low_voltage>=delta_carica){       //c'è da fare una scarica
    if(tensione-low_voltage>delta_carica+delta_carica){
      greater_balance(low_voltage,bms_ic, modulo_corrente, cella_corrente);
      *tempoIniziale=millis();
    }
    else{
      intermediate_balance(cella_corrente,bms_ic);
      flag_inScarica=true;
    }
  }
  else{ 
    if(flag_inScarica && tensione-low_voltage<delta_carica -100){
      reset_discharge(bms_ic);   //se si è scaricata abbastaza disattivo la scarica e spengo il flag
      flag_inScarica=false;
    }
  
  }
  return false;                   //ritonra false -> vuol dire che la cella non è carica
}


bool Cella::get_flag(){
  return flag_error;
}

bool Cella::get_flagInScarica(){
  return flag_inScarica;
}

unsigned long Cella::get_tempo(){
  return tempo;
}