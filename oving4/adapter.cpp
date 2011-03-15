#include "adapter.h"

// Construktor
Adapter::Adapter (sc_module_name name) : sc_module (name)
{
  SC_HAS_PROCESS (Adapter);
  SC_THREAD(main);
  listening = false;
  button_id = 0;
  status = 0;
}

void Adapter::push (int b_id) {
  // Write to memory
  button_id = b_id;
  packet.button_id = button_id;
  packet.button_pushed = 1;
  send(&packet);
  listening = true; // Do this last
  listen_event.notify();
}

void Adapter::send(data_packet_t* packet) {

  // Find next available location in ring buffer
  bus_p->burst_read((unsigned)button_id+10, &temp_read, 
	     FREELOC, 1, true);

  // Write status packet to location
  bus_p->burst_write((unsigned)button_id+10, (int*)packet, 
	      temp_read, 3, true);
  
  // Generate control word with location and button id
  control_word = (temp_read<<16)|(1<<button_id);

  // Generate and write next location
  temp_read = (temp_read+12) % BUFFER_MAX_ADDRESS;
  bus_p->burst_write((unsigned)button_id+10, &temp_read, 
	      FREELOC, 3, true);

  // Read control word
  bus_p->burst_read((unsigned)button_id+10, &temp_read, 
	     CONTROL_ADDRESS, 1, true);

  // Loop until control unit available
  while (!(temp_read & 0xFF)) {
    // Read for releasing lock
    bus_p->burst_read((unsigned)button_id+10, &temp_read, 
	       CONTROL_ADDRESS, 0, false);
    wait(ADAPTER_PUSH_WAIT,SC_MS);
    // Try reading again
    bus_p->burst_read((unsigned)button_id+10, &temp_read, 
	       CONTROL_ADDRESS, 1, true);
  }

  // Write new control word and release bus
  bus_p->burst_write((unsigned)button_id+10, &control_word, 
	      CONTROL_ADDRESS, 1, false);

}

void Adapter::main(void) {
  while (true) {
    if (listening)
      {
	// Check memory
	bus_p->burst_read((unsigned)button_id+10, &temp_read, 
		   BUTTON_STATUS_ADDRESS + button_id,
		   1, false);
	
	if (temp_read != status) {
	  status = temp_read;
	  listening = temp_read;
	  button_p->light(status);
	}

	wait(ADAPTER_LIGHT_WAIT, SC_MS);
      }
    else 
      wait(listen_event);
  }
}
