#ifndef ELEMENTI_h
#define ELEMENTI_h
#include "ltc681x.h"

 class Elemento{
    protected:
        bool flag_error;
};

class Ntc : public Elemento{
    private:
        unsigned long tempo;
    public:
        Ntc();
        bool error_check(uint16_t temperatura);
};

class Cella : public Elemento{
    private:
        unsigned long tempo;
    public:
        Cella();
        bool error_check(uint16_t tensione);
        bool carica(uint16_t tensione,cell_asic bms_ic[],uint16_t top_voltage,uint8_t modulo_corrente,uint8_t cella_corrente);

};

class Modulo: public Elemento{

    private:
       uint16_t low_voltage;
       bool modulo_carico;
       Cella** cella;
       Ntc** ntc;
       int n_celle;
       int n_ntc;
    public :
       Modulo(int N_celle,int N_ntc);
       bool error_check(cell_asic bms_ic[],int modulo_corrente);
       bool carica(cell_asic bms_ic[],int modulo_corrente);
       void StampaVoltaggio (cell_asic bms_ic[], int modulo_corrente);   //stampa nel monitor seriale di arduino i volt di tutte le celle del modulo
       void StampaTemp (cell_asic bms_ic[], int modulo_corrente);        //stampa nel monitor seriale di arduino le temperature di tutti gli ntc del modulo

};

class Pacco: public Elemento{

    private:
       Modulo** modulo;
       int n_moduli;
    public :
       cell_asic bms_ic;
       Pacco(int N_moduli,int N_celle,int N_ntc);
       bool error_check(cell_asic bms_ic[]);
       bool carica(cell_asic bms_ic[]);
       void StampaVoltaggio (cell_asic bms_ic[]);  //stampa nel monitor seriale di arduino il volt di tutte le celle del pacco
       void StampaTemp (cell_asic bms_ic[]);         //stampa nel monitor seriale di arduino la temperatura di tutti gli ntc del pacco   
       void StampaDebug(cell_asic bms_ic[], bool InCarica, bool CaricaCompleta);
};
#endif