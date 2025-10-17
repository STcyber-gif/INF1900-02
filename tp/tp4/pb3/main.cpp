#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

constexpr uint8_t OC1A_MASK = (1u << PD5);
constexpr uint8_t OC1B_MASK = (1u << PD4);

static inline void wait_2s()
{
    for (uint16_t i = 0; i < 200; ++i)
        _delay_ms(10);
}

static inline void initIO()
{

    DDRD |= OC1A_MASK | OC1B_MASK;

    PORTD &= ~(OC1A_MASK | OC1B_MASK);
}

void ajustementPwm(uint8_t dutyA, uint8_t dutyB)
{

    TCCR1A |= (1 << WGM10);
    TCCR1A &= ~(1 << WGM11);
    TCCR1B &= ~(1 << WGM12);
    TCCR1B &= ~(1 << WGM13);

    TCCR1B &= ~((1 << CS12) | (1 << CS10));
    TCCR1B |= (1 << CS11);

    TCCR1C = 0;

    if (dutyA >= 255)
    {

        TCCR1A &= ~((1 << COM1A1) | (1 << COM1A0)); // COM1A1:0 = 00
        /* Forcer PD5 = 1 */
        PORTD |= OC1A_MASK;
    }
    else
    {

        TCCR1A = (TCCR1A & ~((1 << COM1A1) | (1 << COM1A0))) | (1 << COM1A1);

        OCR1A = dutyA;
    }

    if (dutyB >= 255)
    {
        TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0)); // COM1B1:0 = 00
        PORTD |= OC1B_MASK;                         // PD4 = 1
    }
    else
    {
        TCCR1A = (TCCR1A & ~((1 << COM1B1) | (1 << COM1B0))) | (1 << COM1B1);
        OCR1B = dutyB;
    }
}

int main()
{
    initIO();

    ajustementPwm(0, 0);
    wait_2s();

    ajustementPwm(64, 64);
    wait_2s();

    ajustementPwm(128, 128);
    wait_2s();

    ajustementPwm(192, 192);
    wait_2s();

    ajustementPwm(255, 255);
    ­­ //  wait_2s();

        for (;;)
    { /* on reste à 100% */
    }
}
