#include <systemc.h>
#include "button.h"
#include "control.h"
#include "bus.h"

SC_MODULE(top) {
public:
  // Instances
  button *button_inst[9];
  control *control_inst;
  bus *bus_inst;
  top (sc_module_name name);
  void test_thread(void);// Simulation thread
  sc_signal<bool> pressSignals[9];// Signals for "pushing" the buttons
};

SC_HAS_PROCESS(top);
top::top (sc_module_name name) : sc_module (name) { 
  control_inst = new control("control");
  bus_inst = new bus("bus_inst");
  char buf[8];
  for (int i = 0; i<9; i++) {
    sprintf(buf, "button%d\0",i+1); //Gen. unique names for buttons
    button_inst[i] = new button(buf, i+1); 
    // Connect buttons to bus
    button_inst[i]->bus(*bus_inst); 
    bus_inst->buttons[i](*button_inst[i]);
    // Connect buttons to signals for "physical" push
    button_inst[i]->press(pressSignals[i]);
  }
  bus_inst->ctrl(*control_inst);
  control_inst->bus(*bus_inst);
  SC_THREAD(test_thread);
}

// Test method
void top::test_thread(void) {
  cout << "Testthread started!\n" <<
    "Testing correct button sequence:\n";
  pressSignals[2].write(true);
  // Need wait so button release does not happen in same delta cycle
  wait(SC_ZERO_TIME); 
  pressSignals[2].write(false);
  pressSignals[7].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[7].write(false);
  pressSignals[0].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[0].write(false);
  pressSignals[1].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[1].write(false);
  pressSignals[8].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[8].write(false);
  pressSignals[4].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[4].write(false);
  pressSignals[6].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[6].write(false);
  pressSignals[5].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[5].write(false);
  pressSignals[3].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[3].write(false);

  cout << "Testing incrementing button sequence:\n";
  pressSignals[0].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[0].write(false);
  pressSignals[1].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[1].write(false);
  pressSignals[2].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[2].write(false);
  pressSignals[3].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[3].write(false);
  pressSignals[4].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[4].write(false);
  pressSignals[5].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[5].write(false);
  pressSignals[6].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[6].write(false);
  pressSignals[7].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[7].write(false);
  pressSignals[8].write(true);
  wait(SC_ZERO_TIME);
  pressSignals[8].write(false);
}
int sc_main (int argc , char *argv[])  {
  top top_inst("Top");
  sc_start ();
  return 0;
}
