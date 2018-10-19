#include "slave.hpp"
//namespace lyrobot {

Slave::Slave(int position,int vendor,int productCode)
{
  this->vendor = vendor;
  this->productCode = productCode;
  this->aliasAddress = aliasAddress;
}

int Slave::GetVendor()
{
  return vendor;
}
int Slave::GetProductCode()
{
  return productCode;
}
void Slave::ConfigurePDOWatchdog(int watchdogTimeoutInNamoseconds)
{

}
void Slave::Configure()
{

}
void Slave::ConfigureImpl()
{

}
int Slave::GetInputBytes()
{

}
int Slave::GetOutputBytes()
{

}
void Slave::RegisterSyncManager()
{

}
int Slave::GetSlaveIndex()
{
  return slaveIndex;
}
int Slave::getALStatusCode()
{
  if(ec_slave != NULL)
  {
    return ec_slave[slaveIndex].ALstatuscode;
  }
  else {
    return 0x0000;
  }
}
std::string Slave::getALStatusMessage()
 {
    return ec_ALstatuscode2string(getALStatusCode());
 }
State Slave::getState()
{
  return this->state;
}
bool Slave::isOperational()
{
  return getState() == OP;
}
State Slave::getStateFromEcSlave()
{
  int state = getstate();
  if (state == EC_STATE_BOOT.getSwigValue())
  {
     return BOOT;
  }
  else if (state == EC_STATE_INIT.getSwigValue())
  {
     return INIT;
  }
  else if (state == EC_STATE_PRE_OP.getSwigValue())
  {
     return PRE_OP;
  }
  else if (state == EC_STATE_PRE_OP.getSwigValue() + EC_STATE_ERROR.getSwigValue())
  {
     return PRE_OPERR;
  }
  else if (state == EC_STATE_SAFE_OP.getSwigValue())
  {
     return SAFE_OP;
  }
  else if (state == EC_STATE_SAFE_OP.getSwigValue() + EC_STATE_ERROR.getSwigValue())
  {
     return SAFE_OPERR;
  }
  else if (state == EC_STATE_OPERATIONAL.getSwigValue())
  {
     return OP;
  }
  else
  {
     return OFFLINE;
  }
}
void Slave::doEtherCATStateControl(long runTime)
{
  switch (this->state)
  {
  case BOOT:
  case INIT:
     if(!dcEnabled)
     {
//        master.getEtherCATStatusCallback().trace(this, TRACE_EVENT.RECONFIG_TO_PREOP);
        if(reconfig_slave_to_preop() > 0)
        {
           setIslost(false);
        }
     }
     break;
  case PRE_OP:
     if(!dcEnabled)
     {
       dcEnabled = ec_configdc();
     }
     break;
  case PRE_OPERR:
     break;
  case SAFE_OP:
     if(dcEnabled && !dcClockStable)
     {
        int dcOffset = getDCSyncOffset();
        if (dcOffset < maximumDCOffset && dcOffset > -maximumDCOffset)
        {
           // At boot dcOffset can be zero. Ignore this. Except for the master clock, which is always zero.
           if (dcOffset != 0 || ec_slave[slaveIndex].DCprevious == 0)
           {
              if (dcOffsetSamples++ > MAX_DC_OFFSET_SAMPLES)
              {
                 dcClockStable = true;
              }
           }
           else
           {
              dcOffsetSamples = 0;
           }
        }
        else
        {
//           master.getEtherCATStatusCallback().reportDCSyncWaitTime(this, runTime, dcOffset);
        }
     }
     else
     {
        setstate(EC_STATE_OPERATIONAL.getSwigValue());
     }
     break;
  case SAFE_OPERR:
     dcOffsetSamples = 0;
     dcClockStable = false;
     setstate(EC_STATE_SAFE_OP.getSwigValue() + EC_STATE_ACK.getSwigValue());
     break;
  case OP:
     break;
  case OFFLINE:
     if(getIslost() == 0)
     {
      ec_statecheck(slaveIndex,EC_STATE_OPERATIONAL.getSwigValue(),EC_TIMEOUTRET);
      if(getState() == 0)
      {
         setIslost(true);
//         master.getEtherCATStatusCallback().trace(this, TRACE_EVENT.SLAVE_LOST);
      }
     }
     break;
  }
}
int Slave::WriteSDO(uint16 index,uint16 subindex,uint8 value)
{
  return ec_SDOwrite(slaveIndex,index,subindex,FALSE,sizeof(value),&value,EC_TIMEOUTRXM);
}
int Slave::WriteSDO(uint16 index,uint16 subindex,uint16 value)
{
  return ec_SDOwrite(slaveIndex,index,subindex,FALSE,sizeof(value),&value,EC_TIMEOUTRXM);
}
int Slave::WriteSDO(uint16 index,uint16 subindex,uint32 value)
{
  return ec_SDOwrite(slaveIndex,index,subindex,FALSE,sizeof(value),&value,EC_TIMEOUTRXM);
}
int Slave::ReadSDO(uint16 index,uint16 subindex,uint8 buf)
{
  int __s = sizeof(buf);
  return ec_SDOread(slaveIndex, index, subindex, FALSE, &__s, &buf, EC_TIMEOUTRXM);
}
int Slave::ReadSDO(uint16 index,uint16 subindex,uint16 buf)
{
  int __s = sizeof(buf);
  return ec_SDOread(slaveIndex, index, subindex, FALSE, &__s, &buf, EC_TIMEOUTRXM);
}
int Slave::ReadSDO(uint16 index,uint16 subindex,uint32 buf)
{
  int __s = sizeof(buf);
  return ec_SDOread(slaveIndex, index, subindex, FALSE, &__s, &buf, EC_TIMEOUTRXM);
}

void Slave::configureDCSync0(bool enable, long sync0time, int sync0shift)
{
  ec_dcsync0(slaveIndex, enable?(short) 1 :(short)0, sync0time, sync0shift);
  this->dcEnabled = enable;
}
void Slave::configureDCSync01(bool enable, long sync0time, long sync1time, int sync0shift)
{
  ec_dcsync01(slaveIndex, enable?(short) 1 :(short)0, sync0time,sync1time, sync0shift);
  this->dcEnabled = enable;
}
int Slave::getstate()
{
  return ec_slave[slaveIndex].state;
}
void Slave::setstate(int value)
{
  ec_slave[slaveIndex].state = value;
}

void Slave::setIslost(bool value)
{
  ec_slave[slaveIndex].islost = (boolean)value;
}
bool Slave::getIslost()
{
  return (bool)ec_slave[slaveIndex].islost;
}
int Slave::reconfig_slave_to_safeop()
{
  int state, FMMUc;
  uint16 configadr;

  configadr = ec_slave[slaveIndex].configadr;

  state = ec_statecheck(slaveIndex, EC_STATE_PRE_OP.getSwigValue(), EC_TIMEOUTSTATE); /* check state change pre-op */
  if( state == EC_STATE_PRE_OP.getSwigValue())
     {
        ec_FPWRw(configadr, ECT_REG_ALCTL, htoes(EC_STATE_SAFE_OP.getSwigValue()) , EC_TIMEOUTSTATE*3); /* set safeop status */
        state = ec_statecheck(slaveIndex, EC_STATE_SAFE_OP.getSwigValue(), EC_TIMEOUTSTATE); /* check state change safe-op */
        /* program configured FMMU */
        for( FMMUc = 0 ; FMMUc < ec_slave[slaveIndex].FMMUunused ; FMMUc++ )
        {
           ec_FPWR(configadr, ECT_REG_FMMU0 + (sizeof(ec_fmmut) * FMMUc),
              sizeof(ec_fmmut), &ec_slave[slaveIndex].FMMU[FMMUc], EC_TIMEOUTSTATE*3);
        }
     }
  return state;
}
int Slave::reconfig_slave_to_preop()
{
  int nSM;
  uint16 configadr,state;
  configadr = ec_slave[slaveIndex].configadr;
  if (ec_FPWRw(configadr, ECT_REG_ALCTL, htoes(EC_STATE_INIT.getSwigValue()) , EC_TIMEOUTSTATE) <= 0)
  {
     return 0;
  }
  state = 0;
  ec_eeprom2pdi(slaveIndex); /* set Eeprom control to PDI */
  /* check state change init */
  state = ec_statecheck(slaveIndex, EC_STATE_INIT.getSwigValue(), EC_TIMEOUTSTATE);
  if(state == EC_STATE_INIT.getSwigValue())
  {
     /* program all enabled SM */
     for( nSM = 0 ; nSM < EC_MAXSM ; nSM++ )
     {
        if (ec_slave[slaveIndex].SM[nSM].StartAddr)
        {
           ec_FPWR(configadr, ECT_REG_SM0 + (nSM * sizeof(ec_smt)),
              sizeof(ec_smt), &ec_slave[slaveIndex].SM[nSM], EC_TIMEOUTSTATE*3);
        }
     }
     ec_FPWRw(configadr, ECT_REG_ALCTL, htoes(EC_STATE_PRE_OP.getSwigValue()) , EC_TIMEOUTSTATE*3);
     state = ec_statecheck(slaveIndex, EC_STATE_PRE_OP.getSwigValue(), EC_TIMEOUTSTATE); /* check state change pre-op */
  }

  return state;
}

int Slave::getDCSyncOffset()
{
  ec_FPRD(ec_slave[1].configadr, ECT_REG_DCSYSDIFF, sizeof(DCdiff), &DCdiff, EC_TIMEOUTRET);
  if(DCdiff<0)
  {
    DCdiff = - (int32)((uint32)DCdiff & 0x7ffffff);
  }
  return DCdiff;
}
void Slave::configureImpl(ec_slavet slave, boolean enableDC, long cycleTimeInNs)
{
  configureDCSync0(false, 0, 0);   // Disable DC Sync

  // Configure the PDO watchdog register by reading the divisor first

  if(configurePDOWatchdog)
  {
    //Configure the PDO watchdog
  }
//  for (int i = 0; i < syncManagers.length; i++)
//  {
//     if (syncManagers[i] != NULL)
//     {
//        syncManagers[i].configure(master, this);
//     }
//  }
//  master.getEtherCATStatusCallback().trace(this, TRACE_EVENT.CONFIGURE_DC);
  configure(enableDC, cycleTimeInNs);

  // Slaves are in SAFE_OP when the master has been initialized
  state = SAFE_OP;
}
void Slave::updateEtherCATState()
{
  State previousState = this->state;
  this->state = getStateFromEcSlave();

  if(previousState != this->state)
  {
//     master.getEtherCATStatusCallback().notifyStateChange(this, previousState, this.state);
  }
}
void Slave::configure(bool dcEnabled, long cycleTimeInNs)
{
  if (dcEnabled)
  {
     configureDCSync0(true, cycleTimeInNs, 0);
  }
  else
  {
     configureDCSync0(false, 0, 0);
  }
}
//}//namespace lyrobot




















