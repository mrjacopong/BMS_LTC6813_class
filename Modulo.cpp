#include "elementi.h"


Modulo::Modulo(int n_celle,int n_ntc){
    flag_error=false;
    cella= new Cella* [n_celle];
    for (int i=0;i<n_celle;i++){
    cella[i] = new Cella();
    }
    ntc= new Ntc* [n_ntc];
    for (int j=0;j<n_ntc;j++){
    ntc[j] = new Ntc();
    }

}
bool Modulo::error_check(){

}

bool Modulo::carica(){

}
