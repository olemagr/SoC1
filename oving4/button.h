#ifndef   	BUTTON_H_
# define   	BUTTON_H_

#include <systemc.h>
#include "if.h"

class button : public button_if, public sc_module
{
public:
  sc_in<bool> press;// Port for simulation of physical push
  sc_port<control_if> bus;

  button (sc_module_name, int);
  virtual void main(void);
  virtual void light (bool);
private:
  int id;        
  bool lightOn; // Light status of button
};
#endif 	    /* !BUTTON_H_ */
