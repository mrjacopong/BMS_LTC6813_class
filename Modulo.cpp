#include "elementi.h"
#include "libreriafunzioni.h"

Modulo::Modulo(int N_celle,int N_ntc){
    flag_error=false;
    low_voltage=60000;
    modulo_carico=false;
    n_celle=N_celle;
    n_ntc=N_ntc;
    tempoIniziale=0;
    cella= new Cella* [N_celle];
    for (int i=0;i<N_celle;i++){
    cella[i] = new Cella();
    }
    ntc= new Ntc* [N_ntc];
    for (int j=0;j<N_ntc;j++){
    ntc[j] = new Ntc();
    }

}
bool Modulo::error_check(cell_asic bms_ic[],int modulo_corrente){
    for (int i=0;i<n_celle;i++){ //controllo errore overvoltage
        //salta la cella  unused_ch_1=9 e unused_ch_2=18
        //queste due celle sono sempre a 0v e non sono un errore
        if ((modulo_corrente== unused_ch_1) || (modulo_corrente == unused_ch_2))
            return false;
        if (cella[i]->error_check(bms_ic[modulo_corrente].cells.c_codes[i])==true) 
            return true;
    }
    for (int j=0;j<n_ntc;j++){ //controllo errore overtemperature
       if( ntc[j]->error_check(bms_ic[modulo_corrente].aux.a_codes[j])==true)
            return true;
    }
    return false;
}

bool Modulo::carica(cell_asic bms_ic[],int modulo_corrente){
    low_voltage=60000; //inizzializzo il low_voltage ad ogni ciclo in quanto lo devo confrontare con ogni cella
    if(tempoIniziale==0){//altrimenti viene confrontato il low_voltage vecchio
        modulo_carico=true;           //diventa false se c'è almeno una cella scarcia
        for (int i=0;i<n_celle;i++){
            if ( i!= unused_ch_1 && i!= unused_ch_2) {
                low_voltage=IsLow(low_voltage,bms_ic[modulo_corrente].cells.c_codes[i]); //trovo tensione minore    
                if (!cella[i]->carica(bms_ic[modulo_corrente].cells.c_codes[i],bms_ic,low_voltage,modulo_corrente,i,&tempoIniziale))
                    modulo_carico=false;  //se c'è almeno una cella scarica vuol dire che il modulo non è carico 
            }
        }
    }
    else if(millis()-tempoIniziale>=60000){ //una volta che siamo entrati in greater balance
        tempoIniziale=0;                    //aspettiamo 60 secondi in modo da far scaricare
        reset_discharge(bms_ic);            //le celle a tensione molto più alta
        SpegniLed(LedBilanciamentoPesante);
        close_relay(RelayPin);
        modulo_carico=false;
    }
    return modulo_carico;
}

void Modulo::StampaVoltaggio (cell_asic bms_ic[],int modulo_corrente){  //stampa nel monitor seriale di arduino
    for (int i=0;i<n_celle;i++){
        //salta la cella  unused_ch_1=9 e unused_ch_2=18
        //queste due celle sono sempre a 0v e non sono un errore
        if ((modulo_corrente!= unused_ch_1) || (modulo_corrente != unused_ch_2)){
            Serial.println(bms_ic[modulo_corrente].cells.c_codes[i]);
            Serial.println("V");
        }
    }
}
void Modulo::StampaTemp (cell_asic bms_ic[],int modulo_corrente){   //stampa nel monitor seriale di arduino
    for (int i=0;i<n_ntc;i++){
        //salta la cella  unused_ch_1=9 e unused_ch_2=18
        //queste due celle sono sempre a 0v e non sono un errore
        if ((modulo_corrente!= unused_ch_1) || (modulo_corrente != unused_ch_2)){
            Serial.print(bms_ic[modulo_corrente].aux.a_codes[i]);
            Serial.println("V");
        }
    }
}

bool Modulo::get_flag(){
  return flag_error;
}

bool Modulo::get_modulocarico(){
    return  modulo_carico;
}


int Modulo::get_Ncelle(){
    return n_celle;  
}

int Modulo::get_Nntc(){
    return n_ntc;
}

uint16_t Modulo::get_lowvoltage(){
    return low_voltage;
}

