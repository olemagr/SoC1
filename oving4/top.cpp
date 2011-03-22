#include <systemc.h>
#include "if.h"
#include "button.h"
#include "adapter.h"
#include "control.h"
#include "simple_bus.h"
#include "simple_bus_fast_mem.h"
#include "simple_bus_arbiter.h"

SC_MODULE(Top) 
{
    public:
    // Instances
    Button *button[10];
    Adapter *adapter[10];
    Control *control;
    simple_bus *bus;
    simple_bus_arbiter *arbiter;
    simple_bus_fast_mem *fast_mem;
    sc_clock *clock;

    Top (sc_module_name name);
    void test_thread(void);// Simulation thread
    sc_signal<bool> pressSignals[10];// Signals for "pushing" the buttons
};

SC_HAS_PROCESS(Top);
Top::Top (sc_module_name name) : sc_module (name) 
{ 
    clock = new sc_clock("klokke", sc_time(500, SC_US));
    control = new Control("control");
    bus = new simple_bus("bus_inst");
    arbiter = new simple_bus_arbiter("arbiter");
    fast_mem = new simple_bus_fast_mem("fast_mem", 0, 4*1024-1);
    char buf[8];
    for (int i = 0; i<10; i++) {
        sprintf(buf, "button%d\0",i); //Gen. unique names for buttons
        button[i] = new Button(buf, i); 
        sprintf(buf, "adapter%d\0", i);
        adapter[i] = new Adapter(buf);
        // Connect buttons to bus
        button[i]->bus(*adapter[i]); 
        adapter[i]->button_p(*button[i]);
        adapter[i]->bus_p(*bus);
        // Connect buttons to signals for "physical" push
        button[i]->press(pressSignals[i]);
    }
    bus->clock(*clock);
    bus->arbiter_port(*arbiter);
    bus->slave_port(*fast_mem);
    control->bus_p(*bus);
    SC_THREAD(test_thread);
}

// Test method
void Top::test_thread(void) 
{ 
    const int   sequence1[9] = {3,8,1,2,9,5,7,6,4},
                sequence2[9] = {1,2,3,4,5,6,7,8,9};

    cout << "Test: \tThread start.\n" 
         << "Test: Testing correct button sequence...\n";
    for (int i = 0; i < 9; i++) {
        pressSignals[sequence1[i]].write(true);
        wait(1, SC_MS);
        pressSignals[sequence1[i]].write(false);
        wait(BUTTON_PUSH_INTERVAL, SC_MS);
    }
    pressSignals[0].write(true);
    wait(1, SC_MS);
    pressSignals[0].write(false);
    wait(BUTTON_PUSH_INTERVAL, SC_MS);
    cout << "Test: \tTesting incorrect button sequence...\n";
    for (int i = 0; i < 9; i++) {
        pressSignals[sequence2[i]].write(true);
        wait(BUTTON_PUSH_INTERVAL, SC_MS);
    }
    cout << "Test: \tTesting complete, stopping simulation.\n";
    sc_stop();
}

int sc_main (int argc , char *argv[])  
{
    Top top_inst("Top");
    sc_start ();
    return 0;
}
