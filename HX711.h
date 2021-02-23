#ifndef HX711_H
#define HX711_H

#include <HX711_ADC.h>
#include "Arduino.h"


class HX711 {
  public:

  HX711();

  void tare();
  double read();
  void setup();
  void TareNoDelay();
  void Calibrate();

  float calibrationValue = 0;

};

#endif
