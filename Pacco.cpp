#include "elementi.h"
#include "libreriafunzioni.h"

Pacco::Pacco(int N_moduli,int N_celle,int N_ntc){
    flag_error=false;
    low_voltage=60000;
    n_moduli=N_moduli;
    modulo= new Modulo* [N_moduli];
    for (int i=0;i<N_moduli;i++){
    modulo[i] = new Modulo(N_celle,N_ntc);
    }
    cell_asic bms_ic;
    //soc=soc();
}
bool Pacco::ErrorCheck(cell_asic bms_ic[]){ 
    for (int i=0;i<n_moduli;i++){                          //controllo errore in pacco
        if(modulo[i]->ErrorCheck(bms_ic,i)==true)
            return true;
    }
    return false;
}   

bool Pacco::carica(cell_asic bms_ic[]){
    for (int i=0;i<n_moduli;i++){
        if(modulo[i]->carica(bms_ic,i,&low_voltage)){                   //true se carica
            //pin da vedere il pin quale è
            if(ReadCurrent(current_sensor,0,bms_ic)<0.05){ //controlla che la corrente sia abbastanza bassa
                StopCharge(relay_pin);                     //stoppa la carica del modulo in queestione
                return true;
            }
        }
    }
    return false;
}

bool Pacco::Bilancia(cell_asic bms_ic[]){
    for (int i=0;i<n_moduli;i++){
        if(modulo[i]->Bilancia(bms_ic,i,&low_voltage)){                 //true se ha finito il bilanciamento
            StopBilanciamento(bms_ic);
            return true; 
        }
    }
    return false;
}

uint16_t Pacco::Soc(){

    for (int i=0;i<n_moduli;i++){
        soc+=modulo[i]->Soc();
    }
    return soc/n_moduli;
}

void Pacco::StampaVoltaggio (cell_asic bms_ic[]){          //stampa nel monitor seriale di arduino
    for (int i=0;i<n_moduli;i++){
            Serial.print("modulo");
            Serial.print(i);
            Serial.println(":");
            modulo[i]->StampaVoltaggio (bms_ic,i);
    }
}

void Pacco::StampaTemp (cell_asic bms_ic[]){               //stampa nel monitor seriale di arduino 
    for (int i=0;i<n_moduli;i++){
            Serial.print("modulo");
            Serial.print(i);
            Serial.println(":");
            modulo[i]->StampaTemp (bms_ic,i);
    }
}

void Pacco::StampaDebug(cell_asic bms_ic[], bool in_carica, bool carica_completata){
    Serial.print(millis());
    Serial.print(";");
    for(int i=0; i<TOTAL_CH ;i++){
        if(i!=unused_ch_1 && i!=unused_ch_2){
            Serial.print(bms_ic[0].cells.c_codes[i]*0.0001,4);
            Serial.print(";");
        }
    }

    Serial.print(ReadTempGrad (3,0,bms_ic));
    Serial.print(";");

    for(int i=0; i<ntc_usati ;i++){
            Serial.print(ReadCurrent(current_sensor,0,bms_ic));
            Serial.print(";");
    }  
    if(in_carica)
        Serial.print("Si");
    else 
        Serial.print("No");

    Serial.print(";");
    if(carica_completata)
        Serial.print("Si");
    else 
        Serial.print("No");

    Serial.println();
}

bool Pacco::GetFlag(){
  return flag_error;
}

uint16_t Pacco::GetSoc(){
  return soc;
}

int Pacco::GetN_moduli(){
    return n_moduli;
}

uint16_t Pacco::GetLowVoltage(){
    return low_voltage;
}