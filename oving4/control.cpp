#include "Control.h"

const int Control::X[9] = {3,8,1,2,9,5,7,6,4};

/*
 * Constructor for Control.
 */
Control::Control(sc_module_name name) 
: sc_module(name) 
, count(0)
, my_addr(CONTROL_ADDRESS)
{ 
}

#define B_ADDR(button) BUTTON_STATUS_ADDRESS+button*BUTTON_STATUS_OFFSET
/*
 * Main thread.
 */
void Control::main()
{
    int bs, button;
    uint32_t *data, *reset;
    uint16_t mem_addr, button_id;
    data_packet_t *packet;

    data = new uint32_t;
    reset = new uint32_t[9];
    memset(reset, 0, 9*sizeof(uint32_t));
    packet = new data_packet_t;

    while (true) {
        // Read status word.
        bs = burst_read(priority, data, my_addr);

        if (*data != 0) {
            // Get content of status word
            button_id = (uint16_t)(*data & 0x0F);
            mem_addr = (uint16_t)(*data >> 16);
            button = 0;
            while ((button_id >> 1) != 0)
                button++;
            // Read data package in memory address.
            bs = burst_read(priority, packet, mem_addr, 3, true); 
            // Update Memory Machine
            if (button == packet->button_id &&
                packet->button_pushed != 0) {
                if (button == X[count]) { // Button push was correct
                    count++;
                    *data = 1;
                    bs = burst_write(priority, data, B_ADDR(button), 1, true);
                } else { // Button push was wrong, reset lights
                    count = 0;
                    bs = burst_write(priority, reset, B_ADDR(1), 9, true);
                }
            }
            // Clear status word. (release lock)
            *data = 0;
            bs = burst_write(priority, data, my_addr);
        }
        wait(CONTROL_SLEEP, sc_ms);
    }

    delete data;
    delete [] reset; 
    delete packet;
}
