#ifndef LIBRERIAFUNZIONI_h
#define LIBRERIAFUNZIONI_h


#define LedBilanciamentoPesante 7           //Definisco i pin dei led e del relè
#define LedSistema 4
#define LedCarica 5
#define LedErrore 3
#define RelayPin 6
#define ChargeSwitchPin 2

#include "ltc6813.h"
const unsigned long intervalloBlink = 200;  //Parametri per la stampa 

//ADC Command Configurations
const uint8_t ADC_OPT = ADC_OPT_DISABLED; // See ltc6813_daisy.h for Options
const uint8_t ADC_CONVERSION_MODE = MD_7KHZ_3KHZ;//MD_7KHZ_3KHZ; //MD_26HZ_2KHZ;//MD_7KHZ_3KHZ; // See ltc6813_daisy.h for Options
const uint8_t ADC_DCP = DCP_DISABLED; // See ltc6813_daisy.h for Options
const uint8_t CELL_CH_TO_CONVERT = CELL_CH_ALL; // See ltc6813_daisy.h for Options
const uint8_t AUX_CH_TO_CONVERT = AUX_CH_ALL; // See ltc6813_daisy.h for Options
const uint8_t STAT_CH_TO_CONVERT = STAT_CH_ALL; // See ltc6813_daisy.h for Options
const uint16_t MEASUREMENT_LOOP_TIME = 500;//milliseconds(mS)

//Under Voltage and Over Voltage Thresholds
const uint16_t OV_THRESHOLD = 41000; // Over voltage threshold ADC Code. LSB = 0.0001
const uint16_t UV_THRESHOLD = 30000; // Under voltage threshold ADC Code. LSB = 0.0001
const uint16_t MAXTEMP = 60;         // Over temperature GRADI CENTIGRADI
const double MAXVOLTAGE = OV_THRESHOLD;    // per convertirla in double nella funzione error_check
const uint16_t OV_TIME_LIMIT=500;    // limite di tempo in millisecondi OV
const uint16_t OT_TIME_LIMIT=1000;    // limite di tempo in millisecondi OT

const uint8_t TOTAL_IC = 1;//!<number of ICs in the daisy chain
const uint8_t TOTAL_CH = 18; // number of channel used per ADC
const uint8_t TOTAL_NTC = 8; // number of temperatures per ADC
const uint8_t unused_ch_1=13-1;//celle non usate
const uint8_t unused_ch_2=18-1;//celle non nusate
const uint8_t CelleUsate=16;
const uint8_t NtcUsati=1;

//per algoritmo di carica
const uint16_t delta_carica = 2000;  // massima differenza tra due batterie in serie 
const uint16_t SogliaCarica=41000;   // soglia tensione carica (4.0V)
//questo valore viene controllato nel loop in modo tale che sia modificabile 
//dinamicamente ogni volta che avviene il loop
/*--variabili per ntc--*/
const float AA1 = 3.354016 * pow(10, -3);
const float BB1 = 2.569850 * pow(10, -4);
const float CC1 = 2.620131 * pow(10, -6);
const float D1 = 6.383091 * pow(10, -8);
const float Resistenza = 10000;



/*Funzioni */
uint16_t IsTop(uint16_t top,uint16_t actual);
uint16_t IsLow(uint16_t low,uint16_t actual);
float ReadTempGrad (uint8_t pin,uint8_t current_ic,cell_asic bms_ic[]);
void voltage_measurment(cell_asic bms_ic[]);
void shoutdown_error();
bool time_check(unsigned long t_inizio ,uint16_t durata_max );
void init_pinout();
bool stop_charge(uint8_t pinOut);
void final_balance(uint16_t Low_voltage,uint16_t tensione,uint8_t pinOut,cell_asic bms_ic[],int8_t modulo,int8_t cella);
void greater_balance(uint16_t tensoine_iniziale,cell_asic bms_ic[],uint8_t modulo,uint8_t cella);
void intermediate_balance(int8_t cella,cell_asic bms_ic[]);
void gpio_measurment(cell_asic bms_ic[]);
float ReadTempGrad (uint8_t pin,uint8_t current_ic);
void set_discharge(int8_t cella,cell_asic bms_ic[]);
void reset_discharge(cell_asic bms_ic[]);
void open_relay(uint8_t relay);
void close_relay(uint8_t relay);
void StampaHeaderTabella();
void AccendiLed(int Pin);
void SpegniLed(int Pin);
unsigned long Blink(int Pin,unsigned long LastMillisLed);
void StampaDebug2(cell_asic bms_ic[], bool InCarica, bool CaricaCompletata);






#endif  