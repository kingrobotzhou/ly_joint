#ifndef SLAVE_HPP
#define SLAVE_HPP
#include <climits>
#include "soem.hpp"
#include "ec_state.hpp"
//namespace lyrobot{

enum State{OFFLINE, BOOT, INIT, PRE_OP, PRE_OPERR, SAFE_OP, SAFE_OPERR, OP};

class Slave
{
public:
  Slave(int position,int vendor,int productCode);
  int GetVendor();
  int GetProductCode();
  void ConfigurePDOWatchdog(int watchdogTimeoutInNamoseconds);
  void Configure();
  void ConfigureImpl();
  int GetInputBytes();
  int GetOutputBytes();
  void RegisterSyncManager();
  int GetSlaveIndex();
  State getState();
  int WriteSDO(uint16 index,uint16 subindex,uint8 value);
  int WriteSDO(uint16 index,uint16 subindex,uint16 value);
  int WriteSDO(uint16 index,uint16 subindex,uint32 value);
  int ReadSDO(uint16 index,uint16 subindex,uint8 buf);
  int ReadSDO(uint16 index,uint16 subindex,uint16 buf);
  int ReadSDO(uint16 index,uint16 subindex,uint32 buf);
  void configureDCSync0(bool enable, long sync0time, int sync0shift);
  void configureDCSync01(bool enable, long sync0time, long sync1time, int sync0shift);
  bool isOperational();
  void doEtherCATStateControl(long runTime);
  int getstate();
  void setstate(int value);
  bool getIslost();
  void setIslost(bool value);
  int reconfig_slave_to_preop();
  int reconfig_slave_to_safeop();
  void configure(bool dcEnabled, long cycleTimeInNs);
  int getALStatusCode();
  std::string getALStatusMessage();
private:
  void updateEtherCATState();
  void configureImpl(ec_slavet slave, boolean enableDC, long cycleTimeInNs);
private:
    const int MAX_DC_OFFSET_DEFAULT = 200;
    const int MAX_DC_OFFSET_SAMPLES = 10;
    const int ETC_SMT_SIZE = 8;
    const int ECT_REG_WATCHDOG_DIV = 0x0400;
    const int ETC_REG_WATCHDOG_PDO_TIMEOUT = 0x0420;
    int aliasAddress;
    int vendor;
    int productCode;
    ec_slavet ecslave;
    int slaveIndex;
    int DCdiff;
    bool dcEnabled;
    bool dcClockStable = false;
    volatile State state = OFFLINE;
    int maximumDCOffset = MAX_DC_OFFSET_DEFAULT;
    int dcOffsetSamples = 0;
    long cycleTimeInNs;
    bool configurePDOWatchdog = false;
    EC_State EC_STATE_NONE =EC_State("EC_STATE_NONE", 0x00);
    EC_State EC_STATE_INIT = EC_State("EC_STATE_INIT", 0x01);
    EC_State EC_STATE_PRE_OP =EC_State("EC_STATE_PRE_OP", 0x02);
    EC_State EC_STATE_BOOT = EC_State("EC_STATE_BOOT", 0x03);
    EC_State EC_STATE_SAFE_OP = EC_State("EC_STATE_SAFE_OP", 0x04);
    EC_State EC_STATE_OPERATIONAL = EC_State("EC_STATE_OPERATIONAL", 0x08);
    EC_State EC_STATE_ACK = EC_State("EC_STATE_ACK", 0x10);
    EC_State EC_STATE_ERROR = EC_State("EC_STATE_ERROR", 0x10);
 private:
    State getStateFromEcSlave();
    int getDCSyncOffset();
};


//}//namespace lyrobot
#endif // SLAVE_HPP














