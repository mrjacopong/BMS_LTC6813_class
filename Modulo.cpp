#include "elementi.h"
#include "libreriafunzioni.h"

Modulo::Modulo(int N_celle,int N_ntc){
    flag_error=false;
    n_celle=N_celle;
    n_ntc=N_ntc;
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
    for (int i=0;i<n_celle;i++){
        //salta la cella  unused_ch_1=9 e unused_ch_2=18
        //queste due celle sono sempre a 0v e non sono un errore
        if ((modulo_corrente== unused_ch_1) || (modulo_corrente == unused_ch_2))
            return false;
        if (cella[i]->error_check(bms_ic[modulo_corrente].cells.c_codes[i])==true)
            return true;
    }
    for (int j=0;j<n_ntc;j++){
       if( ntc[j]->error_check(bms_ic[modulo_corrente].aux.a_codes[j])==true)
            return true;
    }
    return false;
}

bool Modulo::carica(cell_asic bms_ic[]){
    for (int i=0;i<n_celle;i++){
    cella[i]->carica(bms_ic);
    }

}