#ifndef ETHERCATSTATUSCALLBACK_HPP
#define ETHERCATSTATUSCALLBACK_HPP
#include <string>
#include <iostream>
#include "slave.hpp"
//namespace lyrobot {

using namespace std;

string FAST_IRQ("Setting up fast IRQ");
string CREATE_CONTEXT("Creating context");
string OPEN_INTERFACE("Opening interface");
string CONFIGURING_SLAVES("Configured slaves");
string DC_ENABLED("DC Enabled");
string DC_DISABLED("DC Disabled");
string INITIALIZING_SLAVES("Initializing slaves");
string ALLOCATE_IOMAP("Allocating IO Map");
string LINK_BUFFERS("Linking buffers to slave data");
string CONFIGURE_TXRX("Sending first datagram");
string CONFIGURE_COMPLETE("Configuration complete");
string WAIT_SHUTDOWN("Waiting for EtherCAT Controller to stop");
string STOP_HOUSEHOLDER("Shutting down controller thread");
string SWITCH_PREOP("Switching slaves to PRE-OP state");
string CLEANUP_SLAVES("Cleanup slaves");
string SWITCH_TO_INIT("Switching slaves to INIT state");
string CONFIGURE_DC("Configuring DC settings");
string CLEAR_PDOS("Clearing PDO configuration");
string WRITE_PDOS("Writing PDO entries");
string WRITE_PDO_SIZE("Writing Number of PDO entries");
string RECONFIG_TO_PREOP("Reconfiguring slave to PRE-OP");
string RECONFIG_TO_SAFEOP("Reconfiguring slave to SAFE-OP");
string RECOVER_SLAVE("Slave lost. Recovering slave");
string RECOVERED_SLAVE("Sucessfully recovered slave");
string SLAVE_FOUND("Slave found");
string SLAVE_LOST("Slave lost");
string READ_WATCHDOG_DIV("Reading watchdog division time");
string WRITE_WATCHDOG_TIMEOUT("Writing watchdog timeout");
string IS_MASTER_DC("Is master DC");

class EtherCATStatusCallback
{
public:
  EtherCATStatusCallback(bool trace);
  void notifyStateChange(Slave slave, State previousState, State currentState);
private:
  string msg;
  bool TRACE;
private:
  void TRACE_EVENT(string msg)
  {
     this->msg = msg;
  }

  string getMessage()
  {
     return msg;
  }
  void trace(string event)
  {
    if (TRACE)
    {
//       System.out.println("[" + System.nanoTime() + "] Master: " + event.getMessage());
       cout<<"["<<"] Master: "<<event<<endl;
    }
  }
  void trace(Slave slave, string event)
     {
        if (TRACE)
        {
           cout<<"["<<"] Master: "<<"Slave["<<slave.GetSlaveIndex()<<"]"<<event<<endl;
        }
     }

};


//}//namespace lyrobot
#endif // ETHERCATSTATUSCALLBACK_HPP
