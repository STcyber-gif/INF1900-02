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

#ifndef TIMER0_H
#define TIMER0_H

#include <avr/io.h>

/*
 * Classe Timer0:
 *
 */

class Timer0
{
public:

   enum class CompareMatchOutputMode {NORMAL, TOGGLE, CLEAR, SET};

   enum class WaveformGenerationMode {NORMAL  , PHASE_CORRECT_PWM     , CTC          ,
                                      FAST_PWM, PHASE_CORRECT_PWM_OCRA, FAST_PWM_OCRA};
   
   enum class ClockSelect {NO_CLOCK_SOURCE, PRESCALER_0   , PRESCALER_8                , PRESCALER_64              ,
                           PRESCALER_256  , PRESCALER_1024, EXTERNAL_CLOCK_FALLING_EDGE, EXTERNAL_CLOCK_RISING_EDGE};

   Timer0();
   ~Timer0();

   setCompareMatchOutputModeA(CompareMatchOutputMode CMOMA);
   setCompareMatchOutputModeB(CompareMatchOutputMode CMOMB);

   setWaveformGenerationMode(WaveformGenerationMode WGM);

   setClockSelect(ClockSelect CS);

   setCounter(uint8_t counterValue);

   setCompareValueA(uint8_t compareValue);
   setCompareValueB(uint8_t compareValue);

   enableInterruptMatchA();
   disableInterruptMatchA();

   enableInterruptMatchB();
   disableInterruptMatchB();

   enableInterruptOverflow();
   disableInterruptOverflow();

   reset();

private:
   // Donnees membres - aucun
};

#endif /* TIMER0_H */
