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

/* ===================== Constantes ===================== */
/* DEL bicolore : PA1 = rouge, PA0 = vert */
constexpr uint8_t RedLedMask = (1u << PA1);
constexpr uint8_t GreenLedMask = (1u << PA0);
constexpr uint8_t BothLedMask = (RedLedMask | GreenLedMask);

/* Bouton sur PD2 — sans pull-up interne */
constexpr uint8_t ButtonMask = (1u << PD2);

/* Anti-rebond */
constexpr uint16_t ConfirmDelayMs = 20;

/* ===================== Helpers LED ===================== */
static inline void ledOff() { PORTA &= ~BothLedMask; }
static inline void ledRed() { PORTA = (PORTA & ~BothLedMask) | RedLedMask; }
static inline void ledGreen() { PORTA = (PORTA & ~BothLedMask) | GreenLedMask; }

/* Ambre = alterner très vite entre rouge et vert */
static inline void ledAmber()
{
    for (uint8_t i = 0; i < 10; i++)
    {
        PORTA = RedLedMask; // rouge
        _delay_ms(8);
        PORTA = GreenLedMask; // vert
        _delay_ms(10);
    }
}

/* ===================== Init ===================== */
static inline void initIO()
{
    DDRA |= BothLedMask; // sorties DEL
    DDRD &= ~ButtonMask; // entrée bouton
    // pas de pull-up
    ledOff();
}

/* ===================== États PB2 ===================== */
enum class P2State
{
    INIT_Red,
    WaitRelease_ShowAmber, // ambre tant que pressé
    ShowGreen,
    WaitRelease_ShowRed, // rouge tant que pressé
    ShowOff,
    WaitRelease_ShowGreen, // vert tant que pressé
    ReturnToInit_OnRelease
};

int main()
{
    initIO();
    P2State state = P2State::INIT_Red;

    while (1)
    {
        // essaie avec !=0 ou ==0 selon ton câblage
        bool pressedNow = ((PIND & ButtonMask) != 0);

        switch (state)
        {

        case P2State::INIT_Red:
            ledRed();
            if (pressedNow)
            {
                _delay_ms(ConfirmDelayMs);
                if ((PIND & ButtonMask) != 0)
                {
                    state = P2State::WaitRelease_ShowAmber;
                }
            }
            break;

        case P2State::WaitRelease_ShowAmber:
            if (pressedNow)
            {
                ledAmber(); // boucle rapide rouge/vert
            }
            else
            {
                _delay_ms(ConfirmDelayMs);
                if ((PIND & ButtonMask) == 0)
                {
                    state = P2State::ShowGreen;
                }
            }
            break;

        case P2State::ShowGreen:
            ledGreen();
            if (pressedNow)
            {
                _delay_ms(ConfirmDelayMs);
                if ((PIND & ButtonMask) != 0)
                {
                    state = P2State::WaitRelease_ShowRed;
                }
            }
            break;

        case P2State::WaitRelease_ShowRed:
            if (pressedNow)
            {
                ledRed();
            }
            else
            {
                _delay_ms(ConfirmDelayMs);
                if ((PIND & ButtonMask) == 0)
                {
                    state = P2State::ShowOff;
                }
            }
            break;

        case P2State::ShowOff:
            ledOff();
            if (pressedNow)
            {
                _delay_ms(ConfirmDelayMs);
                if ((PIND & ButtonMask) != 0)
                {
                    state = P2State::WaitRelease_ShowGreen;
                }
            }
            break;

        case P2State::WaitRelease_ShowGreen:
            if (pressedNow)
            {
                ledGreen();
            }
            else
            {
                _delay_ms(ConfirmDelayMs);
                if ((PIND & ButtonMask) == 0)
                {
                    state = P2State::ReturnToInit_OnRelease;
                }
            }
            break;

        case P2State::ReturnToInit_OnRelease:
            ledRed();
            state = P2State::INIT_Red;
            break;
        }

        _delay_ms(1); // mini souffle
    }
}
