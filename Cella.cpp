#include "elementi.h"
#include "libreriafunzioni.h"

Cella::Cella(){
    tempo=0;
    flag_error=false;
    flag_in_scarica=false;
}

bool Cella::ErrorCheck(uint16_t tensione){
  if(tensione> MAXVOLTAGE){
    if (flag_error==false){                                    //si triggera l'if se c'è
      tempo=millis();                                          //un error_OT nuovo lo segno
      flag_error=true;                                         //e inizializzo il tempo.
            
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
  else flag_error=false;                                        //in asssenza di error_OV il flag è diasttivato
    
  return false;
}

bool Cella::carica(uint16_t tensione,cell_asic bms_ic[],uint16_t Low_voltage,uint8_t modulo_corrente,uint8_t cella_corrente,unsigned long *tempo_iniziale){
  /*controllo se la cella è carica*/
  if( tensione >= soglia_carica){
    bool prec_falg_in_scarica=flag_in_scarica;
    flag_in_scarica=FinalBalance(Low_voltage,tensione,bms_ic,modulo_corrente,cella_corrente,tempo_iniziale); //bilanciamento specifico per fine carica
    
    if (prec_falg_in_scarica==true && flag_in_scarica==false){
      if(tensione-Low_voltage<=100){
        ResetDischarge(bms_ic);
        flag_in_scarica=false;
      }
      else
        flag_in_scarica=true;
    }
    return true;  //true -> la cella è carica
  }

  if(tensione-Low_voltage>=delta_carica){                //c'è da fare una scarica per troppa differenza di potenziale
    if(tensione-Low_voltage>delta_carica+delta_carica){  //se la tenisone della cella corrente è molto maggiore di
      Serial.println("GreaterBalance");                  //quella della cella a tensione minore avviamo il GreaterBalance
      GreaterBalance(Low_voltage,bms_ic, modulo_corrente, cella_corrente);
      *tempo_iniziale=millis();                          //inizializziamo il contatore che interrompera' il GreaterBalance dopo 1 minuto
    }
    else{
      IntermediateBalance(cella_corrente,bms_ic);        //se non è necessaria la GreaterBalance usiamo la IntermediateBalance
      flag_in_scarica=true;                              //settiamo il flag della scarica intermedia su vero
    }
  }
  else{ 
    if(flag_in_scarica && tensione-Low_voltage<delta_carica -100){ //se si è scaricata abbastaza disattivo la scarica e spengo il flag
      ResetDischarge(bms_ic);   
      flag_in_scarica=false;
    }
  
  }
  return false;                   //ritonra false -> vuol dire che la cella non è carica
}


bool Cella::GetFlag(){
  return flag_error;
}

unsigned long Cella::GetTempo(){
  return tempo;
}