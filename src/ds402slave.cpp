#include "ds402slave.hpp"

//namespace lyrobot {

void DS402Slave::SetControlWord(ControlWord control)
{
  int cmd = 0;
  switch (control)
  {
  case NOACTION:
     cmd = noAction;
     break;
  case SHUTDOWN:
     cmd = shutdown;
     break;
  case SWITCHON:
     cmd = switchon;
     break;
  case DISABLEVOLTAGE:
     cmd = disableVoltage;
     break;
  case ENABLEOPERATION:
     cmd = enableOperation;
     break;
  case QUICKSTOPACTIVE:
     cmd = quickStopActive;
     break;
  case DISABLEOP:
     cmd = disableOp;
     break;
  case FAULTRESET:
     cmd = faultReset;
     break;
  default:
    throw std::runtime_error("Invalid command");
  }
}

StatusWord DS402Slave::GetStatus()
{
//  int statusWord = getStatusWordPDOEntry().get();
  int statusWord;

  StatusWord state = NOTREADYTOSWITCHON;

  if ((statusWord & MASK_NOTREADYTOSWITCHON) == STATUS_NOTREADYTOSWITCHON)
  {
     state = NOTREADYTOSWITCHON;
  }
  else if ((statusWord & MASK_SWITCHONDISABLED) == STATUS_SWITCHONDISABLED)
  {
     state = SWITCHONDISABLED;
  }
  else if ((statusWord & MASK_READYTOSWITCHON) == STATUS_READYTOSWITCHON)
  {
     state = READYTOSWITCHON;
  }
  else if ((statusWord & MASK_SWITCHON) == STATUS_SWITHON)
  {
     state = SWITCHEDON;
  }
  else if ((statusWord & MASK_OPERATIONENABLED) == STATUS_OPERATIONENABLED)
  {
     state = OPERATIONENABLE;
  }
  else if ((statusWord & MASK_QUICKSTOPACTIVE) == STATUS_QUICKSTOPACTIVE)
  {
     state = QUICKSTOPACTIVEING;
  }
  else if ((statusWord & MASK_FAULT) == STATUS_FAULT)
  {
     state = FAULT;
  }

  return state;
}
ControlWord DS402Slave::GetCurrentControlWord()
{
  return command;
}

void DS402Slave::DoStateControl()
{
  StatusWord state = GetStatus();
  switch (state) {
     case NOTREADYTOSWITCHON: {
        break;
     }
     case SWITCHONDISABLED: {

        if(command == FAULTRESET)
        {
           command = NOACTION;
        }
        else if(enableDrive)
        {
           command = SHUTDOWN;
        }

        break;
     }
     case READYTOSWITCHON: {
        command = SWITCHON;
        break;
     }
     case SWITCHEDON: {
        if(enableDrive)
        {
           command = ENABLEOPERATION;
        }
        else
        {
           command = DISABLEVOLTAGE;
        }
        break;
     }
     case OPERATIONENABLE: {

        if(!enableDrive)
           command = DISABLEVOLTAGE;
        break;
     }
     case QUICKSTOPACTIVE: {

        break;
     }
     case FAULT: {

        if(command == FAULTRESET)
        {
           command = NOACTION;
        }
        else
        {
//               System.err.println("Drive fault detected, resetting");
           command = FAULTRESET;
        }
        enableDrive = false;

        break;
     }
     default: {
        throw std::runtime_error("Unknown state");
     }

  }
  if(command != NULL)
     SetControlWord(command);

}
void DS402Slave::EnableDrive()
{
  enableDrive = true;
}
void DS402Slave::DisableDrive()
{
  enableDrive = false;
}
void DS402Slave::SetEnableDrive(bool enable)
{
   enableDrive = enable;
}
bool DS402Slave::IsDriveOperational()
{
  return GetStatus() == OPERATIONENABLE;
}
bool DS402Slave::IsReady()
{
//  int statusWord = getStatusWordPDOEntry().get();
  int statusWord;
  return isOperational() && statusWord != 0 && GetStatus() != NOTREADYTOSWITCHON;
}

void DS402Slave::shutDown()
{
  DisableDrive();
  DoStateControl();
}
bool DS402Slave::HasShutDown()
{
  return !IsDriveOperational();
}
//}//namespace lyrobot
