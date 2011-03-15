#ifndef   	IF_H_
# define   	IF_H_

// Interface for Button
class button_if : public sc_interface {
public:
  virtual void light (bool) = 0;
};

// Interface for Control, also used by bus
class control_if : public sc_interface {
public:
  virtual void push (int) = 0;
};

// Interface for Buss
class bus_if : public sc_interface {
public:
  virtual void light (int, bool) = 0;
};
#endif 	    /* !IF_H_ */
