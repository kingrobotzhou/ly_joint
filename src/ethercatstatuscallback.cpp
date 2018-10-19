#include "ethercatstatuscallback.hpp"
//namespace lyrobot {

EtherCATStatusCallback::EtherCATStatusCallback(bool trace)
{
  this->TRACE = trace;
}
void EtherCATStatusCallback::notifyStateChange(Slave slave, State previousState, State currentState)
{
  if (TRACE)
  {
     cout<<"Slave "<<slave.GetSlaveIndex()<<" changed state from "<<(int)previousState<<" to "<<(int)currentState<<endl;
  }
  switch (currentState)
  {
  case BOOT:
     break;
  case INIT:
     break;
  case PRE_OP:
     break;
  case PRE_OPERR:
     cout<<slave.GetSlaveIndex()<<" in PREOP+ERR. "<<slave.getALStatusCode();
     break;
  case SAFE_OP:
     break;
  case SAFE_OPERR:
     cout<<slave.GetSlaveIndex()<<" in SAFEOP+ERR. "<<slave.getALStatusCode();
     break;
  case OP:
     break;
  case OFFLINE:
     break;

  }

}

//}//namespace lyrobot
