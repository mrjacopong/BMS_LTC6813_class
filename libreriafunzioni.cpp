#include "libreriafunzioni.h"



uint16_t IsTop(uint16_t top,uint16_t actual){                            //ritorna il valore più grande
  if (top>actual){
    return top;
  }
  return actual;
}

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

void greater_balance(int8_t cella,uint8_t pinOut,cell_asic bms_ic[]){
  open_relay(pinOut);
  set_discharge(cella,bms_ic);
}

void intermediate_balance(int8_t cella,cell_asic bms_ic[]){
  set_discharge(cella,bms_ic);
}


void gpio_measurment(cell_asic bms_ic[]){
  wakeup_sleep(TOTAL_IC);                                             //converte gpio
  ltc6813_adax(ADC_CONVERSION_MODE , AUX_CH_TO_CONVERT);
  ltc6813_pollAdc();
  wakeup_sleep(TOTAL_IC);
  ltc6813_rdaux(0,TOTAL_IC,bms_ic);                                   // questo serve a caso
}


float ReadTempGrad (uint8_t pin,uint8_t current_ic,cell_asic bms_ic[]){    //legge la temperatura in gradi              //solo il pin passato             
  gpio_measurment(bms_ic);                                                  //e l'IC passato
  float Vout = bms_ic[current_ic].aux.a_codes[pin]*0.0001;
  float Vref2=bms_ic[current_ic].aux.a_codes[5]*0.0001;
  float Rntc = ((Resistenza * Vref2) / Vout) - Resistenza;
  float Rdif = Rntc / Resistenza;
  float Peppa = log(Rdif);
  float B = BB1 * Peppa;
  float C = CC1 * (pow(Peppa,2));
  float D = D1 * (pow(Peppa,3));
  float sum = AA1 + B + C + D;
  float Temp = (pow(sum, -1)-274);
  return (Temp);
}
void set_discharge(int8_t cella,cell_asic bms_ic[]){
  ltc6813_set_discharge(cella,TOTAL_IC,bms_ic);
  wakeup_sleep(TOTAL_IC);
  ltc6813_wrcfg(TOTAL_IC,bms_ic);
  ltc6813_wrcfgb(TOTAL_IC,bms_ic);
}

void reset_discharge(cell_asic bms_ic[]){
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

void voltage_measurment(cell_asic bms_ic[]){
  wakeup_sleep(TOTAL_IC);
  ltc6813_adcv(ADC_CONVERSION_MODE, ADC_DCP, CELL_CH_TO_CONVERT);
  uint8_t conv_time = ltc6813_pollAdc();
  /*Serial.print(F("cell conversion completed in:"));
  Serial.print(((float)conv_time / 1000), 1);
  Serial.println(F("mS"));
  Serial.println();*/
  ltc6813_rdcv(0, TOTAL_IC, bms_ic); // Set to read back all cell voltage registers
}