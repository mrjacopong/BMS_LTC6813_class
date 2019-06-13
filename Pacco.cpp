#include "elementi.h"


Pacco::Pacco(int n_moduli,int n_celle,int n_ntc){
    flag_error=false;
    modulo= new Modulo* [n_moduli];
    for (int i=0;i<n_moduli;i++){
    modulo[i] = new Modulo(n_celle,n_ntc);
    }
 

}
bool Pacco::error_check(){

}

bool Pacco::carica(){

}