#include "Motors.h"

Motors::Motors() {
   DDRB |= 1 << DDB2 | 1 << DDB3 | 1 << DDB4 | 1 << DDB5;
   timer_.setCompareMatchOutputModeA(Timer0::CompareMatchOutputMode::CLEAR);
   timer_.setCompareMatchOutputModeB(Timer0::CompareMatchOutputMode::CLEAR);
   timer_.setWaveformGenerationMode(Timer0::WaveformGenerationMode::PHASE_CORRECT_PWM);
   timer_.setClockSelect(Timer0::ClockSelect::PRESCALER_8);
}

Motors::~Motors() { DDRB &= ~(1 << DDB2 | 1 << DDB3 | 1 << DDB4 | 1 << DDB5); }

Motors::setRightMotorForwards() { PORTB |= 1 << DDB2; }
Motors::setRightMotorBackwards() { PORTB &= ~(1 << DDB2); }

Motors::setLeftMotorForwards() { PORTB |= 1 << DDB5; }
Motors::setLeftMotorBackwards() { PORTB &= ~(1 << DDB5); }

Motors::setRightMotorSpeed(uint8_t speed) { timer_.setCompareValueA(speed); }
Motors::setLeftMotorSpeed(uint8_t speed) { timer_.setCompareValueB(speed); }