#include <systemc.h>
#include "button.h"
#include "control.h"
#include "simple_bus.h"

SC_MODULE(Top) 
{
    public:
    // Instances
    Button *button[9];
    Control *control;
    simple_bus *bus;
    Top (sc_module_name name);
    void test_thread(void);// Simulation thread
    sc_signal<bool> pressSignals[9];// Signals for "pushing" the buttons
};

SC_HAS_PROCESS(Top);
Top::Top (sc_module_name name) : sc_module (name) 
{ 
    control = new Control("control");
    bus = new Bus("bus_inst");
    char buf[8];
    for (int i = 0; i<9; i++) {
        sprintf(buf, "button%d\0",i+1); //Gen. unique names for buttons
        button[i] = new button(buf, i+1); 
        // Connect buttons to bus
        button[i]->bus(*bus); 
        // Connect buttons to signals for "physical" push
        button[i]->press(pressSignals[i]);
    }
    control->bus(*bus_inst);
    SC_THREAD(test_thread);
}

// Test method
void top::test_thread(void) 
{ 
    const int   sequence1[9] = {3,8,1,2,9,5,7,6,4},
                sequence2[9] = {1,2,3,4,5,6,7,8,9};

    cout << "Testthread started!\n" 
         << "Testing correct button sequence:\n";
    for (int i = 0; i < 9; i++) {
        pressSignals[sequence1[i]].write(true);
        wait(BUTTON_PUSH_INTERVAL, sc_ms);
    }
    cout << "Testing incrementing button sequence:\n";
    for (int i = 0; i < 9; i++) {
        pressSignals[sequence2[i]].write(true);
        wait(BUTTON_PUSH_INTERVAL, sc_ms);
    }
    cout << "Testing complete.\n";
}

int sc_main (int argc , char *argv[])  
{
    top top_inst("Top");
    sc_start ();
    return 0;
}
