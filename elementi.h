#ifndef ELEMENTI_h
#define ELEMENTI_h
#include "ltc681x.h"

 class Elemento{
    protected: 
        bool flag_error;
    public:
        virtual bool GetFlag()=0;
};

class Ntc : public Elemento{
    private:
        unsigned long tempo;
    public:
        Ntc();
        bool ErrorCheck(cell_asic bms_ic[],uint16_t temperatura,uint8_t current_IC,uint8_t current_NTC);
        bool GetFlag();
        unsigned long GetTempo();
};

class Cella : public Elemento{
    private:
        unsigned long tempo;
        bool flag_in_scarica;
        uint16_t soc;
    public:
        Cella();
        bool ErrorCheck(cell_asic bms_ic[],uint16_t tensione,uint8_t current_IC,uint8_t current_CELL);
        bool carica(uint16_t tensione,cell_asic bms_ic[],uint16_t top_voltage,uint8_t modulo_corrente,uint8_t cella_corrente,unsigned long* tempo_iniziale);
        bool Bilancia(uint16_t tensione,cell_asic bms_ic[],uint16_t top_voltage,uint8_t modulo_corrente,uint8_t cella_corrente);
        bool GetFlag();
        unsigned long GetTempo();
        uint16_t Soc();
        uint16_t GetSoc();
};

class Modulo: public Elemento{

    private:
       bool modulo_carico;
       bool modulo_bilanciato;
       unsigned long tempo_iniziale;
       Cella** cella;
       Ntc** ntc;
       int n_celle;
       int n_ntc;
       uint16_t soc;
    public :
       Modulo(int N_celle,int N_ntc);
       bool ErrorCheck(cell_asic bms_ic[],int modulo_corrente);
       bool carica(cell_asic bms_ic[],int modulo_corrente,uint16_t* low_voltage);
       bool Bilancia(cell_asic bms_ic[],int modulo_corrente,uint16_t* low_voltage);
       void StampaVoltaggio (cell_asic bms_ic[], int modulo_corrente);   //stampa nel monitor seriale di arduino i volt di tutte le celle del modulo
       void StampaTemp (cell_asic bms_ic[], int modulo_corrente);        //stampa nel monitor seriale di arduino le temperature di tutti gli ntc del modulo
       void StampaDebug (cell_asic bms_ic[], int modulo_corrente);
       bool GetFlag();
       bool GetModulocarico();
       int GetN_celle();
       int GetN_ntc();
       uint16_t Soc();
       uint16_t GetSoc();

};

class Pacco: public Elemento{

    private:
       uint16_t low_voltage;
       Modulo** modulo;
       int n_moduli;
       uint16_t soc;
    public :
       Pacco(int N_moduli,int N_celle,int N_ntc,cell_asic bms_ic[]);
       bool ErrorCheck(cell_asic bms_ic[]);
       bool carica(cell_asic bms_ic[]);
       bool Bilancia(cell_asic bms_ic[]);            //quando volgiamo bilanciare il pacco separatamente dalla carica
       void StampaVoltaggio (cell_asic bms_ic[]);    //stampa nel monitor seriale di arduino il volt di tutte le celle del pacco
       void StampaTemp (cell_asic bms_ic[]);         //stampa nel monitor seriale di arduino la temperatura di tutti gli ntc del pacco   
       void StampaDebug (String messagge,cell_asic bms_ic[]);
       bool GetFlag();
       int GetN_moduli();
       uint16_t GetLowVoltage();
       uint16_t Soc();
       uint16_t GetSoc();
};
#endif