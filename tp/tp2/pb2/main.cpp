/*
 * ============================================================================
 *  Projet : INF1900 – PB2
 *  Auteurs : Sidi Mohammed TOURE, Alvynn Yoan BLIME
 *  Fichier : main.cpp
 * ----------------------------------------------------------------------------
 *  Description du programme
 *  --------------------------------
 *  La DEL bicolore suit un cycle d’états en fonction des press/release du bouton :
 *    1) INIT : DEL ROUGE allumée.
 *    2) Premier press  → DEL AMBRE.
 *    3) Premier release → DEL VERTE.
 *    4) Deuxième press  → DEL ROUGE.
 *    5) Deuxième release → DEL éteinte (OFF).
 *    6) Troisième press  → DEL VERTE.
 *    7) Troisième release → retour à INIT (ROUGE) et le cycle recommence.
 *
 *  Remarques :
 *   - AMBRE = alternance très rapide entre ROUGE et VERTE (durées courtes via _delay_ms).
 *   - Anti-rebond minimal : double-lecture après un court délai (_delay_ms).
 *
 *  Identification matérielle (câblage utilisé pour les tests)
 *  --------------------------------
 *   - DEL bicolore (2 sorties) :
 *       • PA1 → Résistance (≈220 Ω à 1 kΩ) → Anode ROUGE de la DEL
 *       • PA0 → Résistance (≈220 Ω à 1 kΩ) → Anode VERTE de la DEL
 *       • Cathode(s) → GND
 *   - Bouton poussoir :
 *       • PD2 relié à l’interrupteur (l’autre borne de l’interrupteur à VCC ou GND).
 * ============================================================================
 * +------------------------+---------+------------------------+--------+
            | current state          | pressed | next state             | output |
            +------------------------+---------+------------------------+--------+
            | INIT                   | 0       | INIT                   | red    |
            +------------------------+---------+------------------------+--------+
            | INIT                   | 1       | Waitrelease_showamber  | amber  |
            +------------------------+---------+------------------------+--------+
            | Waitrelease_showamber  | 0       | showgreen              | green  |
            +------------------------+---------+------------------------+--------+
            | Waitrelease_showamber  | 1       | Waitrelease_showamber  | amber  |
            +------------------------+---------+------------------------+--------+
            | showgreen              | 0       | showgreen              | green  |
            +------------------------+---------+------------------------+--------+
            | showgreen              | 1       | Waitrelease_showred    | red    |
            +------------------------+---------+------------------------+--------+
            | Waitrelease_showred    | 0       | showoff                | none   |
            +------------------------+---------+------------------------+--------+
            | Waitrelease_showred    | 1       | Waitrelease_showred    | red    |
            +------------------------+---------+------------------------+--------+
            | showoff                | 0       | showoff                | none   |
            +------------------------+---------+------------------------+--------+
            | showoff                | 1       | waitrelease_showgreen  | green  |
            +------------------------+---------+------------------------+--------+
            | waitrelease_showgreen  | 0       | returntoINIT_onrelease | none   |
            +------------------------+---------+------------------------+--------+
            | waitrelease_showgreen  | 1       | waitrelease_showgreen  | green  |
            +------------------------+---------+------------------------+--------+
            | returntoINIT_onrelease | X       | INIT                   | red    |
            +------------------------+---------+------------------------+--------+

 */
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

constexpr uint8_t redLedMask = (1u << PA1);
constexpr uint8_t greenLedMask = (1u << PA0);
constexpr uint8_t bothLedMask = (redLedMask | greenLedMask);
constexpr uint8_t buttonMask = (1u << PD2);
constexpr uint16_t confirmDelayMs = 20;

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
    DDRA |= bothLedMask;
    DDRD &= ~buttonMask;
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
};

int main()
{
    initIo();
    P2State state = P2State::InitRed;

    while (true)
    {
        bool pressedNow = ((PIND & buttonMask) != 0);

        switch (state)
        {
        case P2State::InitRed:
            ledRed();
            if (pressedNow)
            {
                _delay_ms(confirmDelayMs);
                if ((PIND & buttonMask) != 0)
                {
                    state = P2State::WaitReleaseShowAmber;
                }
            }
            break;

        case P2State::WaitReleaseShowAmber:
            if (pressedNow)
            {
                ledAmber();
            }
            else
            {
                _delay_ms(confirmDelayMs);
                if ((PIND & buttonMask) == 0)
                {
                    state = P2State::ShowGreen;
                }
            }
            break;

        case P2State::ShowGreen:
            ledGreen();
            if (pressedNow)
            {
                _delay_ms(confirmDelayMs);
                if ((PIND & buttonMask) != 0)
                {
                    state = P2State::WaitReleaseShowRed;
                }
            }
            break;

        case P2State::WaitReleaseShowRed:
            if (pressedNow)
            {
                ledRed();
            }
            else
            {
                _delay_ms(confirmDelayMs);
                if ((PIND & buttonMask) == 0)
                {
                    state = P2State::ShowOff;
                }
            }
            break;

        case P2State::ShowOff:
            ledOff();
            if (pressedNow)
            {
                _delay_ms(confirmDelayMs);
                if ((PIND & buttonMask) != 0)
                {
                    state = P2State::WaitReleaseShowGreen;
                }
            }
            break;

        case P2State::WaitReleaseShowGreen:
            if (pressedNow)
            {
                ledGreen();
            }
            else
            {
                _delay_ms(confirmDelayMs);
                if ((PIND & buttonMask) == 0)
                {
                    state = P2State::ReturnToInitOnRelease;
                }
            }
            break;

        case P2State::ReturnToInitOnRelease:
            ledRed();
            state = P2State::InitRed;
            break;
        }
    }
}
