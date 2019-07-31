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

void ShoutdownError(){
  //ResetDischarge(bms_ic);
  OpenRelay(relay_pin);
  AccendiLed(led_errore);
  SpegniLed(led_carica);
  SpegniLed(led_bilanciamento_pesante);
  Serial.println("errore ");
}
void  UnderVoltageShoutdown(){  //scollega il carico 
  AccendiLed(led_errore);//momentaneo
  OpenRelay(relay_pin);
  SpegniLed(led_carica);
  SpegniLed(led_bilanciamento_pesante);
}
bool TimeCheck(unsigned long t_inizio ,uint16_t durata_max ){        //true se l'errore persiste
  /*controllo della durata dell'errore*/
  if (millis()-t_inizio>durata_max){
    return true;
  }
  return false;
}

void InitPinOut(){ //inizzializza il pinout per l'arduino
  pinMode(relay_pin,OUTPUT);
  digitalWrite(relay_pin, HIGH);
  pinMode(charge_switch_pin,INPUT);
  pinMode(led_errore,OUTPUT);
  pinMode(led_sistema,OUTPUT);
  pinMode(led_carica,OUTPUT);
  pinMode(led_bilanciamento_pesante,OUTPUT);
  pinMode(balance_switch_pin,INPUT);
}
bool StopCharge(uint8_t pin_out){  //ferma la carica 
  OpenRelay(pin_out);
  return true;
}

bool StopBilanciamento(cell_asic bms_ic[]){  //ferma il bilanciamento
  ResetDischarge(bms_ic);
  Serial.println("bilanciamento completato");
  return true;
}

bool FinalBalance(uint16_t Low_voltage, uint16_t tensione,cell_asic bms_ic[],int8_t modulo,int8_t cella,unsigned long* tempo_iniziale){
  //ritorna se sta scaricando o meno
  
  if (tensione-Low_voltage >= delta_carica_finale){
    IntermediateBalance(cella,bms_ic);
    return true;
  }
  
  if (tensione >= 41900){          //tensione poco prima dell'errore
    GreaterBalance(Low_voltage,bms_ic,modulo,cella);
    *tempo_iniziale=millis();
    Serial.println("GreaterBalance dentro final");
    Serial.print(tensione);
    Serial.println(" cella numero:");
    Serial.println(cella);
    Serial.println();
    return true;
  }
  return false;
  /*voglio che se la batteria sia a 4,15V va bene
  se arriva a 4.19V bilancio maggiormente perchè non volgio arrivare a 4.2v */
}

void GreaterBalance(uint16_t Low_voltage,cell_asic bms_ic[],uint8_t modulo,uint8_t cella){
  OpenRelay(relay_pin);
  AccendiLed(led_bilanciamento_pesante);
  SetDischarge(cella+1,bms_ic);
}


void IntermediateBalance(int8_t cella,cell_asic bms_ic[]){
  SetDischarge(cella+1,bms_ic);
}


void GpioMeasurment(cell_asic bms_ic[]){
  wakeup_sleep(TOTAL_IC);                                             //converte gpio
  ltc6813_adax(ADC_CONVERSION_MODE , AUX_CH_TO_CONVERT);
  ltc6813_pollAdc();
  wakeup_sleep(TOTAL_IC);
  ltc6813_rdaux(0,TOTAL_IC,bms_ic);                             
}


float ReadTempGrad (uint8_t pin,uint8_t current_ic,cell_asic bms_ic[]){    //legge la temperatura in gradi              //solo il pin passato             
  GpioMeasurment(bms_ic);                                                 //e l'IC passato
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
void SetDischarge(int8_t cella,cell_asic bms_ic[]){
  /*da testare quando avremo più moduli,non attiva ora perchè bisognerebbe modificare alcune librerie */
  //ltc6813_set_discharge(cella,modulo,bms_ic);
  ltc6813_set_discharge(cella,TOTAL_IC,bms_ic);
  wakeup_sleep(TOTAL_IC);
  ltc6813_wrcfg(TOTAL_IC,bms_ic);
  ltc6813_wrcfgb(TOTAL_IC,bms_ic);
}

void ResetDischarge(cell_asic bms_ic[]){
  clear_discharge(TOTAL_IC,bms_ic);
  wakeup_sleep(TOTAL_IC);
  ltc6813_wrcfg(TOTAL_IC,bms_ic);
  ltc6813_wrcfgb(TOTAL_IC,bms_ic);
}

void OpenRelay(uint8_t relay){
  digitalWrite(relay, HIGH);
}

void CloseRelay(uint8_t relay){
  digitalWrite(relay, LOW);
}

void VoltageMeasurment(cell_asic bms_ic[]){
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
  for (int i=0; i<celle_usate; i++){
    Serial.print("cella ");
    Serial.print(i);
    Serial.print(";");
  }
  for (int i=0; i<ntc_usati; i++){
    Serial.print("ntc ");
    Serial.print(i);
    Serial.print(";");
  }
  Serial.print("in carica;");
  Serial.print("carica completata");
  Serial.println();
}

void AccendiLed(int pin){
    digitalWrite(pin,HIGH);    
}

void SpegniLed(int pin){
    digitalWrite(pin,LOW);
}

unsigned long Blink(int pin,unsigned long last_millis_led){
  if(millis() - last_millis_led > intervalloBlink) {
    last_millis_led = millis(); //save the last time you blinked the LED
    //if the LED is off turn it on and vice-versa:
    digitalWrite(pin, !digitalRead(pin));
  }
  return last_millis_led;
}

void StampaDebug2(cell_asic bms_ic[], bool in_carica, bool carica_completata){
    Serial.print(millis());
    Serial.print(";");
    for(int i=0; i<TOTAL_CH ;i++){
        if(i!=unused_ch_1 && i!=unused_ch_2){
          Serial.print(bms_ic[0].cells.c_codes[i]*0.0001,4);
          Serial.print(";");
        }
    }
     for(int i=0; i<ntc_usati ;i++){
       Serial.print(ReadTempGrad (3,0,bms_ic));
       Serial.print(";");
    }
    if(in_carica)
      Serial.print("Si");
    else 
      Serial.print("No");

    Serial.print(";");
    if(carica_completata)
      Serial.print("Si");
    else 
      Serial.print("No");

    Serial.print(";");

    Serial.println();
}

float ReadCurrent(){
  return 1;//faccio finta che passa 1A
}