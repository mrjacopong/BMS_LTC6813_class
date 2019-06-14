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

bool Cella::carica(uint8_t tensione,cell_asic bms_ic[],uint16_t top_voltage){
    bool modulo_carico=0;
    uint8_t numero_moduli_carichi=0;

  //for (uint8_t current_ic = 0; current_ic < TOTAL_IC; current_ic++) {
    //modulo_carico=true;
    /* true-> tutte cariche */
    /* false-> non tutte cariche */
   // for (uint8_t current_ch = 0; current_ch < TOTAL_CH; current_ch++) {
      /*aggiorno la tensione massima per ogni IC*/
      /*controllo se la cella non è carica*/
      if( tensione < SogliaCarica){
        modulo_carico=false;                                        //se le celle sono tutte cariche
      }                                                             // allora la variabile rimane true  
      else{
        numero_moduli_carichi++;
        /*bilanciamento finale*/
      }
      if(tensione-top_voltage>delta_carica){
        intermediate_balance(current_ch,bms_ic);
        if(tensione-top_voltage>delta_carica+0.2){
          /*bilanciamento intermedio ma più potente*/
          /*ferma la carica e bilancia*/
          greater_balance(current_ch,RelayPin,bms_ic);
        }
      }
  if(numero_moduli_carichi==TOTAL_IC){
    return stop_charge(RelayPin);
  }
  return true;

}