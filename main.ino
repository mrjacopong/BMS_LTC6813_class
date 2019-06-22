/*!
  DC2259
  ltc6813-1: Battery stack monitor

  REVISION HISTORY
  $Revision: 1200 $
  $Date: 2017-01-26

  @verbatim

  NOTES
  Setup:
   Set the terminal baud rate to 115200 and select the newline terminator.
   Ensure all jumpers on the demo board are installed in their default positions from the factory.
   Refer to Demo Manual DC2259.

  USER INPUT DATA FORMAT:
  decimal : 1024
  hex     : 0x400
  octal   : 02000  (leading 0)
  binary  : B10000000000
  float   : 1024.0
  @endverbatim


  Copyright (c) 2017, Linear Technology Corp.(LTC)
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  The views and conclusions contained in the software and documentation are those
  of the authors and should not be interpreted as representing official policies,
  either expressed or implied, of Linear Technology Corp.

  The Linear Technology Linduino is not affiliated with the official Arduino team.
  However, the Linduino is only possible because of the Arduino team's commitment
  to the open-source community.  Please, visit http://www.arduino.cc and
  http://store.arduino.cc , and consider a purchase that will help fund their
  ongoing work.

  Copyright 2017 Linear Technology Corp. (LTC)
*/

//--------------------compatibilità con schede non arduino uno-----------------------------//

/*modifiche su LT_SPI.cpp
FUNCTION-> void spi_write(int8_t  data) :175
  SPI.transfer(data);
  //SPDR = data;                  
	//while (!(SPSR & _BV(SPIF)));  

FUNCTION-> int8_t spi_read(int8_t  data) :183    
  int8_t receivedVal = SPI.transfer(data);
	return receivedVal;
	//SPDR = data;
	//while (!(SPSR & _BV(SPIF)));
	//return SPDR; 
*/
/*modifiche su ltc681x.cpp IMPORTANTI
  aggiungere a 
  FUNCTION-> void clear_discharge(uint8_t total_ic, cell_asic ic[]) :1067
    ic[i].configb.tx_data[0] = 0;
    ic[i].configb.tx_data[1] = 0;
  
  altrimenti non funziona reset reset_discharge*/

#include <Arduino.h>
#include <stdint.h>
#include "Linduino.h"
#include "LT_SPI.h"
#include "UserInterface.h"
#include "ltc681x.h"
#include "ltc6813.h"
#include <SPI.h>
#include "elementi.h"
#include "libreriafunzioni.h"



#define ENABLED 1
#define DISABLED 0

#define DATALOG_ENABLED 1
#define DATALOG_DISABLED 0

unsigned long LastMillisLed1 = 0;      //per il funzionamento del blink
unsigned long LastMillisLed2 = 0;      //per il funzionamento del blink
unsigned long TempoPrec=0;

/**********************************************************
  Setup Variables
  The following variables can be modified to
  configure the software.

***********************************************************/
bool in_carica=true;                 // true->in carica ; false->non in carica;
bool IsCharged=false;                //serve per controllare che stiamo caricando
bool solo_una_volta=true;            //per controllare che sia la prima volta che entriamo nel while e quindi dobbiamo chiudere il relè
/************************************
  END SETUP
*************************************/

/******************************************************
 *** Global Battery Variables received from 681x commands
  These variables store the results from the ltc6813
  register reads and the array lengths must be based
  on the number of ICs on the stack
 ******************************************************/

cell_asic bms_ic[TOTAL_IC];                   //array di tensioni
Pacco pacco(1,18,1);                          //array di ogetti
/*!**********************************************************************
  \brief  Inititializes hardware and variables
 ***********************************************************************/

/*!**********************************************************************
  \struttura per gli errori
 ***********************************************************************/

void setup()
{
  Serial.begin(115200);
  spi_enable(SPI_CLOCK_DIV16); // This will set the Linduino to have a 1MHz Clock
  ltc681x_init_cfg(TOTAL_IC, bms_ic);
  ltc6813_reset_crc_count(TOTAL_IC, bms_ic);
  ltc6813_init_reg_limits(TOTAL_IC, bms_ic);
  init_pinout();              //inizializza i pin per led e rele'
  StampaHeaderTabella();
}

void loop(){
 
  bool ChargeSwitch=digitalRead (ChargeSwitchPin);
  voltage_measurment(bms_ic);             //leggo le tensioni dall'adc
  gpio_measurment(bms_ic);                //leggo le temperature dall'adc
  if(pacco.error_check(bms_ic))           //controllo degli errori
    shoutdown_error();                    //e spengo tutto se c'è un errore
  
  
  while (ChargeSwitch==HIGH && !pacco.error_check(bms_ic) && !IsCharged) {  //se c'è un errore interrompo la carica uscendo dal ciclo di carica
    if(solo_una_volta){                   //entra nell'if solo all'inizio del primo ciclo
      close_relay(RelayPin);              //in questo modo il controllo del relè passa a cella.carica()
      solo_una_volta=false;               //dopo essersi chiuso la prima volta per iniziare la carica
    }
    voltage_measurment(bms_ic);           
    gpio_measurment(bms_ic);
    IsCharged=pacco.carica(bms_ic);       //algorimtmo di carica
    ChargeSwitch=digitalRead (ChargeSwitchPin);
    delay(1000);
    
    LastMillisLed1=Blink(LedCarica,LastMillisLed1); //codice per led di debug
    SpegniLed(LedSistema);

    if(ChargeSwitch==LOW){                //se vogliamo interrompere la carica apriamo il rele' 
      open_relay(RelayPin);               //e interrompiamo la scarica delle celle(bilanciamenti)
      reset_discharge(bms_ic);
      solo_una_volta=true;
    }
    if(millis()-TempoPrec>30000){          //stampa ogni intervallo per la tabella
    TempoPrec=millis();
    pacco.StampaDebug(bms_ic,ChargeSwitch,IsCharged);
    }
  }


  if(IsCharged && ChargeSwitch==LOW) {
    IsCharged=false;
    solo_una_volta=true;
  }
  /*se lo switch è low vuol dire che non voglio caricare
  appena si triggera entro nel while per la prima volta
  faccio la carica in loop, appena finisce IsCharged è vero, quindi non entra nel loop di nuovo
  appena rimetto lo switch in low, vuol dire che non voglio più caricare
  quindi setto IsCharged falso in modo fa autorizzare la carica 
  per la prossima volta che voglio caricare

  un po' difficile da capire, ma dovrebbe funzionare

  /*debug*/
  if(millis()-TempoPrec>3000){        //stampa ogni intervallo per la tabella (se siamo fuori dal while)
    TempoPrec=millis();
    pacco.StampaDebug(bms_ic, IsCharged, ChargeSwitch);
  }
  LastMillisLed2 = Blink(LedSistema,LastMillisLed2); //codice per led di debug
  SpegniLed(LedCarica);
  delay(1000);
}