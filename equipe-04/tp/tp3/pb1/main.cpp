#define F_CPU 8000000ul
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

/* ===== Constantes ===== */
const uint8_t RedLedMask = (uint8_t)(1u << PA1);   // ROUGE sur PA1
const uint8_t GreenLedMask = (uint8_t)(1u << PA0); // VERT  sur PA0
const uint8_t BothMask = (uint8_t)(RedLedMask | GreenLedMask);

const uint8_t ButtonMask = (uint8_t)(1u << PD2); // Bouton sur PD2 (pas de pull-up)
const uint16_t PWM_PERIOD_US = 1250;             // 800 Hz -> 1,25 ms
const uint8_t LEVELS = 100;                      // 100 → 0 %
const uint8_t PERIODS_PER_LEVEL = 24;            // ~3,0 s (101 * 24 * 1,25 ms)

/* ===== Helpers LED ===== */
static inline void ledOff() { PORTA &= (uint8_t)~BothMask; }
static inline void ledRed() { PORTA = (PORTA & (uint8_t)~BothMask) | RedLedMask; }
static inline void ledGreen() { PORTA = (PORTA & (uint8_t)~BothMask) | GreenLedMask; }

/* ===== Attente variable (évite _delay_us(variable)) ===== */
static inline void delayVariableUs(uint16_t duree)
{
    while (duree--)
    {
        _delay_us(1);
    } // constante → OK pour avr-libc
}

/* ===== Fade générique (une seule exécution) ===== */
static inline void fadeLed(uint8_t ledMask)
{
    for (int16_t level = LEVELS; level >= 0; --level)
    {
        uint16_t dureeOnMicroSec = (uint16_t)((level * PWM_PERIOD_US) / 100);
        uint16_t dureeOffMicroSec = (uint16_t)(PWM_PERIOD_US - dureeOnMicroSec);

        for (uint8_t p = 0; p < PERIODS_PER_LEVEL; ++p)
        {
            if (dureeOnMicroSec > 0)
            {
                PORTA = ledMask; // ON
                delayVariableUs(dureeOnMicroSec);
            }
            PORTA = 0; // OFF
            if (dureeOffMicroSec > 0)
            {
                delayVariableUs(dureeOffMicroSec);
            }
        }
    }
    PORTA = 0; // éteindre à la fin
}

int main(void)
{
    // Config sorties LED
    DDRA |= (1 << PA0) | (1 << PA1);
    // Bouton en entrée (pas de pull-up)
    DDRD &= (uint8_t)~ButtonMask;
    ledOff();

    enum class NextColor
    {
        Red,
        Green
    };
    NextColor next = NextColor::Red;

    while (1)
    {
        // --- 1) Attendre un PRESS (polarité à ajuster : ici appui = (PIND & ButtonMask) != 0) ---
        while ((PIND & ButtonMask) == 0)
        {
            _delay_ms(1);
        } // assurer "au repos"
        // détection press
        while (1)
        {
            if ((PIND & ButtonMask) != 0)
            {                  // appui détecté
                _delay_ms(20); // anti-rebond press
                if ((PIND & ButtonMask) != 0)
                    break; // confirmé
            }
            _delay_ms(1);
        }

        // --- 2) Attendre le RELEASE pour n’exécuter qu’une seule fade par appui ---
        while ((PIND & ButtonMask) != 0)
        {
            _delay_ms(1);
        } // attendre relâchement
        _delay_ms(20); // anti-rebond release

        // --- 3) Exécuter UNE SEULE fade selon la phase courante ---
        if (next == NextColor::Red)
        {
            fadeLed(RedLedMask);
            next = NextColor::Green; // bascule la phase
        }
        else
        {
            fadeLed(GreenLedMask);
            next = NextColor::Red;
        }

        // Retour à l’attente d’un nouveau press→release
        _delay_ms(1);
    }
}
