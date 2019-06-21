#include "elementi.h"
#include "libreriafunzioni.h"

Pacco::Pacco(int N_moduli,int N_celle,int N_ntc){
    flag_error=false;
    n_moduli=N_moduli;
    modulo= new Modulo* [N_moduli];
    for (int i=0;i<N_moduli;i++){
    modulo[i] = new Modulo(N_celle,N_ntc);
    }
    cell_asic bms_ic;

}
bool Pacco::error_check(cell_asic bms_ic[]){ 
    for (int i=0;i<n_moduli;i++){ //controllo errore in pacco
        if(modulo[i]->error_check(bms_ic,i)==true)
            return true;
    }
    return false;
}   

bool Pacco::carica(cell_asic bms_ic[]){
    for (int i=0;i<n_moduli;i++){
        if(modulo[i]->carica(bms_ic,i)) //true se carica
            stop_charge(RelayPin);      //stoppa la carica del modulo in queestione
    }
}

void Pacco::StampaVoltaggio (cell_asic bms_ic[]){  //stampa nel monitor seriale di arduino
    for (int i=0;i<n_moduli;i++){
            Serial.print("modulo");
            Serial.print(i);
            Serial.println(":");
            modulo[i]->StampaVoltaggio (bms_ic,i);
    }
}

void Pacco::StampaTemp (cell_asic bms_ic[]){   //stampa nel monitor seriale di arduino 
    for (int i=0;i<n_moduli;i++){
            Serial.print("modulo");
            Serial.print(i);
            Serial.println(":");
            modulo[i]->StampaTemp (bms_ic,i);
    }
}

void Pacco::StampaDebug(cell_asic bms_ic[], bool InCarica, bool CaricaCompletata){
    Serial.print(millis());
    Serial.print(";");
    for(int i=0; i<TOTAL_CH ;i++){
        if(i!=unused_ch_1 && i!=unused_ch_2){
            Serial.print(bms_ic[0].cells.c_codes[i]*0.0001,4);
            Serial.print(";");
        }
    }
    for(int i=0; i<NtcUsati ;i++){
            Serial.print(ReadTempGrad (3,0,bms_ic));
            Serial.print(";");
    }  
    if(InCarica)
        Serial.print("Si");
    else 
        Serial.print("No");

    Serial.print(";");
    if(CaricaCompletata)
        Serial.print("Si");
    else 
        Serial.print("No");

    Serial.print(";");

    Serial.println();
}

bool Pacco::get_flag(){
  return flag_error;
}

int Pacco::get_nModuli(){
    return n_moduli;
}

