#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// ==== Sortie PWM : PB0 ====
#define PWM_PIN PB0

static inline void pwmHigh(void) { PORTB |= (1 << PWM_PIN); }
static inline void pwmLow(void) { PORTB &= ~(1 << PWM_PIN); }

// ==== Attente variable en microsecondes ====
// Boucle sur _delay_us(1) -> OK pour avr-libc, même avec une variable
static inline void delayVariableUs(uint16_t duree_us)
{
    while (duree_us--)
    {
        _delay_us(1);
    }
}

void runPwm(uint16_t duree_ms, uint16_t freq_hz, uint8_t duty_percent)
{
    // Période en µs,
    uint32_t period_us32 = (1000000UL + freq_hz / 2) / freq_hz;

    // ON / OFF en µs
    uint16_t on_us = (uint16_t)((period_us32 * duty_percent) / 100UL);
    uint16_t off_us = (uint16_t)(period_us32 - on_us);

    // Combien de périodes pour couvrir 'duree_ms'
    uint32_t cycles = ((uint32_t)duree_ms * (uint32_t)freq_hz) / 1000UL;

    // Cas 0% et 100%
    if (duty_percent == 0)
    {
        pwmLow();
        _delay_ms(2000);
        return;
    }
    if (duty_percent == 100)
    {
        pwmHigh();
        _delay_ms(2000);
        return;
    }

    // PWM "logiciel"
    for (uint32_t i = 0; i < cycles; ++i)
    {
        if (on_us)
        {
            pwmHigh();
            delayVariableUs(on_us);
        }
        pwmLow();
        if (off_us)
        {
            delayVariableUs(off_us);
        }
    }
}

int main(void)
{

    DDRB = 0xFF;

    const uint8_t DUTY[5] = {0, 25, 50, 75, 100};

    while (1)
    {
        // 60 Hz : 5 paliers × 2 s
        for (uint8_t i = 0; i < 5; ++i)
        {
            runPwm(2000, 60, DUTY[i]);
        }

        // 400 Hz : 5 paliers × 2 s
        for (uint8_t i = 0; i < 5; ++i)
        {
            runPwm(2000, 400, DUTY[i]);
        }
    }
}
