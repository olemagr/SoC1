#ifndef   	IF_H_
# define   	IF_H_


// Defines
#define ADAPTER_UPDATE_PERIOD 4*100
#define BUTTON_STATUS_ADDRESS 4*10
#define BUTTON_STATUS_OFFSET 4*4
#define FREELOC 4*500
#define BUFFER_MAX_ADDRESS 4*1000
#define CONTROL_ADDRESS 4*1

struct data_packet_t {
  int packet_size = 3;
  int button_id;
  int button_pushed;
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
