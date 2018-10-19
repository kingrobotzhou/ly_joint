#ifndef ETHERCATSTATEMACHINE_HPP
#define ETHERCATSTATEMACHINE_HPP
#include "slave.hpp"
class EtherCATStateMachine
{
public:
  EtherCATStateMachine();
  void setSlaves(Slave slaves[]);
  void disableRecovery();
  void doStateControl();
  int updateSlaveStates();
  void doSDOTransfer();
  void shutDown();
private:
  long MINIMUM_JITTER_SAMPLES = 1000;
  Slave slaves[];
  bool recoveryDisabled;
private:
  class EtherCATState
  {
  public:
    virtual EtherCATState next();
  };
  class ReadState:public EtherCATState
  {
  public:
    EtherCATState next()
    {
      int state = updateSlaveStates();
      if (state == EC_STATE_OPERATIONAL)
      {
         if(recoveryDisabled)
         {
            return finalState;
         }
         else
         {
            return checkForLostSlavesState;
         }
      }
      else
      {
         return waitForMasterState;
      }
    }
  };
  class WaitForMasterState:public EtherCATState
  {
  public:
    EtherCATState next()
    {
       long jitterEstimate = master.getJitterEstimate();

       if (!master.getDCEnabled())
       {
          return slaveState;
       }
       else if (master.getJitterSamples() < MINIMUM_JITTER_SAMPLES)
       {
          return this;
       }
       else if (jitterEstimate == 0 || jitterEstimate > master.getMaximumExecutionJitter())
       {
          master.getEtherCATStatusCallback().reportMasterThreadStableRateTime(runTime, jitterEstimate);
          return this;
       }
       else
       {
          return slaveState;
       }

    }
  };
  class SlaveState:public EtherCATState
  {
  private:
    Slave slave;
    EtherCATState next;
  private:
    SlaveState(Slave slave, EtherCATState next)
    {
       this->slave = slave;
       this->next= next;
    }
  public:
    EtherCATState Next()
    {
       slave.doEtherCATStateControl(runTime);
       return next;
    }

  };
  class CheckForLostSlavesState:public EtherCATState
  {
  public:
    EtherCATState next()
    {
//       if (master.getExpectedWorkingCounter() != master.getActualWorkingCounter())
//       {
//          return readState;
//       }
//       else
//       {
//          doSDOTransfer();
//          return this;
//       }
    }

  };
  class FinalState:public EtherCATState
  {
    long previousActualWorkingCounter = 0;
  public:
    EtherCATState next()
     {
        int wkc = master.getActualWorkingCounter();
        if (wkc != master.getExpectedWorkingCounter())
        {
           if(previousActualWorkingCounter != wkc)
           {
              updateSlaveStates();
           }
           else
           {
              doSDOTransfer();
           }
        }
        else
        {
           doSDOTransfer();
        }

        previousActualWorkingCounter = wkc;

        return this;
     }
  };

};

#endif // ETHERCATSTATEMACHINE_HPP
