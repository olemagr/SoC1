#include "button.h"

// Construktor
button::button (sc_module_name name, int _id) : sc_module (name)
{
  SC_HAS_PROCESS (button);
  SC_METHOD(main);
  sensitive << press; // Set method sensitive to pushes
  id = _id;
  lightOn = false; // Light initially off
}
void button::light(bool b) {
  if(lightOn!=b) { // Toggle light if bool different
    lightOn=b;
    if(b)
      cout <<"Light on " << id << endl; // Output light change
    else
      cout <<"Light off " << id << endl;
  }
}
// Called whenever button signal changes
void button::main(void) {
  if (press.read() == true)
    {
      cout << "Button "<<id<<" was pressed!\n";
      bus->push(id);
    }
}
