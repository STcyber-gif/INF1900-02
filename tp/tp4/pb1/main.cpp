#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>

constexpr uint8_t redLedMask = (1u << PA1);
constexpr uint8_t greenLedMask = (1u << PA0);
constexpr uint8_t bothLedMask = (redLedMask | greenLedMask);
constexpr uint8_t buttonMask = (1u << PD2);

static inline void ledOff() { PORTA &= ~bothLedMask; }
static inline void ledRed() { PORTA = (PORTA & ~bothLedMask) | redLedMask; }
static inline void ledGreen() { PORTA = (PORTA & ~bothLedMask) | greenLedMask; }

static inline void ledAmber()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        PORTA = redLedMask;
        _delay_ms(8);
        PORTA = greenLedMask;
        _delay_ms(10);
    }
}

static inline void initIo()
{
    DDRA |= bothLedMask; // LED en sortie
    DDRD &= ~buttonMask; // PD2 en entrée

    ledOff();
}

enum class P2State
{
    InitRed,
    WaitReleaseShowAmber,
    ShowGreen,
    WaitReleaseShowRed,
    ShowOff,
    WaitReleaseShowGreen,
    ReturnToInitOnRelease
},

    static volatile P2State gState = P2State::InitRed;

static inline void initInt0()
{
    cli();

    EICRA &= ~((1 << ISC01) | (1 << ISC00));
    EICRA |= (1 << ISC00);

    EIMSK |= (1 << INT0);
    EIFR |= (1 << INTF0);

    sei();
};

static inline void debounce_short() { _delay_ms(20); }

ISR(INT0_vect)
{
    debounce_short();
    const bool pressed = ((PIND & buttonMask) != 0);

    switch (gState)
    {
    case P2State::InitRed:
        if (pressed)
            gState = P2State::WaitReleaseShowAmber;
        break;

    case P2State::WaitReleaseShowAmber:
        if (!pressed)
            gState = P2State::ShowGreen;
        break;

    case P2State::ShowGreen:
        if (pressed)
            gState = P2State::WaitReleaseShowRed;
        break;

    case P2State::WaitReleaseShowRed:
        if (!pressed)
            gState = P2State::ShowOff;
        break;

    case P2State::ShowOff:
        if (pressed)
            gState = P2State::WaitReleaseShowGreen;
        break;

    case P2State::WaitReleaseShowGreen:
        if (!pressed)
            gState = P2State::ReturnToInitOnRelease;
        break;

    case P2State::ReturnToInitOnRelease:

        break;
    }

    EIFR |= (1 << INTF0);
}

int main()
{
    initIo();
    initInt0();

    for (;;)
    {
        switch (gState)
        {
        case P2State::InitRed:
            ledRed();
            break;
        case P2State::WaitReleaseShowAmber:
            ledAmber();
            break;
        case P2State::ShowGreen:
            ledGreen();
            break;
        case P2State::WaitReleaseShowRed:
            ledRed();
            break;
        case P2State::ShowOff:
            ledOff();
            break;
        case P2State::WaitReleaseShowGreen:
            ledGreen();
            break;
        case P2State::ReturnToInitOnRelease:
            ledRed();
            gState = P2State::InitRed;
            break;
        }
    }
}
