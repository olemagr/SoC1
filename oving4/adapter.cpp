#include "adapter.h"

#define B_ADDR(button) BUTTON_STATUS_ADDRESS+button*BUTTON_STATUS_OFFSET
#define B_PRI(button) (unsigned)(button + BUTTON_BASE_PRIORITY)

Adapter::Adapter (sc_module_name name) : sc_module (name)
{
  SC_HAS_PROCESS (Adapter);
  SC_THREAD(main);
  SC_THREAD(pushlisten);
  listening = false;
  button_id = 0;
  status = 0;
}

/*
 * Bounce simulates spamming of the bus
 */
void Adapter::bounce() 
{
  wait(TR, TR_UNIT); 
  int dummysize;
  sc_time interval( 3*TR ,TR_UNIT);
  sc_time stop_time = sc_time_stamp() + interval;
  while( sc_time_stamp() < stop_time )
    {
      // Generate dummy garbage packets 
      dummysize = rand() % 18;
      packet.packet_size = 3 + dummysize;
      packet.button_id = button_id;
      packet.button_pushed = 0;
      packet.dummy = new int[dummysize]; // Attach garbage data
      memset( packet.dummy, 0, dummysize*sizeof(int) ); // Set garbage to zeroes
      send (&packet);
      delete [] packet.dummy; 
      wait(rand()%TR, TR_UNIT);
    }

}

/*
 * Thread waiting for buttonpushes. Generates push packet. Calls bounce.
 */
void Adapter::pushlisten() 
{
  while(true) 
    {
      wait(push_event);
      packet.packet_size = 3;
      packet.button_id = button_id;
      packet.button_pushed = 1;
      send (&packet);
      listening = true;
      listen_event.notify();
      bounce();
    }
}

/*
 * Part of the Bus interface called by the buttons
 */
void Adapter::push (int b_id) 
{
  button_id = b_id;
  push_event.notify();
}

/*
 * Handles sends of push and release packets.
 */
void Adapter::send(data_packet_t* packet) 
{
  int temp_read, control_word;
  // Find next available location in ring buffer
  bus_p->burst_read(B_PRI(button_id), &temp_read, 
		    FREELOC, 1, true);

  if ((temp_read+(packet->packet_size*4)) >= BUFFER_MAX_ADDRESS) 
    {
      // Prevent memory overflow
      temp_read = BUFFER_START;
    } 
  
  // Write status packet to free location
  bus_p->burst_write(B_PRI(button_id), (int*)packet, 
		     temp_read, 3, true);

  if ( int dummy_size = (packet->packet_size - 3) )
    {
      // Writes additional dummy data, if attached to packet.
      bus_p->burst_write(B_PRI(button_id), packet->dummy, 
			 temp_read + 12, dummy_size, true);
    }

  // Generate control word with location and button id
  control_word = (temp_read<<16)|(1<<button_id);

  // Calculate and write next free location
  temp_read = temp_read + packet->packet_size*4;
  bus_p->burst_write(B_PRI(button_id), &temp_read, 
		     FREELOC, 1, true);
  
  if (packet->button_pushed != 0) // Releasepackets does not wait for free control
    {
      // Read control word
      bus_p->burst_read(B_PRI(button_id), &temp_read, 
			CONTROL_ADDRESS, 1, true);

      // Loop until control unit available
      while ((temp_read & 0xFF)) 
	{
	  wait(ADAPTER_PUSH_WAIT,INTERNAL_UNIT);
	  bus_p->burst_read(B_PRI(button_id), &temp_read, 
			    CONTROL_ADDRESS, 1, true);
	}

      // Write new control word
      bus_p->burst_write(B_PRI(button_id), &control_word, 
			 CONTROL_ADDRESS, 1, false);
    }
}

/*
 * The main method takes care of listening to changes to the light status in memory.
 */
void Adapter::main(void) 
{
  int temp;
  while (true) 
    {
      if (listening) // Listening is activated when the button is pushed.
	{
	  // Check light status in memory
	  bus_p->burst_read(B_PRI(button_id+10), &temp, 
			    B_ADDR(button_id), 1, false);
	  
	  if (temp != status) 
	    {
	      // Set new light status if it has changed in memory
	      status = temp;     
	      // If the new status is false, we stop listening for changes, 
	      // as the light will only be switched on after a button push
	      listening = temp;  
	      button_p->light(status);
	    }
	  
	  wait(ADAPTER_LIGHT_WAIT, INTERNAL_UNIT); // Sleep
	}
      else 
	{
	  wait(listen_event); // Notified on button push
	}
    }
}
