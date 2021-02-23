/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/

#include <HX711.h>

HX711 loadcell = HX711();

void setup() {
  Serial.begin(9600); delay(10);
  Serial.println();
  Serial.println("Starting...");
  loadcell.setup();
  loadcell.tare();
}

void loop() {
  
  Serial.println("Loadcell val : " + (String)loadcell.read());
  
  if (Serial.available() > 0) {
    char inByte = Serial.read();
  if (inByte == 't') loadcell.TareNoDelay();   
  if (inByte == 'r') loadcell.Calibrate();
  }
  
  delay(200);
}
