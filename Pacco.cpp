#include "elementi.h"


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
    for (int i=0;i<n_moduli;i++){
        if(modulo[i]->error_check(bms_ic,i)==true)
            return true;
    }
    return false;
}

bool Pacco::carica(cell_asic bms_ic[]){
    for (int i=0;i<n_moduli;i++){
    modulo[i]->carica(bms_ic,i);
    }

}