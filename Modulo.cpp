#include "elementi.h"

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
bool Modulo::error_check(cell_asic bms_ic[]){
    for (int i=0;i<n_celle;i++){
    cella[i]->error_check(bms_ic);
    }
    for (int j=0;j<n_ntc;j++){
    ntc[j]->error_check(bms_ic);
    }
}

bool Modulo::carica(cell_asic bms_ic[]){

}