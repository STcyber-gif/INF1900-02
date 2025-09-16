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
 *       • PA1 → Résistance (≈220 Ω à 1 kΩ) → Anode de la DEL
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

/* ===================== Constantes ===================== */
constexpr uint8_t GreenLedMask = (1u << PA0); // LED verte sur PA0 (adapte si besoin)
constexpr uint8_t ButtonMask = (1u << PD2);   // bouton sur PD2
constexpr uint16_t ConfirmDelayMs = 20;       // anti-rebond simple
constexpr uint16_t GreenHoldMs = 2000;        // LED verte allumée 2 s

/* ===================== Helpers LED ===================== */
static inline void ledOff() { PORTA &= ~GreenLedMask; }
static inline void ledGreen() { PORTA |= GreenLedMask; }

static inline void initIO()
{
    DDRA = 0xFF; // LED en sortie
    DDRD = 0;    // bouton en entrée
    ledOff();
}

/* ===================== États ===================== */
enum class P1State
{
    INIT,
    CountingPress, // on compte les appuis
    LedOpen        // LED allumée 2 s, puis retour INIT
};

int main()
{
    initIO();
    P1State state = P1State::INIT;
    uint8_t pressCount = 0;

    while (1)
    {
        // lecture brute (essaie avec ==0 ou !=0 selon ton câblage)
        bool pressed = ((PIND & ButtonMask) != 0);

        switch (state)
        {
        case P1State::INIT:
            ledOff();
            pressCount = 0;
            state = P1State::CountingPress;
            break;

        case P1State::CountingPress:
            if (pressed)
            {
                _delay_ms(ConfirmDelayMs);
                if ((PIND & ButtonMask) != 0)
                { // encore appuyé ?
                    pressCount++;
                    // attendre le relâchement pour éviter de recompter le même appui
                    while ((PIND & ButtonMask) != 0)
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
                while (elapsed < GreenHoldMs)
                {
                    _delay_ms(10);
                    elapsed += 10;
                }
            }
            ledOff();
            state = P1State::INIT;
            break;
        }

        _delay_ms(1); // mini souffle
    }
}
