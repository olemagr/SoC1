#include "control.h"

const int control::X[9] = {3,8,1,2,9,5,7,6,4};

control::control (sc_module_name name) : sc_module (name) { 
  count = 0; // Initialize count 
}
// Checks for correct button in sequence
void control::push(int button) {
  if (button==X[count]) { 
    cout << "Correct button pushed!" << endl;
    bus->light(button, true);
    count++;
  }
  else {
    cout << "Wrong button pushed.." << endl;
    bus->light(0,false);
    count = 0;
  }
  if(count==9) { // Sequence finished
    bus -> light(0,false);
    count=0;
    cout << "Finished"<< endl;
  }
}
