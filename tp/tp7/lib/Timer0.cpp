#include "Timer0.h"

Timer0::Timer0() { reset(); }

Timer0::~Timer0() { reset(); }

Timer0::setCompareMatchOutputModeA(Timer0::CompareMatchOutputMode CMOMA){
   switch(CMOMA){
      case(Timer0::CompareMatchOutputMode::NORMAL):
         TCCR0A &= ~(1 << COM0A1 | 1 << COM0A0);
         break;
      case(Timer0::CompareMatchOutputMode::TOGGLE):
         TCCR0A &= ~(1 << COM0A1);
         TCCR0A |= (1 << COM0A0);
         break;
      case(Timer0::CompareMatchOutputMode::CLEAR):
         TCCR0A |= (1 << COM0A1);
         TCCR0A &= ~(1 << COM0A0);
         break;
      case(Timer0::CompareMatchOutputMode::SET):
         TCCR0A |= (1 << COM0A1 | 1 << COM0A0);
         break;
   }
}

Timer0::setCompareMatchOutputModeB(Timer0::CompareMatchOutputMode CMOMB){
   switch(CMOMB){
      case(Timer0::CompareMatchOutputMode::NORMAL):
         TCCR0A &= ~(1 << COM0B1 | 1 << COM0B0);
         break;
      case(Timer0::CompareMatchOutputMode::TOGGLE):
         TCCR0A &= ~(1 << COM0B1);
         TCCR0A |= (1 << COM0B0);
         break;
      case(Timer0::CompareMatchOutputMode::CLEAR):
         TCCR0A |= (1 << COM0B1);
         TCCR0A &= ~(1 << COM0B0);
         break;
      case(Timer0::CompareMatchOutputMode::SET):
         TCCR0A |= (1 << COM0B1 | 1 << COM0B0);
         break;
    }
}

Timer0::setWaveformGenerationMode(Timer0::WaveformGenerationMode WGM){
   switch(WGM){
      case(Timer0::WaveformGenerationMode::NORMAL):
         TCCR0A &= ~(1 << WGM00 | 1 << WGM01);
         TCCR0B &= ~(1 << WGM02);
         break;
      case(Timer0::WaveformGenerationMode::PHASE_CORRECT_PWM):
         TCCR0A |= 1 << WGM00;
         TCCR0A &= ~(1 << WGM01);
         TCCR0B &= ~(1 << WGM02);
         break;
      case(Timer0::WaveformGenerationMode::CTC):
         TCCR0A &= ~(1 << WGM00);
         TCCR0A |= 1 << WGM01;
         TCCR0B &= ~(1 << WGM02);
         break;
      case(Timer0::WaveformGenerationMode::FAST_PWM):
         TCCR0A |= 1 << WGM00 | 1 << WGM01;
         TCCR0B &= ~(1 << WGM02);
         break;
      case(Timer0::WaveformGenerationMode::PHASE_CORRECT_PWM_OCRA):
         TCCR0A |= 1 << WGM00;
         TCCR0A &= ~(1 << WGM01);
         TCCR0B |= 1 << WGM02;
         break;
      case(Timer0::WaveformGenerationMode::FAST_PWM_OCRA):
         TCCR0A |= 1 << WGM00 | 1 << WGM01;
         TCCR0B |= 1 << WGM02;
         break;
   }
}

Timer0::setClockSelect(Timer0::ClockSelect CS){
   switch(CS){
      case(Timer0::ClockSelect::NO_CLOCK_SOURCE):
         TCCR0B &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
         break;
      case(Timer0::ClockSelect::PRESCALER_0):
         TCCR0B |= 1 << CS00;
         TCCR0B &= ~(1 << CS01 | 1 << CS01);
         break;
      case(Timer0::ClockSelect::PRESCALER_8):
         TCCR0B |= 1 << CS01;
         TCCR0B &= ~(1 << CS00 | 1 << CS02);
         break;
      case(Timer0::ClockSelect::PRESCALER_64):
         TCCR0B |= 1 << CS00 | 1 << CS01;
         TCCR0B &= ~(1 << CS02);
         break;
      case(Timer0::ClockSelect::PRESCALER_256):
         TCCR0B &= ~(1 << CS00 | 1 << CS01);
         TCCR0B |= 1 << CS02;
         break;
      case(Timer0::ClockSelect::PRESCALER_1024):
         TCCR0B |= 1 << CS00 | 1 << CS02;
         TCCR0B &= ~(1 << CS01);
         break;
      case(Timer0::ClockSelect::EXTERNAL_CLOCK_FALLING_EDGE):
         TCCR0B &= ~(1 << CS00);
         TCCR0B |= 1 << CS01 | 1 << CS02;
         break;
      case(Timer0::ClockSelect::EXTERNAL_CLOCK_RISING_EDGE):
         TCCR0B |= 1 << CS00 | 1 << CS01 | 1 << CS02;
         break;
   }
}

Timer0::setCounter(uint8_t counterValue) { TCNT0 = counterValue; }

Timer0::setCompareValueA(uint8_t compareValue) { OCR0A = compareValue; }
Timer0::setCompareValueB(uint8_t compareValue) { OCR0B = compareValue; }

Timer0::enableInterruptMatchA() { TIMSK0 |= 1 << OCIE0A; }
Timer0::disableInterruptMatchA() { TIMSK0 &= ~(1 << OCIE0A); }

Timer0::enableInterruptMatchB() { TIMSK0 |= 1 << OCIE0B; }
Timer0::disableInterruptMatchB() { TIMSK0 &= ~(1 << OCIE0B); }

Timer0::enableInterruptOverflow() { TIMSK0 |= 1 << TOIE0; }
Timer0::disableInterruptOverflow() { TIMSK0 &= ~(1 << TOIE0); }

reset(){
   TCCR0A = 0;
   TCCR0B = 0;
   TCNT0 = 0;
   OCR0A = 0;
   OCR0B = 0;
   TIMSK0 = 0;
}