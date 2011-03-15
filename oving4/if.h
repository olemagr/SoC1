#ifndef   	IF_H_
# define   	IF_H_


// Defines
#define ADAPTER_UPDATE_PERIOD 100
#define BUTTON_STATUS_ADDRESS 10
#define FREELOC 1

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
