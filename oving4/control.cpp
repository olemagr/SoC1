#include "control.h"

const int Control::X[9] = {3,8,1,2,9,5,7,6,4};

/*
 * Constructor for Control.
 */
Control::Control(sc_module_name name) 
  : sc_module(name) 
  , count(0)
  , priority(1)
  , my_addr(CONTROL_ADDRESS)
{ 
  SC_HAS_PROCESS(Control);
  SC_THREAD(main);
}

#define B_ADDR(button) BUTTON_STATUS_ADDRESS+button*BUTTON_STATUS_OFFSET
/*
 * Main thread.
 */
void Control::main()
{
  int button;
  int *data, *reset;
  uint16_t mem_addr, button_id;
  data_packet_t *packet;

  data = new int;
  reset = new int[10];
  memset(reset, 0, 10*sizeof(uint32_t));
  packet = new data_packet_t;

  *data = BUFFER_START;
  bus_p->burst_write(priority, reset, B_ADDR(0), 10, true);
  bus_p->burst_write(priority, data, FREELOC); 
  while (true) 
    {
      // Read status word.
      bus_p->burst_read(priority, data, my_addr);

      if (*data != 0) 
	{
	  // Get content of status word
	  button_id = (uint16_t)(*data & 0x0000FFFF);
	  mem_addr = (uint16_t)(*data >> 16);
	  button = 0;
	  while ((button_id >>= 1) != 0) 
	    button++;
      
	  // Read data package in memory address.
	  bus_p->burst_read(priority, (int*)packet, (int)mem_addr, 3, true); 
	  // Update Memory Machine
	  if (button == packet->button_id &&
	      packet->button_pushed != 0) 
	    {
	      if (button == X[count]) 
		{ // Button push was correct
		  count++;
		  *data = 1;
		  bus_p->burst_write(priority, data, B_ADDR(button), 1, true);
		} 
	      else 
		{ // Button push was wrong, reset lights
		  count = 0;
		  bus_p->burst_write(priority, reset, B_ADDR(0), 10, true);
		}
	    }
	  // Clear status word. (release lock)
	  *data = 0;
	  bus_p->burst_write(priority, data, my_addr);
	}
      wait(CONTROL_SLEEP, INTERNAL_UNIT);
    }
  
  delete data;
  delete [] reset; 
  delete packet;
}
