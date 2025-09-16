/*
 * ============================================================================
 *  Projet : TP2 INF1900 – PB1
 *  Auteurs : Sidi Mohammed TOURE, Alvynn Yoan BLIME
 *  Fichier : main.cpp
 * ----------------------------------------------------------------------------
 *  Description du programme
 *  --------------------------------
 *  La DEL commence éteinte. Un compteur d’appuis (press) est incrémenté à
 *  chaque détection d’un appui valide (anti-rebond minimal par double-lecture).
 *  Lorsque 3 pressions sont détectées, la DEL passe au vert pendant 2 secondes,
 *  puis s’éteint à nouveau (retour à l’état initial / début du programme).
 *
 *  Logique générale :
 *   - Compter uniquement les press (pas les releases explicites).
 *   - Anti-rebond : double vérification après un court délai (_delay_ms).
 *   - Temporisation 2 s : attente bloquante simple par tranches (_delay_ms).
 *
 *  Identification matérielle (câblage utilisé pour les tests)
 *  --------------------------------
 *   - DEL (simple) :
 *       • PA0 → Résistance (≈220 Ω à 1 kΩ) → Anode de la DEL
 *       • Cathode de la DEL → GND
 *   - Bouton poussoir :
 *       • PD2 relié à l’interrupteur (l’autre borne de l’interrupteur à VCC ou GND).
 * ============================================================================
                +---------------+---------------+---------------+--------+
                | current state | pressed       | next state    | output |
                +---------------+---------------+---------------+--------+
                | INIT          | 1 (1st press) | Countingpress | none   |
                +---------------+---------------+---------------+--------+
                | Countingpress | 1 (2nd press) | Countingpress | none   |
                +---------------+---------------+---------------+--------+
                | Countingpress | 1 (3rd press) | LedOpen       | green  |
                +---------------+---------------+---------------+--------+
                | LedOpen       | X             | INIT          | none   |
                +---------------+---------------+---------------+--------+
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

constexpr uint8_t greenLedMask = (1u << PA0);
constexpr uint8_t buttonMask = (1u << PD2);
constexpr uint16_t confirmDelayMs = 20;
constexpr uint16_t greenHoldMs = 2000;

static inline void ledOff() { PORTA &= ~greenLedMask; }
static inline void ledGreen() { PORTA |= greenLedMask; }

static inline void initIo()
{
    DDRA = 0xFF;
    DDRD = 0;
    ledOff();
}

enum class P1State
{
    Init,
    CountingPress,
    LedOpen
};

int main()
{
    initIo();
    P1State state = P1State::Init;
    uint8_t pressCount = 0;

    while (true)
    {
        bool pressed = ((PIND & buttonMask) != 0);

        switch (state)
        {
        case P1State::Init:
            ledOff();
            pressCount = 0;
            state = P1State::CountingPress;
            break;

        case P1State::CountingPress:
            if (pressed)
            {
                _delay_ms(confirmDelayMs);
                if ((PIND & buttonMask) != 0)
                {
                    pressCount++;
                    while ((PIND & buttonMask) != 0)
                    {
                        _delay_ms(5);
                    }
                    if (pressCount >= 3)
                    {
                        state = P1State::LedOpen;
                    }
                }
            }
            break;

        case P1State::LedOpen:
            ledGreen();
            {
                uint16_t elapsed = 0;
                while (elapsed < greenHoldMs)
                {
                    _delay_ms(10);
                    elapsed += 10;
                }
            }
            ledOff();
            state = P1State::Init;
            break;
        }
    }
}
