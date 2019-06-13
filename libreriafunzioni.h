#ifndef LIBRERIAFUNZIONI_h
#define LIBRERIAFUNZIONI_h
#include <cmath>
#include <stdint.h>
#include "ltc6813.h"

const uint8_t ADC_CONVERSION_MODE = MD_7KHZ_3KHZ;//MD_7KHZ_3KHZ; //MD_26HZ_2KHZ;//MD_7KHZ_3KHZ; // See ltc6813_daisy.h for Options
const uint8_t AUX_CH_TO_CONVERT = AUX_CH_ALL; // See ltc6813_daisy.h for Options

//Under Voltage and Over Voltage Thresholds
const uint16_t OV_THRESHOLD = 41000; // Over voltage threshold ADC Code. LSB = 0.0001
const uint16_t UV_THRESHOLD = 30000; // Under voltage threshold ADC Code. LSB = 0.0001
const uint16_t MAXTEMP = 60;         // Over temperature GRADI CENTIGRADI
double MAXVOLTAGE = OV_THRESHOLD;    // per convertirla in double nella funzione error_check
const uint16_t OV_TIME_LIMIT=500;    // limite di tempo in millisecondi OV
const uint16_t OT_TIME_LIMIT=1000;    // limite di tempo in millisecondi OT

const uint8_t TOTAL_IC = 1;//!<number of ICs in the daisy chain
const uint8_t TOTAL_CH = 18; // number of channel used per ADC
const uint8_t TOTAL_NTC = 8; // number of temperatures per ADC
const uint8_t unused_ch_1=9;//celle non usate
const uint8_t unused_ch_2=18;//celle no nusate

//per algoritmo di carica
const uint16_t delta_carica = 2000;  // massima differenza tra due batterie in serie 
const uint16_t SogliaCarica=41000;   // soglia tensione carica (4.0V)
bool in_carica=true;                 // true->in carica ; false->non in carica;
bool IsCharged=false;                //serve per controllare che stiamo caricando
//questo valore viene controllato nel loop in modo tale che sia modificabile 
//dinamicamente ogni volta che avviene il loop
const uint8_t RelayPin=10;           //pin del relay da aprire in caso di errore
const uint8_t ChargeSwitchPin=11;         //pin dello switch per avviare la carica
/*--variabili per ntc--*/
float AA1 = 3.354016 * pow(10, -3);
float BB1 = 2.569850 * pow(10, -4);
float CC1 = 2.620131 * pow(10, -6);
float D1 = 6.383091 * pow(10, -8);
float Resistenza = 10000;



/*Funzioni */
void shoutdown_error(uint8_t pinOut);
bool time_check(unsigned long t_inizio ,uint16_t durata_max );
void init_pinout();
bool stop_charge(uint8_t pinOut);
void greater_balance(int8_t cella,uint8_t pinOut);
void intermediate_balance(int8_t cella);
void gpio_measurment();
float ReadTempGrad (uint8_t pin,uint8_t current_ic);
void set_discharge(int8_t cella);
void reset_discharge();
void open_relay(uint8_t relay);
void close_relay(uint8_t relay);







#endif  