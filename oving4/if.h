#ifndef   	IF_H_
# define   	IF_H_


// Defines
#define CONTROL_ADDRESS 0
#define FREELOC 4*1
#define BUTTON_STATUS_ADDRESS 4*2

#define BUTTON_BASE_PRIORITY 5

#define BUFFER_START 4*18
#define BUFFER_MAX_ADDRESS 4*1024
#define BUTTON_STATUS_OFFSET 4

#define CLK_PERIOD 10
#define ADAPTER_LIGHT_WAIT 10000
#define ADAPTER_PUSH_WAIT 10000
#define CONTROL_SLEEP 200000
#define INTERNAL_UNIT SC_US

#define BUTTON_PUSH_INTERVAL 500000
#define TR 10000
#define TR_UNIT SC_US


struct data_packet_t {
    data_packet_t() : packet_size(3){};
    data_packet_t(int _id, int _push)
    : packet_size(3)
    , button_id(_id)
    , button_pushed(_push)
    , dummy(NULL)
    {};
  int packet_size;
  int button_id;
  int button_pushed;
  int * dummy;
};

// Interface for Button
class button_if : public sc_interface {
public:
  virtual void light (bool) = 0;
};

class control_if : public sc_interface {
public:
  virtual void push (int) = 0;
};


#endif 	    /* !IF_H_ */
