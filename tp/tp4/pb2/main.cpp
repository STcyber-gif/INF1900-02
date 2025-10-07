#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

constexpr uint8_t RED = (1u << PA1);
constexpr uint8_t GREEN = (1u << PA0);
constexpr uint8_t BOTH = (RED | GREEN);

constexpr uint8_t BTN_MASK = (1u << PD2);

static volatile uint8_t gMinuterieExpiree = 0;
static volatile uint8_t gBoutonPoussoir = 0;
static inline void ledOff() { PORTA &= ~BOTH; }
static inline void ledRed() { PORTA = (PORTA & ~BOTH) | RED; }
static inline void ledGreen() { PORTA = (PORTA & ~BOTH) | GREEN; }

static inline void blinkRed100ms()
{
    PORTA = (PORTA & ~BOTH) | RED;
    _delay_ms(100);
    PORTA &= ~BOTH;
}

static inline void wait10s_blocking()
{
    for (uint16_t i = 0; i < 100; ++i)
        _delay_ms(100);
}

static inline void initIO()
{
    DDRA |= BOTH;
    DDRD &= ~BTN_MASK;

    ledOff();
}

constexpr uint16_t OCR1A_1S = 7811;

static inline void startWindow1s()
{
    gMinuterieExpiree = 0;

    TCCR1A = 0;
    TCNT1 = 0;
    OCR1A = OCR1A_1S;

    TIMSK1 = (1 << OCIE1A);
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
}

static inline void stopTimer1()
{
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    TIMSK1 &= ~(1 << OCIE1A);
}

static inline void initInt0()
{

    EICRA &= ~((1 << ISC01) | (1 << ISC00));
    EICRA |= ((1 << ISC01) | (1 << ISC00));

    EIFR |= (1 << INTF0);
    EIMSK |= (1 << INT0);
}

static inline void disableInt0()
{
    EIMSK &= ~(1 << INT0);
}

ISR(TIMER1_COMPA_vect)
{
    gMinuterieExpiree = 1;
}

ISR(INT0_vect)
{

    _delay_ms(20);
    if ((PIND & BTN_MASK) != 0)
    {
        gBoutonPoussoir = 1;
        disableInt0();
    }
    EIFR |= (1 << INTF0);
}

int main()
{
    cli();
    initIO();
    initInt0();
    sei();

    wait10s_blocking();

    blinkRed100ms();
    ledOff();

    gBoutonPoussoir = 0;
    gMinuterieExpiree = 0;
    startWindow1s();

    /*Attendre un seul événement : appui OU timer expiré */
    while (gBoutonPoussoir == 0 && gMinuterieExpiree == 0)
    {
        /* boucle d’attente passive : les ISRs mettront les flags à 1 */
    }

    /* Une réponse suffit : bloquer les IT et arrêter les sources */
    cli();
    stopTimer1();
    disableInt0();

    if (gBoutonPoussoir && !gMinuterieExpiree)
    {
        ledGreen();
    }
    else
    {
        ledRed();
    }

    /* 7) Rester figé */
    for (;;)
    { /* no-op */
    }
}
