#ifndef EC_STATE_HPP
#define EC_STATE_HPP
#include <string>
//namespace  lyrobot {
class EC_State
{
public:
  int getSwigValue();
  EC_State(std::string swigName, int swigValue);
private:
  int swigNext = 0;
  int swigValue;
  std::string swigName;

};

//}//namespace lyrobot
#endif // EC_STATE_HPP
