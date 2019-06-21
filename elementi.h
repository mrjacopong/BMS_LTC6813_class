#ifndef ELEMENTI_h
#define ELEMENTI_h
#include "ltc681x.h"

 class Elemento{
    protected: 
        bool flag_error;
    public:
        virtual bool get_flag()=0;
};

class Ntc : public Elemento{
    private:
        unsigned long tempo;
    public:
        Ntc();
        bool error_check(uint16_t temperatura);
        bool get_flag();
        unsigned long get_tempo();
};

class Cella : public Elemento{
    private:
        unsigned long tempo;
        bool flag_inScarica;
    public:
        Cella();
        bool error_check(uint16_t tensione);
        bool carica(uint16_t tensione,cell_asic bms_ic[],uint16_t top_voltage,uint8_t modulo_corrente,uint8_t cella_corrente,unsigned long* tempoIniziale);
        bool get_flag();
        bool get_flagInScarica();
        unsigned long get_tempo();
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
       bool error_check(cell_asic bms_ic[],int modulo_corrente,);
       bool carica(cell_asic bms_ic[],int modulo_corrente,unsigned long *tempoIniziale);
       void StampaVoltaggio (cell_asic bms_ic[], int modulo_corrente);   //stampa nel monitor seriale di arduino i volt di tutte le celle del modulo
       void StampaTemp (cell_asic bms_ic[], int modulo_corrente);        //stampa nel monitor seriale di arduino le temperature di tutti gli ntc del modulo
       bool get_flag();
       uint16_t get_lowvoltage();
       bool get_modulocarico();
       int get_Ncelle();
       int get_Nntc();

};

class Pacco: public Elemento{

    private:
       Modulo** modulo;
       int n_moduli;
       unsigned long tempoIniziale;
    public :
       cell_asic bms_ic;
       Pacco(int N_moduli,int N_celle,int N_ntc);
       bool error_check(cell_asic bms_ic[]);
       bool carica(cell_asic bms_ic[],unsigned long *tempoIniziale);
       void StampaVoltaggio (cell_asic bms_ic[]);  //stampa nel monitor seriale di arduino il volt di tutte le celle del pacco
       void StampaTemp (cell_asic bms_ic[]);         //stampa nel monitor seriale di arduino la temperatura di tutti gli ntc del pacco   
       void StampaDebug(cell_asic bms_ic[], bool InCarica, bool CaricaCompleta);
       bool get_flag();
       int get_nModuli();
};
#endif