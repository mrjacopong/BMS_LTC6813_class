#include "elementi.h"
#include "libreriafunzioni.h"

Modulo::Modulo(int N_celle,int N_ntc){
    flag_error=false;
    top_voltage=0;
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

bool Modulo::carica(cell_asic bms_ic[],int modulo_corrente){
    modulo_carico=true;           //diventa false se c'è almeno una cella scarcia
    for (int i=0;i<n_celle;i++){
        top_voltage=IsTop(top_voltage,bms_ic[modulo_corrente].cells.c_codes[i]);
        if ((i== unused_ch_1)||(i== unused_ch_2)) {}
        else{
            if (!cella[i]->carica(bms_ic[modulo_corrente].cells.c_codes[i],bms_ic,top_voltage,modulo_corrente,i))
                modulo_carico=false;  //se c'è almeno una cella scarica vuol dire che il modulo non è carico 
        }
    }
    return modulo_carico;
}