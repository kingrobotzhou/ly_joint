#include "ec_state.hpp"

//namespace lyrobot {

EC_State::EC_State(std::string swigName, int swigValue)
{
  this->swigName = swigName;
  this->swigValue = swigValue;
  swigNext = swigValue+1;
}
int EC_State::getSwigValue()
{
  return this->swigValue;
}

//}//namespace lyrobot
