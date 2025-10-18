#include "Led.h"

Led::Led(uint8_t* ddr, uint8_t pos){
   ddr_ = ddr;
   pin_ = 1 << pos;

   *ddr_ |= pin;
}

Led::~Led(){
   *ddr_ &= ~pin_; 
}

void Led::turnOn() { *(ddr_ + 1) |= pin_; }
void Led::turnOff() { *(ddr_ + 1) &= ~pin_; }

bool Led::isOn() { return *(ddr_ + 1) & pin; }