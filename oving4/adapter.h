#ifndef   	ADAPTER_H_
# define   	ADAPTER_H_

#include <systemc.h>
#include "if.h"
#include "simple_bus_blocking_if.h"

class Adapter : public control_if, public sc_module
{
 public:
  sc_port<button_if> button_p;
  sc_port<simple_bus_blocking_if> bus_p;
  
  Adapter (sc_module_name);
  virtual void push (int);  
 private:
  virtual void main(void);
  void send(data_packet_t*);
  void pushlisten(void);
  void bounce(void);
  sc_event listen_event;
  sc_event push_event;
  bool listening;
  int button_id;
  int status;
  data_packet_t packet;
};
#endif
