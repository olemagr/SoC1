#include "adapter.h"

#define B_ADDR(button) BUTTON_STATUS_ADDRESS+button*BUTTON_STATUS_OFFSET
#define B_PRI(button) (unsigned)(button + BUTTON_BASE_PRIORITY)

// Construktor
Adapter::Adapter (sc_module_name name) : sc_module (name)
{
  SC_HAS_PROCESS (Adapter);
  SC_THREAD(main);
  SC_THREAD(pushlisten);
  listening = false;
  button_id = 0;
  status = 0;
}

void Adapter::pushlisten() 
{
  while(true) 
    {
      wait(push_event);
      packet.button_id = button_id;
      packet.button_pushed = 1;
      send (&packet);
      listening = true;
      listen_event.notify();
    }
}
void Adapter::push (int b_id) 
{
  button_id = b_id;
  push_event.notify();
}

void Adapter::send(data_packet_t* packet) 
{

  // Find next available location in ring buffer
  bus_p->burst_read(B_PRI(button_id), &temp_read, 
		    FREELOC, 1, true);

  // Write status packet to location
  bus_p->burst_write(B_PRI(button_id), (int*)packet, 
		     temp_read, 3, true);
  cout << "written status packet.\n";
  
  // Generate control word with location and button id
  control_word = (temp_read<<16)|(1<<button_id);

  // Generate and write next location
  temp_read = temp_read+12;
  if (temp_read >= BUFFER_MAX_ADDRESS) temp_read = BUFFER_START;
  bus_p->burst_write(B_PRI(button_id), &temp_read, 
		     FREELOC, 3, true);

  cout << "Written next location.\n";
  // Read control word
  bus_p->burst_read(B_PRI(button_id), &temp_read, 
		    CONTROL_ADDRESS, 1, true);

  cout << "control thing - " << temp_read << "\n";

  // Loop until control unit available
  while ((temp_read & 0xFF)) 
    {
      // Read for releasing lock
      bus_p->burst_read(B_PRI(button_id), &temp_read, 
			CONTROL_ADDRESS, 0, false);
      wait(ADAPTER_PUSH_WAIT,SC_MS);
      // Try reading again
      bus_p->burst_read(B_PRI(button_id), &temp_read, 
			CONTROL_ADDRESS, 1, true);
    }

  // Write new control word and release bus
  bus_p->burst_write(B_PRI(button_id), &control_word, 
		     CONTROL_ADDRESS, 1, false);

}

void Adapter::main(void) 
{
  while (true) 
    {
      if (listening)
	{
	  // Check memory
	  bus_p->burst_read(B_PRI(button_id), &temp_read, 
			    B_ADDR(button_id), 1, false);
	
	  if (temp_read != status) 
	    {
	      status = temp_read;     // Set new light status if it has changed in memory
	      // If the new status is false, we stop listening for changes, 
	      // as the light will only be switched on after a button push
	      listening = temp_read;  
	      button_p->light(status);
	    }

	  wait(ADAPTER_LIGHT_WAIT, SC_MS);
	}
      else 
	wait(listen_event);
    }
}
