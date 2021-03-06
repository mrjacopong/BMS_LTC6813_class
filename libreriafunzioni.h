#ifndef LIBRERIAFUNZIONI_h
#define LIBRERIAFUNZIONI_h


#define led_bilanciamento_pesante 7              //Definisco i pin dei led e del relè
#define led_sistema 4
#define led_carica 5
#define led_errore 3
#define relay_pin 6
#define charge_switch_pin 2
#define balance_switch_pin 9
#define current_sensor 1                         //sulla scheda: current_sensor+1

#include "ltc6813.h"
const unsigned long intervalloBlink = 200;       //Parametri per la stampa

//ADC Command Configurations
const uint8_t ADC_OPT = ADC_OPT_DISABLED;        // See ltc6813_daisy.h for Options
const uint8_t ADC_CONVERSION_MODE = MD_7KHZ_3KHZ;//MD_7KHZ_3KHZ; //MD_26HZ_2KHZ;//MD_7KHZ_3KHZ; // See ltc6813_daisy.h for Options
const uint8_t ADC_DCP = DCP_DISABLED;            // See ltc6813_daisy.h for Options
const uint8_t CELL_CH_TO_CONVERT = CELL_CH_ALL;  // See ltc6813_daisy.h for Options
const uint8_t AUX_CH_TO_CONVERT = AUX_CH_ALL;    // See ltc6813_daisy.h for Options
const uint8_t STAT_CH_TO_CONVERT = STAT_CH_ALL;  // See ltc6813_daisy.h for Options
const uint16_t MEASUREMENT_LOOP_TIME = 500;      //milliseconds(mS)

//Under Voltage and Over Voltage Thresholds
const uint16_t OV_THRESHOLD = 42000;             // Over voltage threshold ADC Code. LSB = 0.0001
const uint16_t UV_THRESHOLD = 30000;             // Under voltage threshold ADC Code. LSB = 0.0001
const uint16_t cutoff_voltage = 25000;           // la tensione più bassa a cui posso arrivare (secondo datasheet vtc5)
const uint16_t MAXTEMP = 60;                     // Over temperature GRADI CENTIGRADI
const double MAXVOLTAGE = OV_THRESHOLD;          // per convertirla in double nella funzione ErrorCheck
const uint16_t OV_TIME_LIMIT=500;                // limite di tempo in millisecondi OV
const uint16_t OT_TIME_LIMIT=1000;               // limite di tempo in millisecondi OT

const uint8_t TOTAL_IC = 1;                      //!<number of ICs in the daisy chain
const uint8_t TOTAL_CH = 18;                     // number of channel used per ADC
const uint8_t TOTAL_NTC = 8;                     // number of temperatures per ADC
const uint8_t unused_ch_1=13-1;                  // celle non usate (13esima cella - 1 perchè l'array parte da 0)
const uint8_t unused_ch_2=18-1;                  // celle non nusate(18esima cella - 1 perchè l'array parte da 0)
const uint8_t celle_usate=16;
const uint8_t ntc_usati=1;

//per algoritmo di carica
const uint16_t delta_carica = 1000;              // massima differenza tra due batterie (0.1v)
const uint16_t delta_bilanciamento=100;          // massima differenza tra due batterie nell'algoritmo di bilanciamento (non in carica)(0.01v)
const uint16_t delta_carica_finale = 400;        // massima differenza tra due batterie nel bilanciamento finale (0.04v)
const uint16_t soglia_carica=41000;              // soglia tensione carica (4.1V)
const uint16_t bilanciamento_off=300;            // differenza di tensione quando il bilanciamento viene fermato
const uint16_t semi_THRESHOLD=OV_THRESHOLD - 100; // poco prima della tensione di errore per evitare di intrare in ShoutdownError durante la fine
//questo valore viene controllato nel loop in modo tale che sia modificabile 
//dinamicamente ogni volta che avviene il loop
/*--variabili per ntc--*/
const float AA1 = 3.354016 * pow(10, -3);
const float BB1 = 2.569850 * pow(10, -4);
const float CC1 = 2.620131 * pow(10, -6);
const float D1 = 6.383091 * pow(10, -8);
const float Resistenza = 10000;



/*Funzioni */

/* return the highter number (top>actual) */
uint16_t IsTop(uint16_t top,uint16_t actual);

/* return the lower number (top<actual) */
uint16_t IsLow(uint16_t low,uint16_t actual);

/* read temperature in grad from a specific gpio pin */
float ReadTempGrad (uint8_t pin,uint8_t current_ic,cell_asic bms_ic[]);

/* read all the voltage from every adc */
void VoltageMeasurment(cell_asic bms_ic[]);

/* error routine */
void ShoutdownError(cell_asic bms_ic[],uint8_t modulo_corrente,uint8_t cella_corrente);

/* undervoltage error routine */
void  UnderVoltageShoutdown(uint8_t modulo_corrente,uint8_t cella_corrente);

/* return true if it is passed a slot of time from t_inizio to durata_max */
bool TimeCheck(unsigned long t_inizio ,uint16_t durata_max );

/* initialize pinout of the arduino */
void InitPinOut();

/* open the charge relay and return true (stop charge routine) */
bool StopCharge(uint8_t pin_out);

/* stop balance resetting all the discharge pin */
bool StopBilanciamento(cell_asic bms_ic[]);

/* activate discharge resistor of "cella" during the final stage of the charge (with finer thresholds) */
bool FinalBalance(uint16_t Low_voltage,uint16_t tensione,cell_asic bms_ic[],int8_t modulo,int8_t cella,unsigned long* tempo_iniziale);

/* activate discharge resistor of "cella" when the voltages are very different */
void GreaterBalance(uint16_t tensoine_iniziale,cell_asic bms_ic[],uint8_t modulo,uint8_t cella);

/* activate discharge resistor of "cella" (resistors starts from 0) */
void IntermediateBalance(int8_t cella,cell_asic bms_ic[]);

/* read voltages from gpio */
void GpioMeasurment(cell_asic bms_ic[]);

/* read temperature in grad from gpio */
float ReadTempGrad (uint8_t pin,uint8_t current_ic);

/* activate discharge resistor of "cella" (resistors starts from 1) */
void SetDischarge(int8_t cella,cell_asic bms_ic[]);

/* refresh discharge signals to not be resetted by watchdog */
void RefreshDischarge(cell_asic bms_ic[]);

/* reset discharge signals to be off in every module*/
void ResetDischarge(cell_asic bms_ic[]);

/* open the relay at the arduino pin "relay" */
void OpenRelay(uint8_t relay);

/* close the relay at the arduino pin "relay" */
void CloseRelay(uint8_t relay);

/* print the header of the table in the serial monitor */
void StampaHeaderTabella();

/* print a string with a message, useful to manage errors*/
void StampaStringaErrore(cell_asic bms_ic[],String messaggio);

/* turn on the led at the arduino pin "pin" */
void AccendiLed(int pin);

/* turn off the led at the arduino pin "pin" */
void SpegniLed(int pin);

/* let the led at the arduino pin "pin" to blink, counting time passed from "last_millis_led" */
unsigned long Blink(int pin,unsigned long last_millis_led);

/* reads current value from a sensor at the gpio pin "pin" of the module "current_ic" (using ltc6813)
   needs to be implemented */
float ReadCurrent(uint8_t pin,uint8_t current_ic,cell_asic bms_ic[]);






#endif  