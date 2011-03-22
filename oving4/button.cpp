#include "button.h"

Button::Button (sc_module_name name, int _id) : sc_module (name)
{
  SC_HAS_PROCESS (Button);
  SC_METHOD(main);
  sensitive << press; // Set method sensitive to pushes
  id = _id;
  lightOn = false; // Light initially off
}

// Light signal response
void Button::light(bool b) 
{
  if(lightOn!=b) 
    { // Toggle light if bool different
      lightOn=b;
      if(b)
	cout <<"Button " << id << ":\tLight on " << endl; // Output light change
      else
	cout <<"Button " << id << ":\tLight off " << endl;
    }
}

// Called whenever button signal changes
void Button::main(void) 
{
  if (press.read() == true)
    {
      cout << "Button "<<id<<":\tPushed.\n";
      bus->push(id);
    }
}
