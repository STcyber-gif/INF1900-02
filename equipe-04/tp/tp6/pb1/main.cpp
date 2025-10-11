/*
 * ============================================================================
 *  Projet : INF1900  –  TP6 Problème 1
 *  Auteurs : Alvyn Yoan BLIME  –  Sidi Mohamed TOURE
 *  Fichier : pb1.cpp
 * ----------------------------------------------------------------------------
 *  Description du programme
 *  --------------------------------
 *  Système de compteur avec bouton-poussoir externe sur breadboard:
 *
 *  Fonctionnement:
 *  1. Bouton enfoncé → Compteur incrémente 10 fois/seconde
 *  2. Bouton relâché OU compteur atteint 120 → DEL verte clignote 0.5s
 *  3. Attente 2 secondes
 *  4. DEL rouge clignote (compteur/2) fois à 2 fois/seconde
 *  5. DEL verte 1 seconde
 *  6. DEL éteinte → Retour état initial
 *
 *  Identification matérielle (breadboard)
 *  --------------------------------
 *   - Bouton-poussoir blanc : Connecté avec résistance 100K
 *   - Signal sortie bouton  : PD2 (cavalier IntEN RETIRÉ!)
 *   - DEL libre bicolore    : B0 (vert), B1 (rouge)
 *   - Condensateur 0.1µF    : Régulation alimentation
 * ============================================================================
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

// Configuration des broches
constexpr uint8_t BUTTON_PIN = PD2;
constexpr uint8_t LED_GREEN_PIN = PB0;
constexpr uint8_t LED_RED_PIN = PB1;

// Constantes du système
constexpr uint8_t MAX_COUNT = 120;           // Compte maximum
constexpr uint16_t INCREMENT_DELAY_MS = 100; // 100ms = 10 fois par seconde

// États de la machine à états
enum class State
{
    IDLE,           // État initial, attente bouton
    COUNTING,       // Comptage en cours
    FLASH_GREEN,    // Clignotement vert 0.5s
    WAIT_2_SECONDS, // Attente 2 secondes
    FLASH_RED,      // Clignotements rouges
    GREEN_1_SECOND, // Vert fixe 1 seconde
    DONE            // Terminé, retour à IDLE
};

// Fonctions de contrôle DEL
inline void ledOff()
{
    PORTB &= ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN));
}

inline void ledGreen()
{
    PORTB = (PORTB & ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN))) | (1 << LED_GREEN_PIN);
}

inline void ledRed()
{
    PORTB = (PORTB & ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN))) | (1 << LED_RED_PIN);
}

// Lecture du bouton avec anti-rebond
bool isButtonPressed()
{
    // PULL-DOWN (signal HIGH quand pressé)
    if (PIND & (1 << BUTTON_PIN))
    {
        _delay_ms(10);
        if (PIND & (1 << BUTTON_PIN))
        {
            return true;
        }
    }
    return false;
}

// Clignotement vert rapide (0.5 seconde)
void flashGreen()
{
    for (uint8_t i = 0; i < 5; i++)
    {
        ledGreen();
        _delay_ms(50);
        ledOff();
        _delay_ms(50);
    }
}

// Clignotement rouge (2 fois par seconde)
void flashRedTimes(uint8_t times)
{
    for (uint8_t i = 0; i < times; i++)
    {
        ledRed();
        _delay_ms(250);
        ledOff();
        _delay_ms(250);
    }
}

// Initialisation des ports
void initIO()
{
    DDRB |= (1 << LED_GREEN_PIN) | (1 << LED_RED_PIN);
    DDRD &= ~(1 << BUTTON_PIN);
    ledOff();
}

int main()
{
    initIO();

    State currentState = State::IDLE;
    uint8_t counter = 0;

    while (true)
    {
        bool isPressed = isButtonPressed();

        switch (currentState)
        {

        case State::IDLE:
            ledOff();
            counter = 0;

            // Attendre que le bouton soit pressé
            if (isPressed)
            {
                currentState = State::COUNTING;
            }
            break;

        case State::COUNTING:
            // Si bouton est pressé ET on n'a pas atteint le max
            if (isPressed && counter < MAX_COUNT)
            {
                counter++;
                _delay_ms(INCREMENT_DELAY_MS);
            }
            // Si bouton relâché OU max atteint
            else if (!isPressed || counter >= MAX_COUNT)
            {
                currentState = State::FLASH_GREEN;
            }
            break;

        case State::FLASH_GREEN:
            flashGreen();

            // INDICATEUR VISUEL: Afficher compteur/10 en clignotements lents
            // Exemple: compteur=60 → 6 clignotements verts
            ledOff();
            _delay_ms(500);
            for (uint8_t i = 0; i < (counter / 10); i++)
            {
                ledGreen();
                _delay_ms(400);
                ledOff();
                _delay_ms(400);
            }
            _delay_ms(500);

            currentState = State::WAIT_2_SECONDS;
            break;

        case State::WAIT_2_SECONDS:
            ledOff();
            _delay_ms(2000);
            currentState = State::FLASH_RED;
            break;

        case State::FLASH_RED:
        {
            uint8_t flashCount = counter / 2;
            if (flashCount > 0)
            {
                flashRedTimes(flashCount);
            }
            currentState = State::GREEN_1_SECOND;
        }
        break;

        case State::GREEN_1_SECOND:
            ledGreen();
            _delay_ms(1000);
            currentState = State::DONE;
            break;

        case State::DONE:
            ledOff();
            _delay_ms(100);
            // Retourner à IDLE pour recommencer
            currentState = State::IDLE;
            break;
        }
    }

    return 0;
}