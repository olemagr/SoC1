#ifndef   	CONTROL_H_
# define   	CONTROL_H_

#include <systemc.h>
#include "if.h"

class control : public control_if, public sc_module {
public:
  sc_port<bus_if> bus;
  control (sc_module_name);
  virtual void push(int);
private:
  int count;
  static const int X[9];
};
#endif 	    /* !CONTROL_H_ */
