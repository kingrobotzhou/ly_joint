#ifndef COPLEYAEM_HPP
#define COPLEYAEM_HPP
#include <math.h>
#include "soem.hpp"
#include "ds402slave.hpp"
//namespace  lyrobot {

class CopleyAEM:public DS402Slave
{
public:
  CopleyAEM(int ringPosition):DS402Slave(CopleyVenderID,ProductCode,ringPosition){};
  void ConfigureDC(bool dcEnabled,int cycleTimeInNs);
  void ConfigurePDO();
private:
    bool getBitValue(long maskValue);
private:
  static const int16 INDEX_Deviece_Type = 0x1000;
  static const int16 INDEX_Amplifier_Event_Nord = 0x1002;
  static const int16 INDEX_Amplifier_Model_Number = 0x1008;
  static const int16 INDEX_Hardware_Version_String = 0x1009;
  static const int16 INDEX_Manufacturer_Software_Version = 0x100a;
  static const int16 INDEX_LatchFaults = 0x2182;
  static const int16 INDEX_AmplifierTemperature = 0x2202;
  static const int CopleyVenderID = 0x000000ab;
  static const int ProductCode = 0x00001030;
  const int ENCODER_ERROR = pow(2,5);
  const int CURRENT_LIMITED = pow(2,7);
  const int UNDER_VOLTAGE = pow(2,3);
  const int OVER_VOLTAGE = pow(2,2);
  const int OUTPUT_VOLTAGE_LIMITED = pow(2,8);
};

//}//namespace lyrobot
#endif // COPLEYAEM_HPP
