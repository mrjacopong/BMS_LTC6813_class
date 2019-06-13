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


char get_char();
void print_menu();
void read_config_data(uint8_t cfg_data[][6], uint8_t nIC);
void print_cells(uint8_t datalog_en);
void print_open();
void print_config();
void print_rxconfig();
void print_aux(uint8_t datalog_en);
void print_stat();
void check_error(int error);
/**********************************************************
  Setup Variables
  The following variables can be modified to
  configure the software.

***********************************************************/
bool in_carica=true;                 // true->in carica ; false->non in carica;
bool IsCharged=false;                //serve per controllare che stiamo caricando
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
Pacco pacco(1,16,1);
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
  //  init_pinout();
}

void loop(){
  uint8_t error = 0;
  uint32_t conv_time = 0;
  uint8_t user_command;
  uint8_t readIC = 0;
  char input = 0;
  uint8_t ChargeSwitch=digitalRead (ChargeSwitchPin);
  //--interfaccia utente temporanea--//
  if (Serial.available()){           // Check for user input
    uint8_t user_command;
    Serial.println("inserisci un numero qualsiasi per lanciare il programma");
    user_command = read_int();      // whait for a key
  
   //---------------------------------//
    voltage_measurment(bms_ic);             //leggo le tensioni dall'adc
    pacco.error_check(bms_ic);              //controllo degli errori e scrittura della matrice di errori
    
   
    
    while (ChargeSwitch==HIGH && !IsCharged) {
      close_relay(RelayPin);
      voltage_measurment(bms_ic);
      if (pacco.error_check(bms_ic)) {      //se c'è un errore evito di tornare in questo ciclo
        IsCharged=true;
        break;
      }
      IsCharged=pacco.carica(bms_ic);
    }
    if(IsCharged && ChargeSwitch==LOW) {
      IsCharged=false;
    }
    /*se lo switch è low vuol dire che non voglio caricare
    appena si triggera entro nel while per la prima volta
    faccio la carica in loop, appena finisce IsCharged è vero, quindi non entra nel loop di nuovo
    appena rimetto lo switch in low, vuol dire che non voglio più caricare
    quindi setto IsCharged falso in modo fa autorizzare la carica 
    per la prossima volta che voglio caricare

    un po' difficile da capire, ma dovrebbe funzionare
    */

    Serial.println(ReadTempGrad (3,0,bms_ic));//0 sarebbe il prmio IC

    /*debug*/

  }
}