#include "HX711.h"
#include <HX711_ADC.h>
#include "Arduino.h"

HX711_ADC Cell(2, 3);


HX711::HX711()
{
}

void HX711::tare() {

    // receive command from serial terminal, send 't' to initiate tare operation:
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') Cell.tareNoDelay();
  }

  // check if last tare operation is complete:
  if (Cell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }

}

double HX711::read() {

  static boolean newDataReady = 0;
  const int serialPrintInterval = 0; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (Cell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    unsigned long t = 0;
    float i;
    if (millis() > t + serialPrintInterval) {
      i = Cell.getData();
      newDataReady = 0;
      t = millis();
    }

    return i;
  }
  
}

void HX711::setup()
{
  Cell.begin();
  calibrationValue = -41.5; // uncomment this if you want to set the calibration value in the sketch

  Cell.start(2000, true);
  if (Cell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
  Cell.setCalFactor(calibrationValue); // set calibration value (float)
  }
}

void HX711::TareNoDelay()
{
  Cell.tare();
}

void HX711::Calibrate() {
  Serial.println("***");
  Serial.println("Start calibration:");
  
  boolean _resume = false;
 

  Serial.println("Now, place your known mass on the loadcell.");
  Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");

  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    Cell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print("Known mass is: ");
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  Cell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  calibrationValue = Cell.getNewCalibration(known_mass); //get the new calibration value

  Serial.print("New calibration value has been set to: ");
  Serial.print(calibrationValue);
  Serial.println(", use this as calibration value (calFactor) in your project sketch.");
  Serial.println("End calibration");
  Serial.println("***");

}
