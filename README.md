# BMS_LTC6813_class
This class will allow us to easlily control our battery pack by monitoring cells voltage and temperature.

This can be done thanks to Linear Technology's libraries:
  "Linduino.h"
  "LT_SPI.h"
  "ltc681x.h"
  "ltc6813.h"

Fully compatible via SPI with Linduino and Arduino UNO (and probably every kind of ATMega based arduino board)

Comunication via SPI:
  /-actual-/
  Arduino(master)->DC1941D->(translation SPI-isoSPI)->DC2350A-b 
    
    DC1941D provides galvanic isolation and translate between SPI and isoSPI.
    DC2350A-b si a demo board with LTC6813 core that allow us to quickly plug our cells and our NTCs.
   
Comunication consist to send an SPI signal to DC1941D, that will be translated in isoSPI (the Linear proprietary comunication protocol) and will be read by the ADC LTC6813.
In the future there will be the possibility to comunicate directly to the ADC.
