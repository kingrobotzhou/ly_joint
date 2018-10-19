#ifndef DS402SLAVE_H
#define DS402SLAVE_H

#include <sstream>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <iostream>
#include "slave.hpp"
#include "soem.hpp"
//namespace  lyrobot {

enum ModeOfOperation{
    Invalid_Mode=0,
    Profile_Position_Mode=1,
    Profile_Velocity_Mode=3,
    Profile_Torque_Mode=4,
    Homing_Mode=6,
    Interpolated_Position_Mode=7,
    Cyclic_Synchronous_Position_Mode=8,
    Cyclic_Synchronous_Velocity_Mode=9,
    Cyclic_Synchronous_Torque_Mode=10
};
enum {
    ServoCommandOFF,ServoCommandON,ServoCommandEnable,ServoCommandDisable,ServoCommandEmeStop,ServoCommandFaultReset,ServoCommandHalt,ServoCommandHaltReset
};

typedef enum {
    NOACTION, SHUTDOWN, SWITCHON, DISABLEVOLTAGE, ENABLEOPERATION, QUICKSTOPACTIVE, DISABLEOP, FAULTRESET
}ControlWord;

typedef enum {
    NOTREADYTOSWITCHON, SWITCHONDISABLED, READYTOSWITCHON, SWITCHEDON, OPERATIONENABLE, QUICKSTOPACTIVEING, FAULT
}StatusWord;

typedef enum {
    NoAction, ShutDown, SwitchOn, EnableOperation, DisableOperation, QuickStop, DisableVoltage, FaultReset
}ServoControlCommond;

enum{
    UnknowState, Not_ready_to_switch_on,Switch_on_disabled,Ready_to_switch_on,Switched_on,Operation_enabled,Quick_stop_active,Fault_reaction_active,Fault
};
class DS402Slave :public Slave
{
public:
    DS402Slave(int vendor, int productCode,int position):Slave(vendor,productCode,position){};
    void SetEnableDrive(bool enable);
    void EnableDrive();
    void DisableDrive();
    void SetOperationMode(ModeOfOperation& mode);
    void SetParameter();
    void SetData();
    void shutDown();
    bool IsReady();
    bool IsDriveOperational();
    void DoStateControl();
    StatusWord GetStatus();
    ControlWord GetCurrentControlWord();
private:
    static const uint16 noAction = 0b000;
    static const uint16 shutdown = 0b110;
    static const uint16 switchon = 0b0111;
    static const uint16 disableVoltage = 0b0101;
    static const uint16 enableOperation = 0b1111;
    static const uint16 quickStopActive = 0b1011;
    static const uint16 disableOp = 0b0111;
    static const uint16 faultReset = 0b10000000;

    static const uint16 STATUS_NOTREADYTOSWITCHON = 0x00;
    static const uint16 MASK_NOTREADYTOSWITCHON = 0x4F;

    static const uint16 STATUS_SWITCHONDISABLED = 0x40;
    static const uint16 MASK_SWITCHONDISABLED = 0x4F;

    static const uint16 STATUS_READYTOSWITCHON = 0x21;
    static const uint16 MASK_READYTOSWITCHON = 0x6F;

    static const uint16 STATUS_SWITHON = 0x23;
    static const uint16 MASK_SWITCHON = 0x6F;

    static const uint16 STATUS_OPERATIONENABLED = 0x27;
    static const uint16 MASK_OPERATIONENABLED = 0x6F;

    static const uint16 STATUS_QUICKSTOPACTIVE = 0x7;
    static const uint16 MASK_QUICKSTOPACTIVE = 0x4F;

    static const uint16 STATUS_FAULT = 0x08;
    static const uint16 MASK_FAULT = 0x4F;
    bool enableDrive = false;
    ControlWord command = NOACTION;
private:
    void SetControlWord(ControlWord control);
    bool HasShutDown();
};

//} namespace lyrobot
#endif // DS402SLAVE_H
