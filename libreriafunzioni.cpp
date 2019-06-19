#include "libreriafunzioni.h"



uint16_t IsTop(uint16_t top,uint16_t actual){                            //ritorna il valore più grande
  if (top>actual){
    return top;
  }
  return actual;
}
uint16_t IsLow(uint16_t low,uint16_t actual){                            //ritorna il valore più grande
  if (low<actual){
    return low;
  }
  return actual;
}

void shoutdown_error(){
  open_relay(RelayPin);
  AccendiLed(LedErrore);
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
  digitalWrite(RelayPin, HIGH);
  pinMode(ChargeSwitchPin,INPUT);
  pinMode(LedErrore,OUTPUT);
  pinMode(LedSistema,OUTPUT);
  pinMode(LedCarica,OUTPUT);
  pinMode(LedBilanciamentoPesante,OUTPUT);
}
bool stop_charge(uint8_t pinOut){  //ferma la carica 
  open_relay(pinOut);
  return true;
}

void final_balance(uint16_t Low_voltage, uint16_t tensione,uint8_t pinOut,cell_asic bms_ic[],int8_t modulo,int8_t cella){
  if (tensione >= SogliaCarica + 500)
    intermediate_balance(cella,bms_ic);
  if (tensione >= SogliaCarica + 900){
    greater_balance(Low_voltage,bms_ic,modulo,cella);
  }
  /*voglio che se la batteria sia a 4,05V va bene
  se arriva a 4.09V bilancio maggiormente perchè non volgio arrivare a 4.1v */
}

void greater_balance(uint16_t Low_voltage,cell_asic bms_ic[],uint8_t modulo,uint8_t cella){
  open_relay(RelayPin);
  AccendiLed(LedBilanciamentoPesante);
  unsigned long TempoPrec=0;
  while (Low_voltage - bms_ic[modulo].cells.c_codes[cella] >= delta_carica && digitalRead(ChargeSwitchPin)){
    set_discharge(cella+1,bms_ic);
    voltage_measurment(bms_ic);
    
    if(millis()-TempoPrec>3000){//lo rimetto qui altrimenti nel whilw non stampa più
      TempoPrec=millis();
      StampaDebug2(bms_ic, false, true);
    }
  delay(100);
  }//finquando la tensione attuale non si abbassa di un delta definito da noi non esce dal ciclo
  reset_discharge(bms_ic);
  close_relay(RelayPin);
  SpegniLed(LedBilanciamentoPesante);
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
  /*da testare quando acremo più moduli,non attiva ora perchè bisognerebbe modificare alcune librerie */
  //ltc6813_set_discharge(cella,modulo,bms_ic);
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
  digitalWrite(relay, LOW);
}

void close_relay(uint8_t relay){
  digitalWrite(relay, HIGH);
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

void StampaHeaderTabella(){
  Serial.print("Tempo;");
  for (int i=0; i<CelleUsate; i++){
    Serial.print("cella ");
    Serial.print(i);
    Serial.print(";");
  }
  for (int i=0; i<NtcUsati; i++){
    Serial.print("ntc ");
    Serial.print(i);
    Serial.print(";");
  }
  Serial.print("in carica;");
  Serial.print("carica completata");
  Serial.println();
}

void AccendiLed(int Pin){
    digitalWrite(Pin,HIGH);    
}

void SpegniLed(int Pin){
    digitalWrite(Pin,LOW);
}

unsigned long Blink(int Pin,unsigned long LastMillisLed){
  if(millis() - LastMillisLed > intervalloBlink) {
    LastMillisLed = millis(); //save the last time you blinked the LED
    //if the LED is off turn it on and vice-versa:
    digitalWrite(Pin, !digitalRead(Pin));
  }
  return LastMillisLed;
}

void StampaDebug2(cell_asic bms_ic[], bool InCarica, bool CaricaCompletata){
    Serial.print(millis());
    Serial.print(";");
    for(int i=0; i<TOTAL_CH ;i++){
        if(i!=unused_ch_1 && i!=unused_ch_2){
            Serial.print(bms_ic[0].cells.c_codes[i]*0.0001,4);
            Serial.print(";");
        }
    }
     for(int i=0; i<NtcUsati ;i++){
            Serial.print(ReadTempGrad (3,0,bms_ic));
            Serial.print(";");
    }
    if(InCarica)
        Serial.print("Si");
    else 
        Serial.print("No");

    Serial.print(";");
    if(CaricaCompletata)
        Serial.print("Si");
    else 
        Serial.print("No");

    Serial.print(";");

    Serial.println();
}