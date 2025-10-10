/*
 * Classe permettant (...)
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1900
 *
 * Frederic Gaudreault, Ethan Curnier, Yoan Blime et (...)
 * 2025
 *
 * Code qui n'est sous aucune license.
 */

#ifndef TIMER1_H
#define TIMER1_H

#include <avr/io.h>

/*
 * Classe Timer1:
 *  
 */

class Timer1
{
public:

   enum class CompareMatchOutputMode {NORMAL, TOGGLE, CLEAR, SET};

   enum class WaveformGenerationMode {NORMAL, CTC_OCRA, CTC_ICR, PHASE_CORRECT_PWM_8BIT, PHASE_CORRECT_PWM_9BIT, PHASE_CORRECT_PWM_10BIT,
                                      FAST_PWM_8BIT, FAST_PWM_9BIT, FAST_PWM_10BIT, PHASE_CORRECT_PWM_ICR, PHASE_CORRECT_PWM_OCRA,
                                      PHASE_AND_FREQUENCY_CORRECT_PWM_ICR, PHASE_AND_FREQUENCY_CORRECT_PWM_OCRA, FAST_PWM_ICR, FAST_PWM_OCRA};

   enum class ClockSelect {NO_CLOCK_SOURCE, PRESCALER_0, PRESCALER_8, PRESCALER_64, PRESCALER_256, PRESCALER_1024, CLOCK_ON_FALLING_EDGE, CLOCK_ON_RISING_EDGE};

   Timer1();
   ~Timer1();

   setCompareMatchOutputModeA(CompareMatchOutputMode CMOMA);
   setCompareMatchOutputModeB(CompareMatchOutputMode CMOMB);

   setWaveformGenerationMode(WaveformGenerationMode WGM);

   setClockSelect(ClockSelect CS);

   setCounter(uint16_t counterValue);

   setCompareValueA(uint16_t compareValue);
   setCompareValueB(uint16_t compareValue);

   enableInterruptMatchA();
   disableInterruptMatchA();

   enableInterruptMatchB();
   disableInterruptMatchB();

   enableInterruptOverflow();
   disableInterruptOverflow();

private:
   // Donnees membres - aucun
};

#endif /* TIMER1_H */