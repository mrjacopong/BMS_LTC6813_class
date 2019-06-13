#include "elementi.h"


Pacco::Pacco(int N_moduli,int N_celle,int N_ntc){
    flag_error=false;
    modulo= new Modulo* [N_moduli];
    for (int i=0;i<N_moduli;i++){
    modulo[i] = new Modulo(N_celle,N_ntc);
    }
 

}
bool Pacco::error_check(){

}

bool Pacco::carica(){

}