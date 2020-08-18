#include "elementi.h"
#include "libreriafunzioni.h"

Modulo::Modulo(int N_celle,int N_ntc){
    flag_error=false;
    modulo_carico=false;
    n_celle=N_celle;
    n_ntc=N_ntc;
    tempo_iniziale=0;
    
    cella= new Cella* [N_celle];
    for (int i=0;i<N_celle;i++){
        cella[i] = new Cella();
        }

    ntc= new Ntc* [N_ntc];
    for (int j=0;j<N_ntc;j++){
        ntc[j] = new Ntc();
        }
    //soc=soc()
}
bool Modulo::ErrorCheck(cell_asic bms_ic[],int modulo_corrente){
    for (int i=0;i<n_celle;i++){                   //controllo errore overvoltage
        //salta la cella  unused_ch_1=9 e unused_ch_2=18
        //queste due celle sono sempre a 0v e non sono un errore
        if ((i== unused_ch_1) || (i == unused_ch_2))
            return false;
        if (cella[i]->ErrorCheck(bms_ic,bms_ic[modulo_corrente].cells.c_codes[i],modulo_corrente,i) == true )
            return true;
    }
    for (int j=0;j<n_ntc;j++){                     //controllo errore overtemperature
       if( ntc[j]->ErrorCheck(bms_ic,bms_ic[modulo_corrente].aux.a_codes[j],modulo_corrente,j) == true )
            return true;
    }
    return false;
}

bool Modulo::carica(cell_asic bms_ic[],int modulo_corrente, uint16_t* low_voltage_puntatore){
    uint16_t low_voltage= *low_voltage_puntatore;
    uint16_t prec_low_voltage=low_voltage;         //evita che la prima cella non si scarichi mai inizzializzo il low_voltage ad ogni ciclo 
    low_voltage=60000;                             //in quanto lo devo confrontare con ogni cella altrimenti viene confrontato il low_voltage vecchio
    if(tempo_iniziale==0){
        modulo_carico=true;                        //diventa false se c'è almeno una cella scarcia
        for (int i=0;i<n_celle;i++){
            if ( i!= unused_ch_1 && i!= unused_ch_2) {
                low_voltage=IsLow(low_voltage,bms_ic[modulo_corrente].cells.c_codes[i]); //trovo tensione minore
                if (low_voltage<=prec_low_voltage) //controllo per avere sempre la tensione più bassa ad ogni ciclo (referito al for poco sopra)
                    prec_low_voltage=low_voltage;   
                if (!cella[i]->carica(bms_ic[modulo_corrente].cells.c_codes[i],bms_ic,prec_low_voltage,modulo_corrente,i,&tempo_iniziale))
                    modulo_carico=false;           //se c'è almeno una cella scarica vuol dire che il modulo non è carico 
            }
        }
    }
    else{
        RefreshDischarge(bms_ic);
        if(millis()-tempo_iniziale>=60000){        //una volta che siamo entrati in greater balance
            tempo_iniziale=0;                      //aspettiamo 60 secondi in modo da far scaricare
            SpegniLed(led_bilanciamento_pesante);  //le celle a tensione molto più alta
            CloseRelay(relay_pin);
            modulo_carico=false;
        }
    }
    return modulo_carico;
}
bool Modulo::Bilancia(cell_asic bms_ic[],int modulo_corrente, uint16_t* low_voltage_puntatore){
    uint16_t low_voltage= *low_voltage_puntatore;
    uint16_t prec_low_voltage=low_voltage;         //evita che la prima cella non si scarichi mai
    low_voltage=60000;                             //inizzializzo il low_voltage ad ogni ciclo in quanto lo devo confrontare con ogni cella
    bool modulo_bilanciato=true;
    for (int i=0;i<n_celle;i++){
        if ( i!= unused_ch_1 && i!= unused_ch_2) {
            low_voltage=IsLow(low_voltage,bms_ic[modulo_corrente].cells.c_codes[i]); //trovo tensione minore
            if (low_voltage<=prec_low_voltage)      //controllo per avere sempre la tensione più bassa ad ogni ciclo
                prec_low_voltage=low_voltage;   
            if (!cella[i]->Bilancia(bms_ic[modulo_corrente].cells.c_codes[i],bms_ic,prec_low_voltage,modulo_corrente,i))
                modulo_bilanciato=false;            //se c'è almeno una cella scarica vuol dire che il modulo non è carico 
        }
    }
    return modulo_bilanciato;
}

uint16_t Modulo::Soc(){
    for (int i=0;i<n_celle;i++){
        soc=cella[i]->Soc();
    }
    return soc/(n_celle-2);                         //non conto le 2 celle che non usiamo
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
void Modulo::StampaTemp (cell_asic bms_ic[],int modulo_corrente){       //stampa nel monitor seriale di arduino
    for (int i=0;i<n_ntc;i++){
        //salta la cella  unused_ch_1=9 e unused_ch_2=18
        //queste due celle sono sempre a 0v e non sono un errore
        if ((modulo_corrente!= unused_ch_1) || (modulo_corrente != unused_ch_2)){
            Serial.print(bms_ic[modulo_corrente].aux.a_codes[i]);
            Serial.println("V");
        }
    }
}

void Modulo::StampaDebug (cell_asic bms_ic[],int modulo_corrente){

    for(int i=0; i<TOTAL_CH ;i++){
        if(i!=unused_ch_1 && i!=unused_ch_2){
            Serial.print(bms_ic[modulo_corrente].cells.c_codes[i]*0.0001,4);
            Serial.print(";");
        }
    }

    for(int i=0; i<ntc_usati ;i++){
        //Serial.print(ReadTempGrad (i,modulo_corrente,bms_ic));
        Serial.print(ReadTempGrad (3,0,bms_ic));
        Serial.print(";");
    }  

}

bool Modulo::GetFlag(){
  return flag_error;
}

uint16_t Modulo::GetSoc(){
  return soc;
}

bool Modulo::GetModulocarico(){
    return  modulo_carico;
}


int Modulo::GetN_celle(){
    return n_celle;  
}

int Modulo::GetN_ntc(){
    return n_ntc;
}



