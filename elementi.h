#ifndef ELEMENTI_h
#define ELEMENTI_h
#include "ltc681x.h"

class Elemento{
    protected:
        bool flag_error;
    public:
        //virtual bool error_check()=0;
};

class Ntc : public Elemento{
    protected:
        unsigned long tempo;
    public:
        Ntc();
        bool error_check(uint16_t temperatura);
};

class Elemento_circuitale: public Elemento{
    public:
        virtual bool carica(cell_asic bms_ic[])=0;
};

class Cella : public Elemento_circuitale{
    protected:
        unsigned long tempo;
    public:
        Cella();
        bool error_check(uint16_t tensione);
        bool carica(cell_asic bms_ic[]);

};

class Modulo: public Elemento_circuitale{

    protected:
       Cella** cella;
       Ntc** ntc;
       int n_celle;
       int n_ntc;
    public :
       Modulo(int N_celle,int N_ntc);
       bool error_check(cell_asic bms_ic[],int modulo_corrente);
       bool carica(cell_asic bms_ic[]);
};

class Pacco: public Elemento_circuitale{

    protected:
       Modulo** modulo;
       int n_moduli;
    public :
       cell_asic bms_ic;
       Pacco(int N_moduli,int N_celle,int N_ntc);
       bool error_check(cell_asic bms_ic[]);
       bool carica(cell_asic bms_ic[]);
};
#endif