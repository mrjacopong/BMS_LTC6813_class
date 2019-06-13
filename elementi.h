#ifndef ELEMENTI_h
#define ELEMENTI_h
class Elemento{
    protected:
        bool flag_error;
    public:
        virtual bool error_check()=0;
};

class Ntc : public Elemento{
    protected:
        unsigned long tempo;
    public:
        Ntc();
        bool error_check();
};

class Elemento_circuitale: public Elemento{
    public:
        virtual bool carica()=0;
};

class Cella : public Elemento_circuitale{
    protected:
        unsigned long tempo;
    public:
        Cella();
        bool error_check();
        bool carica();

};

class Modulo: public Elemento_circuitale{

    protected:
       Cella** cella;
       Ntc** ntc;
       int n_celle;
       int n_ntc;
    public :
       Modulo(int N_celle,int N_ntc){};
       bool error_check(){};
       bool carica(){};
};

class Pacco: public Elemento_circuitale{

    protected:
       Modulo** modulo;
       int n_moduli;
    public :
       Pacco(int N_moduli,int N_celle,int N_ntc){};
       bool error_check(){};
       bool carica(){};
};
#endif