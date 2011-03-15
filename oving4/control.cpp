#include "Control.h"

const int Control::X[9] = {3,8,1,2,9,5,7,6,4};

/*
 * Constructor for Control.
 */
Control::Control(sc_module_name name) 
: sc_module(name) 
, count(0)
{ 
}

/*
 * Main thread.
 */
void main()
{
    int bus_status, button;
    uint32_t *data;
    uint16_t mem_addr, button_id;
    data_packet_t *packet;
    data = new uint32_t();
    packet = new data_packet_t();

    while (true) {
        bus_status = burst_read(priority, data, my_addr);

        switch (bus_status) {
            case SIMPLE_BUS_OK:
                if (*data != 0) {
                    // Get content of status word
                    button_id = (uint16_t)(*data & 0x0F);
                    mem_addr = (uint16_t)(*data >> 16);
                    button = 0;
                    while ((button_id >> 1) != 0)
                        button++;
                    // Read data package in memory address.
                    bus_status = burst_read(priority, packet, 
                                    mem_addr, 3, true); 
                    if (bus_status) {
                        // error handling.
                    }
                    // Update Memory Machine
                    if (button == packet->button_id &&
                        packet->button_pushed != 0) {
                        if (button == X[count]) {
                            count++;
                            *data = 1;
                            bus_status burst_write(priority, data, BUTTON_ 
                            

                        } else {

                        }
                    }

                }
                break;
            case SIMPLE_BUS_ERROR:
                
                break;
            default:

                break;
        }
        
        burst_read(
    simple_bus_status burst_read(unsigned int unique_priority
			       , int *data
			       , unsigned int start_address
			       , unsigned int length = 1
			       , bool lock = false);

    }

    delete data;
    delete packet;
}


// Checks for correct button in sequence
//void Control::push(int button) {
//  if (button==X[count]) { 
//    cout << "Correct button pushed!" << endl;
//    bus->light(button, true);
//    count++;
//  }
//  else {
//    cout << "Wrong button pushed.." << endl;
//    bus->light(0,false);
//    count = 0;
//  }
//  if(count==9) { // Sequence finished
//    bus -> light(0,false);
//    count=0;
//    cout << "Finished"<< endl;
//  }
//}
