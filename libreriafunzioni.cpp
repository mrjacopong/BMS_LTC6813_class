#include "libreriafunzioni.h"
#include <Arduino.h>

void shoutdown_error(uint8_t pinOut){
  open_relay(pinOut);
}
bool time_check(unsigned long t_inizio ,uint16_t durata_max ){        //true se l'errore persiste
  /*controllo della durata dell'errore*/
  if (millis()-t_inizio>durata_max){
    return true;
  }
  return false;
}

void init_pinout(){ //inizzializza il pinout per l'arduino
  pinMode(RelayPin,OUTPUT);
  pinMode(ChargeSwitchPin,INPUT);
}
bool stop_charge(uint8_t pinOut){  //ferma la carica 
  open_relay(pinOut);
  return true;
}

void greater_balance(int8_t cella,uint8_t pinOut){
  open_relay(pinOut);
  set_discharge(cella);
}

void intermediate_balance(int8_t cella){
  set_discharge(cella);
}

void gpio_measurment(){
  wakeup_sleep(TOTAL_IC);                                             //converte gpio
  ltc6813_adax(ADC_CONVERSION_MODE , AUX_CH_TO_CONVERT);
  ltc6813_pollAdc();
  wakeup_sleep(TOTAL_IC);
  ltc6813_rdaux(0,TOTAL_IC,bms_ic);                                   // questo serve a caso
}


float ReadTempGrad (uint8_t pin,uint8_t current_ic){    //legge la temperatura in gradi              //solo il pin passato             
  gpio_measurment();                                                  //e l'IC passato
  float Vout = bms_ic[current_ic].aux.a_codes[pin]*0.0001;
  float Vref2=bms_ic[current_ic].aux.a_codes[5]*0.0001;
  float Rntc = ((Resistenza * Vref2) / Vout) - Resistenza;
  float Rdif = Rntc / Resistenza;
  float Peppa = log(Rdif);
  float B = BB1 * Peppa;
  float C = CC1 * (pow(Peppa,2));
  float D = D1 * (pow(Peppa,3));
  float sum = AA1 + B + C + D;
  /*--un po debug--*/
  Serial.print("Rntc: ");
  Serial.println(Rntc);
  Serial.print("Vref2: ");
  Serial.println(Vref2,4);
  Serial.print("Vout: ");
  Serial.println(Vout,4);
  float Temp = (pow(sum, -1)-274);
  return (Temp);
}
void set_discharge(int8_t cella){
  ltc6813_set_discharge(cella,TOTAL_IC,bms_ic);
  wakeup_sleep(TOTAL_IC);
  ltc6813_wrcfg(TOTAL_IC,bms_ic);
  ltc6813_wrcfgb(TOTAL_IC,bms_ic);
}

void reset_discharge(){
  clear_discharge(TOTAL_IC,bms_ic);
  wakeup_sleep(TOTAL_IC);
  ltc6813_wrcfg(TOTAL_IC,bms_ic);
  ltc6813_wrcfgb(TOTAL_IC,bms_ic);
}

void open_relay(uint8_t relay){
  digitalWrite(relay, HIGH);
}

void close_relay(uint8_t relay){
  digitalWrite(relay, LOW);
}