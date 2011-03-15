#ifndef   	CONTROL_H_
# define   	CONTROL_H_

#include <systemc.h>
#include <stdint.h>
#include "if.h"
#include "simple_bus.h"

class Control 
: public sc_module 
{
    public:
    // Port to Simple Bus' blocking interface, burst-read and -write.
    sc_port<simple_bus_blocking_if> bus_p;

    Control(sc_module_name);
    void main();

    private:
    int priority;
    int count;
    int my_addr;
    static const int X[9];
};
#endif 	    /* !CONTROL_H_ */
