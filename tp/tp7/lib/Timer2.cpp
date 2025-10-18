#include "Timer2.h"

Timer2::Timer2() { reset(); }

Timer2::~Timer2() { reset(); }

Timer2::setCompareMatchOutputModeA(Timer2::CompareMatchOutputMode CMOMA){
   // wait until the TCCR2A register is done being asyncroniously written to.
   while(ASSR & 1 << TCR2AUB);

   switch(CMOMA){
      case(Timer2::CompareMatchOutputMode::NORMAL):
         TCCR2A &= ~(1 << COM2A1 | 1 << COM2A0);
         break;
      case(Timer2::CompareMatchOutputMode::TOGGLE):
         TCCR2A &= ~(1 << COM2A1);
         TCCR2A |= (1 << COM2A0);
         break;
      case(Timer2::CompareMatchOutputMode::CLEAR):
         TCCR2A |= (1 << COM2A1);
         TCCR2A &= ~(1 << COM2A0);
         break;
      case(Timer2::CompareMatchOutputMode::SET):
         TCCR2A |= (1 << COM2A1 | 1 << COM2A0);
         break;
   }
}

Timer2::setCompareMatchOutputModeB(Timer2::CompareMatchOutputMode CMOMB){
   // wait until the TCCR2A register is done being asyncroniously written to.
   while(ASSR & 1 << TCR2AUB);

   switch(CMOMB){
      case(Timer2::CompareMatchOutputMode::NORMAL):
         TCCR2A &= ~(1 << COM2B1 | 1 << COM2B0);
         break;
      case(Timer2::CompareMatchOutputMode::TOGGLE):
         TCCR2A &= ~(1 << COM2B1);
         TCCR2A |= (1 << COM2B0);
         break;
      case(Timer2::CompareMatchOutputMode::CLEAR):
         TCCR2A |= (1 << COM2B1);
         TCCR2A &= ~(1 << COM2B0);
         break;
      case(Timer2::CompareMatchOutputMode::SET):
         TCCR2A |= (1 << COM2B1 | 1 << COM2B0);
         break;
    }
}

Timer2::setWaveformGenerationMode(Timer2::WaveformGenerationMode WGM){
   // wait until the TCCR2A and TCCR2B registers are done being asyncroniously written to.
   while(ASSR & (1 << TCR2AUB | 1 << TCR2AUB));

   switch(WGM){
      case(Timer2::WaveformGenerationMode::NORMAL):
         TCCR2A &= ~(1 << WGM20 | 1 << WGM21);
         TCCR2B &= ~(1 << WGM22);
         break;
      case(Timer2::WaveformGenerationMode::PHASE_CORRECT_PWM):
         TCCR2A |= 1 << WGM20;
         TCCR2A &= ~(1 << WGM21);
         TCCR2B &= ~(1 << WGM22);
         break;
      case(Timer2::WaveformGenerationMode::CTC):
         TCCR2A &= ~(1 << WGM20);
         TCCR2A |= 1 << WGM21;
         TCCR2B &= ~(1 << WGM22);
         break;
      case(Timer2::WaveformGenerationMode::FAST_PWM):
         TCCR2A |= 1 << WGM20 | 1 << WGM21;
         TCCR2B &= ~(1 << WGM22);
         break;
      case(Timer2::WaveformGenerationMode::PHASE_CORRECT_PWM_OCRA):
         TCCR2A |= 1 << WGM20;
         TCCR2A &= ~(1 << WGM21);
         TCCR2B |= 1 << WGM22;
         break;
      case(Timer2::WaveformGenerationMode::FAST_PWM_OCRA):
         TCCR2A |= 1 << WGM20 | 1 << WGM21;
         TCCR2B |= 1 << WGM22;
         break;
   }
}

Timer2::setClockSelect(Timer2::ClockSelect CS){
   // wait until the TCCR2B register is done being asyncroniously written to.
   while(ASSR & 1 << TCR2BUB);

   switch(CS){
      case(Timer2::ClockSelect::NO_CLOCK_SOURCE):
         TCCR2B &= ~(1 << CS00 | 1 << CS01 | 1 << CS02);
         break;
      case(Timer2::ClockSelect::PRESCALER_0):
         TCCR2B |= 1 << CS00;
         TCCR2B &= ~(1 << CS01 | 1 << CS01);
         break;
      case(Timer2::ClockSelect::PRESCALER_8):
         TCCR2B |= 1 << CS01;
         TCCR2B &= ~(1 << CS00 | 1 << CS02);
         break;
      case(Timer2::ClockSelect::PRESCALER_32):
         TCCR2B |= 1 << CS00 | 1 << CS01;
         TCCR2B &= ~(1 << CS02);
         break;
      case(Timer2::ClockSelect::PRESCALER_64):
         TCCR2B &= ~(1 << CS00 | 1 << CS01);
         TCCR2B |= 1 << CS02;
         break;
      case(Timer2::ClockSelect::PRESCALER_128):
         TCCR2B |= 1 << CS00 | 1 << CS02;
         TCCR2B &= ~(1 << CS01);
         break;
      case(Timer2::ClockSelect::PRESCALER_256):
         TCCR2B &= ~(1 << CS00);
         TCCR2B |= 1 << CS01 | 1 << CS02;
         break;
      case(Timer2::ClockSelect::PRESCALER_1024):
         TCCR2B |= 1 << CS00 | 1 << CS01 | 1 << CS02;
         break;
   }
}

Timer2::setCounter(uint8_t counterValue){
   // wait until the counter register is done being asyncroniously written to.
   while (ASSR & 1 << TCN2UB);

   TCNT2 = counterValue;
}

Timer2::setCompareValueA(uint8_t compareValue){
   // wait until the compareA register is done being asyncroniously written to.
   while(ASSR & 1 << OCR2AUB);

   OCR2A = compareValue;
}
Timer2::setCompareValueB(uint8_t compareValue){
   // wait until the compareB register is done being asyncroniously written to.
   while(ASSR & 1 << OCR2BUB);

   OCR2B = compareValue;
}

Timer2::enableInterruptMatchA() { TIMSK2 |= 1 << OCIE2A; }
Timer2::disableInterruptMatchA() { TIMSK2 &= ~(1 << OCIE2A); }

Timer2::enableInterruptMatchB() { TIMSK2 |= 1 << OCIE2B; }
Timer2::disableInterruptMatchB() { TIMSK2 &= ~(1 << OCIE2B); }

Timer2::enableInterruptOverflow() { TIMSK2 |= 1 << TOIE2; }
Timer2::disableInterruptOverflow() { TIMSK2 &= ~(1 << TOIE2); }

Timer2::enableExternalClockInput(){
   ASSR |= 1 << EXCLK;
   ASSR |= 1 << AS2;
}
Timer2::disableExternalClockInput(){
   ASSR &= ~(1 << AS2);
   ASSR &= ~(1 << EXCLK);
}

reset(){
   TCCR2A = 0;
   TCCR2B = 0;
   TCNT2 = 0;
   OCR2A = 0;
   OCR2B = 0;
   TIMSK2 = 0;
}