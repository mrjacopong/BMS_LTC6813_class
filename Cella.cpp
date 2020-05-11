#include "elementi.h"
#include "libreriafunzioni.h"

Cella::Cella(){
    tempo=0;
    flag_error=false;
    flag_in_scarica=false;
    //soc=soc()
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
  if(tensione <= cutoff_voltage){                               //controlla se c'è un under voltage, non è necessariamente un errore, ma stacca il carico
    UnderVoltageShoutdown();
    return true;
  }
  return false;
}

bool Cella::carica(uint16_t tensione,cell_asic bms_ic[],uint16_t Low_voltage,uint8_t modulo_corrente,uint8_t cella_corrente,unsigned long* tempo_iniziale){
  if(Low_voltage==60000)  //necessario per non buggare con la prima iterazione (quando low voltage = 60000 fittizio)
    return false;
  /*controllo se la cella è carica*/
  if( tensione >= soglia_carica){
    bool prec_falg_in_scarica=flag_in_scarica;
    flag_in_scarica=FinalBalance(Low_voltage,tensione,bms_ic,modulo_corrente,cella_corrente,tempo_iniziale); //bilanciamento specifico per fine carica
    
    if (prec_falg_in_scarica==true && flag_in_scarica==false){    //serve solo per evitare accendi/spegni delle resistenze di scarica
      if(tensione-Low_voltage<=bilanciamento_off){                //dovute alla caduta di potenziale quando si collega la R di scarica
        ResetDischarge(bms_ic);
        flag_in_scarica=false;
      }
      else
        flag_in_scarica=true;
        IntermediateBalance(cella_corrente,bms_ic);
    }
    return true;  //true -> la cella è arrivata alla tensione di soglia
  }
  if(tensione-Low_voltage>=delta_carica){                         //c'è da fare una scarica per troppa differenza di potenziale
    if(tensione-Low_voltage>(delta_carica+delta_carica)){         //se la tenisone della cella corrente è molto maggiore di
      Serial.print("GreaterBalance ");                            //quella della cella a tensione minore avviamo il GreaterBalance
      Serial.print("low: ");
      Serial.print(Low_voltage);
      Serial.print("cella: ");
      Serial.println(tensione);
      GreaterBalance(Low_voltage,bms_ic, modulo_corrente, cella_corrente);
      *tempo_iniziale=millis();                                   //inizializziamo il contatore che interrompera' il GreaterBalance dopo 1 minuto
    }
    else{
      flag_in_scarica=true;                                       //settiamo il flag della scarica intermedia su vero
    }
  }
  else{ 
    if(flag_in_scarica && tensione-Low_voltage<bilanciamento_off){//se si è scaricata abbastaza disattivo la scarica e spengo il flag
      ResetDischarge(bms_ic);   
      flag_in_scarica=false;
    }
  }
  if(flag_in_scarica)
    IntermediateBalance(cella_corrente,bms_ic);
  return false;   //ritonra false -> vuol dire che la cella non è carica
}

bool Cella::Bilancia(uint16_t tensione,cell_asic bms_ic[],uint16_t Low_voltage,uint8_t modulo_corrente,uint8_t cella_corrente){
  if(Low_voltage==60000)
    return false;
  if(tensione-Low_voltage>=delta_bilanciamento ){
    flag_in_scarica=true;
  }
  else{ 
    if(flag_in_scarica && tensione-Low_voltage<delta_bilanciamento -20){
      ResetDischarge(bms_ic);   
      flag_in_scarica=false;
    }
  }
  if(flag_in_scarica)
    IntermediateBalance(cella_corrente,bms_ic);
  return !flag_in_scarica;
}

uint16_t Cella::Soc(){
/*
  if (corrente()==0){
    calcola_soc_da_ocv()
  }
  if (corrente > 0){
    if (batteria_carica == true){
      ricalibra_lo_stato_full()
    }
    else{
      calcola_soc_con_CC
    }
  }
  else{
    if(tensione < treshold){
      spegni_tutto_battery_low()
      ricalibra_lo_stato_empty()
    }
    else{
      calcola_dod_con_CC()
    }
  }
*/
  return 0;
}

bool Cella::GetFlag(){
  return flag_error;
}
uint16_t Cella::GetSoc(){
  return soc;
}

unsigned long Cella::GetTempo(){
  return tempo;
}