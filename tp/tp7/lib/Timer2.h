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

#ifndef TIMER2_H
#define TIMER2_H

#include <avr/io.h>

/*
 * Classe Timer2:
 *
 */

class Timer2
{
public:

   enum class CompareMatchOutputMode {NORMAL, TOGGLE, CLEAR, SET};

   enum class WaveformGenerationMode {NORMAL, CTC, PHASE_CORRECT_PWM, FAST_PWM, PHASE_CORRECT_PWM_OCRA, FAST_PWM_OCRA};
   
   enum class ClockSelect {NO_CLOCK_SOURCE, PRESCALER_0, PRESCALER_8, PRESCALER_32, PRESCALER_64, PRESCALER_128, PRESCALER_256, PRESCALER_1024};

   Timer2();
   ~Timer2();

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

   enableExternalClockInput();
   disableExternalClockInput();

private:
   // Donnees membres - aucun
};

#endif /* TIMER2_H */
