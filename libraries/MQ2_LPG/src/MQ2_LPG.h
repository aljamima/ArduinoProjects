// Library: Gas MQ2-LPG
// By: Devan Cakra M.W

#ifndef MQ2_LPG_H
#define MQ2_LPG_H

#include "Arduino.h"

class MQ2Sensor{
  public:
    MQ2Sensor(uint8_t pin);
    void begin();
    void RL(float RL_Value);
    void Ro(float Ro_Value);
    void Volt(float Voltage_Value);
    void BitADC(float bitADC_Value);
    void VRL();
    void getCalibrationData();
    void viewCalibrationData();
    float roCheck();
    float ratioCheck();
    double mCurve(double x1, double x2, double y1, double y2);
    double bCurve(double x, double y);
    float readGas();
    void viewGasData();

  private:
    uint8_t _pin;
    float _RL, _Ro, _Voltage, _bitADC, _VRL, _Rs, _getDataRo, _getDataRatio, _m, _b, _ppm;
};

#endif
