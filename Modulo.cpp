#include "elementi.h"


Modulo::Modulo(int N_celle,int N_ntc){
    flag_error=false;
    cella= new Cella* [N_celle];
    for (int i=0;i<N_celle;i++){
    cella[i] = new Cella();
    }
    ntc= new Ntc* [N_ntc];
    for (int j=0;j<N_ntc;j++){
    ntc[j] = new Ntc();
    }

}
bool Modulo::error_check(){

}

bool Modulo::carica(){

}
