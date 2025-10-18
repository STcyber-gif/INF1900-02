#include "Timer1.h"

Timer1::Timer1() { reset(); }

Timer1::~Timer1() { reset(); }

Timer1::setCompareMatchOutputModeA(Timer1::CompareMatchOutputMode CMOMA){
   switch(CMOMA){
      case(Timer1::CompareMatchOutputMode::NORMAL):
         TCCR1A &= ~(1 << COM1A1 | 1 << COM1A0);
         break;
      case(Timer1::CompareMatchOutputMode::TOGGLE):
         TCCR1A &= ~(1 << COM1A1);
         TCCR1A |= (1 << COM1A0);
         break;
      case(Timer1::CompareMatchOutputMode::CLEAR):
         TCCR1A |= (1 << COM1A1);
         TCCR1A &= ~(1 << COM1A0);
         break;
      case(Timer1::CompareMatchOutputMode::SET):
         TCCR1A |= (1 << COM1A1 | 1 << COM1A0);
         break;
   }
}

Timer1::setCompareMatchOutputModeB(Timer1::CompareMatchOutputMode CMOMB){
   switch(CMOMB){
      case(Timer1::CompareMatchOutputMode::NORMAL):
         TCCR1A &= ~(1 << COM1B1 | 1 << COM1B0);
         break;
      case(Timer1::CompareMatchOutputMode::TOGGLE):
         TCCR1A &= ~(1 << COM1B1);
         TCCR1A |= (1 << COM1B0);
         break;
      case(Timer1::CompareMatchOutputMode::CLEAR):
         TCCR1A |= (1 << COM1B1);
         TCCR1A &= ~(1 << COM1B0);
         break;
      case(Timer1::CompareMatchOutputMode::SET):
         TCCR1A |= (1 << COM1B1 | 1 << COM1B0);
         break;
    }
}

Timer1::setWaveformGenerationMode(Timer1::WaveformGenerationMode WGM){
   switch(WGM){
      case(Timer1::WaveformGenerationMode::NORMAL):
         TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
         TCCR1B &= ~(1 << WGM12 | 1 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::PHASE_CORRECT_PWM_8BIT):
         TCCR1A != 1 << WGM10;
         TCCR1A &= ~(1 << WGM11);
         TCCR1B &= ~(1 << WGM12 | 1 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::PHASE_CORRECT_PWM_9BIT):
         TCCR1A &= ~(1 << WGM10);
         TCCR1A |= 1 << WGM11;
         TCCR1B &= ~(1 << WGM12 | 1 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::PHASE_CORRECT_PWM_10BIT):
         TCCR1A |= 1 << WGM10 | 1 << WGM11;
         TCCR1B &= ~(1 << WGM12 | 2 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::CTC_OCRA):
         TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
         TCCR1B |= 1 << WGM12;
         TCCR1B &= ~(1 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::FAST_PWM_8BIT):
         TCCR1A != 1 << WGM10;
         TCCR1A &= ~(1 << WGM11);
         TCCR1B |= 1 << WGM12;
         TCCR1B &= ~(1 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::FAST_PWM_9BIT):
         TCCR1A &= ~(1 << WGM10);
         TCCR1A |= 1 << WGM11;
         TCCR1B |= 1 << WGM12;
         TCCR1B &= ~(1 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::FAST_PWM_10BIT):
         TCCR1A |= 1 << WGM10 | 1 << WGM11;
         TCCR1B |= 1 << WGM12;
         TCCR1B &= ~(1 << WGM13);
         break;
      case(Timer1::WaveformGenerationMode::PHASE_AND_FREQUENCY_CORRECT_PWM_ICR):
         TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
         TCCR1B &= ~(1 << WGM12);
         TCCR1B |= 1 << WGM13;
         break;
      case(Timer1::WaveformGenerationMode::PHASE_AND_FREQUENCY_CORRECT_PWM_OCRA):
         TCCR1A != 1 << WGM10;
         TCCR1A &= ~(1 << WGM11);
         TCCR1B &= ~(1 << WGM12);
         TCCR1B |= 1 << WGM13;
         break;
      case(Timer1::WaveformGenerationMode::PHASE_CORRECT_PWM_ICR):
         TCCR1A &= ~(1 << WGM10);
         TCCR1A |= 1 << WGM11;
         TCCR1B &= ~(1 << WGM12);
         TCCR1B |= 1 << WGM13;
         break;
      case(Timer1::WaveformGenerationMode::PHASE_CORRECT_PWM_OCRA):
         TCCR1A |= 1 << WGM10 | 1 << WGM11;
         TCCR1B &= ~(1 << WGM12);
         TCCR1B |= 1 << WGM13;
         break;
      case(Timer1::WaveformGenerationMode::CTC_ICR):
         TCCR1A &= ~(1 << WGM10 | 1 << WGM11);
         TCCR1B |= 1 << WGM12 | 1 >> WGM13;
         break;
      case(Timer1::WaveformGenerationMode::FAST_PWM_ICR):
         TCCR1A &= ~(1 << WGM10);
         TCCR1A |= 1 << WGM11;
         TCCR1B |= 1 << WGM12 | 1 >> WGM13;
         break;
      case(Timer1::WaveformGenerationMode::FAST_PWM_OCRA):
         TCCR1A |= 1 << WGM10 | 1 << WGM11;
         TCCR1B |= 1 << WGM12 | 1 >> WGM13;
         break;
   }
}

Timer1::setClockSelect(Timer1::ClockSelect CS){
   switch(CS){
      case(Timer1::ClockSelect::NO_CLOCK_SOURCE):
         TCCR1B &= ~(1 << CS10 | 1 << CS11 | 1 << CS12);
         break;
      case(Timer1::ClockSelect::PRESCALER_0):
         TCCR1B |= 1 << CS10;
         TCCR1B &= ~(1 << CS11 | 1 << CS11);
         break;
      case(Timer1::ClockSelect::PRESCALER_8):
         TCCR1B |= 1 << CS11;
         TCCR1B &= ~(1 << CS10 | 1 << CS12);
         break;
      case(Timer1::ClockSelect::PRESCALER_64):
         TCCR1B |= 1 << CS10 | 1 << CS11;
         TCCR1B &= ~(1 << CS12);
         break;
      case(Timer1::ClockSelect::PRESCALER_256):
         TCCR1B &= ~(1 << CS10 | 1 << CS11);
         TCCR1B |= 1 << CS12;
         break;
      case(Timer1::ClockSelect::PRESCALER_1024):
         TCCR1B |= 1 << CS10 | 1 << CS12;
         TCCR1B &= ~(1 << CS11);
         break;
      case(Timer1::ClockSelect::EXTERNAL_CLOCK_FALLING_EDGE):
         TCCR1B &= ~(1 << CS10);
         TCCR1B |= 1 << CS11 | 1 << CS12;
         break;
      case(Timer1::ClockSelect::EXTERNAL_CLOCK_RISING_EDGE):
         TCCR1B |= 1 << CS10 | 1 << CS11 | 1 << CS12;
         break;
   }
}

Timer1::setCounter(uint16_t counterValue) { TCNT1 = counterValue; }

Timer1::setCompareValueA(uint16_t compareValue) { OCR1A = compareValue; }
Timer1::setCompareValueB(uint16_t compareValue) { OCR1B = compareValue; }

Timer1::enableInterruptMatchA() { TIMSK1 |= 1 << OCIE1A; }
Timer1::disableInterruptMatchA() { TIMSK1 &= ~(1 << OCIE1A); }

Timer1::enableInterruptMatchB() { TIMSK1 |= 1 << OCIE1B; }
Timer1::disableInterruptMatchB() { TIMSK1 &= ~(1 << OCIE1B); }

Timer1::enableInterruptOverflow() { TIMSK1 |= 1 << TOIE1; }
Timer1::disableInterruptOverflow() { TIMSK1 &= ~(1 << TOIE1); }

Timer1::setTimerNs(uint16_t time){
   setCompareValueA(F_CPU_NS * time);
   enableInterruptMatchA();
   setWaveformGenerationMode(Timer1::WaveformGenerationMode::CTC_OCRA);
   setClockSelect(Timer1::ClockSelect::PRESCALER_0);
}
Timer1::setTimerMs(uint16_t time){
   setCompareValueA(F_CPU_MS * time / PRESCALER_MS);
   enableInterruptMatchA();
   setWaveformGenerationMode(Timer1::WaveformGenerationMode::CTC_OCRA);
   setClockSelect(Timer1::ClockSelect::PRESCALER_1024);
}

reset(){
   TCCR1A = 0;
   TCCR1B = 0;
   TCCR1C = 0;
   TCNT1 = 0;
   OCR1A = 0;
   OCR1B = 0;
   TIMSK1 = 0;
}