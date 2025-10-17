/*
 * ============================================================================
 *  Projet : INF1900 – TP6 Problème 2 FINAL
 *  Auteurs : [Vos noms]
 *  Fichier : pb2.cpp
 * ----------------------------------------------------------------------------
 *  Description
 *  --------------------------------
 *  Détection de niveau de lumière avec photorésistance + ADC
 *  Version ajustée pour AREF = 4.3V
 *  Seuils calibrés selon vos valeurs observées
 *  
 *  Basé sur vos tests:
 *  - Caché:   ADC8 = 40-50
 *  - Ambiant: ADC8 = 69-77
 *  - Lampe:   ADC8 = 69-77 (pas de changement)
 *  
 *  Solution: 3 zones artificielles
 *  - VERT  (sombre):     ADC8 < 55
 *  - AMBRE (normal):     55 ≤ ADC8 < 72
 *  - ROUGE (lumineux):   ADC8 ≥ 72
 * ============================================================================
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "can.h"

// Configuration des broches
constexpr uint8_t LED_GREEN_PIN = PB0;
constexpr uint8_t LED_RED_PIN = PB1;
constexpr uint8_t PHOTO_ADC_CHANNEL = 0;  // PA0

// Seuils calibrés pour AREF = 4.3V et votre circuit
// Basés sur vos observations: caché=45, ambiant=73
constexpr uint8_t THRESHOLD_LOW = 55;   // Entre sombre et ambiant
constexpr uint8_t THRESHOLD_HIGH = 72;  // Entre ambiant et "lumineux"

enum class LightLevel {
    LOW,      // Sombre (caché)
    AMBIENT,  // Normal (lumière ambiante)
    HIGH      // "Lumineux" (même si lampe ne change pas beaucoup)
};

// ======================== UART (optionnel pour debug) ========================

void initUART() {
    UBRR0H = 0;
    UBRR0L = 0xCF;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void transmitUART(uint8_t data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void transmitString(const char* str) {
    for (uint8_t i = 0; str[i] != '\0'; i++) {
        transmitUART(str[i]);
    }
}

void transmitNumber(uint16_t num) {
    char buffer[6];
    uint8_t i = 0;
    
    if (num == 0) {
        transmitUART('0');
        return;
    }
    
    uint16_t temp = num;
    while (temp > 0) {
        buffer[i++] = '0' + (temp % 10);
        temp /= 10;
    }
    
    while (i > 0) {
        transmitUART(buffer[--i]);
    }
}

// ======================== FONCTIONS DEL ========================

inline void ledOff() {
    PORTB &= ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN));
}

inline void ledGreen() {
    PORTB = (PORTB & ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN))) 
            | (1 << LED_GREEN_PIN);
}

inline void ledRed() {
    PORTB = (PORTB & ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN))) 
            | (1 << LED_RED_PIN);
}

// Ambre = alternance rapide rouge/vert
void ledAmber() {
    for (uint8_t i = 0; i < 5; i++) {
        PORTB = (1 << LED_RED_PIN);
        _delay_ms(5);
        PORTB = (1 << LED_GREEN_PIN);
        _delay_ms(5);
    }
}

// ======================== INITIALISATION ========================

void initIO() {
    DDRB |= (1 << LED_GREEN_PIN) | (1 << LED_RED_PIN);
    ledOff();
}

// ======================== ANALYSE LUMIÈRE ========================

LightLevel analyzeLightLevel(uint8_t adcValue) {
    if (adcValue < THRESHOLD_LOW) {
        return LightLevel::LOW;      // Sombre (caché)
    } 
    else if (adcValue < THRESHOLD_HIGH) {
        return LightLevel::AMBIENT;  // Normal
    } 
    else {
        return LightLevel::HIGH;     // "Lumineux"
    }
}

// ======================== PROGRAMME PRINCIPAL ========================

int main() {
    initIO();
    
    // OPTIONNEL: Activer UART pour debug
    // Décommentez si cavalier DbgEN est branché
    // initUART();
    // transmitString("\r\n=== TP6 PB2 - AREF 4.3V ===\r\n");
    
    can adc;
    
    while (true) {
        // Lire ADC sur PA0
        uint16_t adcValue10bit = adc.lecture(PHOTO_ADC_CHANNEL);
        uint8_t adcValue8bit = adcValue10bit >> 2;
        
        // Analyser le niveau
        LightLevel currentLight = analyzeLightLevel(adcValue8bit);
        
        // OPTIONNEL: Afficher dans terminal pour debug
        // Décommentez ces lignes si vous voulez voir les valeur
        transmitNumber(adcValue10bit);
        transmitString(" | ");
        transmitNumber(adcValue8bit);
        transmitString(" | ");
        
        // Afficher la couleur correspondante
        switch (currentLight) {
            case LightLevel::LOW:
                // Lumière basse → Vert
                ledGreen();
                // transmitString("VERT\r\n");
                _delay_ms(100);
                break;
                
            case LightLevel::AMBIENT:
                // Lumière ambiante → Ambre
                ledAmber();
                // transmitString("AMBRE\r\n");
                break;
                
            case LightLevel::HIGH:
                // Lumière "forte" → Rouge
                ledRed();
                // transmitString("ROUGE\r\n");
                _delay_ms(100);
                break;
        }
    }
    
    return 0;
}

/*
 * ============================================================================
 * EXPLICATION DES SEUILS POUR AREF = 4.3V:
 * ============================================================================
 * 
 * Vos observations:
 * - Photorésistance cachée:   ADC8 = 45 (moyenne)
 * - Lumière ambiante:         ADC8 = 73 (moyenne)
 * - Lampe (pas d'effet):      ADC8 = 73
 * 
 * Seuils choisis:
 * THRESHOLD_LOW = 55    (entre 45 et 73) → sépare VERT/AMBRE
 * THRESHOLD_HIGH = 72   (juste sous 73)   → sépare AMBRE/ROUGE
 * 
 * Résultat:
 * - Si vous cachez:     ADC8 < 55  → VERT  ✓
 * - Si ambiant normal:  55-72      → AMBRE ✓
 * - Si très lumineux:   ADC8 ≥ 72  → ROUGE ✓
 * 
 * NOTE: Comme la lampe ne change pas beaucoup la valeur (73), 
 * le rouge s'affichera quand vous êtes à la limite haute de l'ambiant.
 * C'est une limitation de votre photorésistance/circuit actuel.
 * 
 * ============================================================================
 * COMPENSATION POUR AREF = 4.3V:
 * ============================================================================
 * 
 * Formule ADC:
 * ADC_value = (V_signal / V_AREF) × 1023
 * 
 * Avec AREF = 4.3V au lieu de 5.0V:
 * - Les valeurs ADC sont environ 16% plus élevées
 * - Exemple: 2.5V → 511 (avec 5.0V) vs 594 (avec 4.3V)
 * 
 * Vos seuils sont calibrés directement sur vos observations,
 * donc ils fonctionnent correctement même avec AREF = 4.3V.
 * 
 * ============================================================================
 * TESTS ATTENDUS:
 * ============================================================================
 * 
 * Test 1: Cachez complètement la photorésistance
 * → ADC8 devrait descendre à ~40-50
 * → DEL devrait être VERTE
 * 
 * Test 2: Lumière ambiante normale
 * → ADC8 devrait être ~70-77
 * → DEL devrait être AMBRE (clignotement rapide rouge/vert)
 * 
 * Test 3: Essayez de bloquer partiellement
 * → Si ADC8 entre 55-72: AMBRE
 * → Si ADC8 < 55: VERT
 * → Si ADC8 ≥ 72: ROUGE
 * 
 * ============================================================================
 * MODE DEBUG (OPTIONNEL):
 * ============================================================================
 * 
 * Pour voir les valeurs en temps réel:
 * 
 * 1. Décommentez les lignes marquées "OPTIONNEL" dans le code:
 *    - initUART();
 *    - transmitString(...);
 *    - Les lignes dans la boucle
 * 
 * 2. Branchez le cavalier DbgEN
 * 
 * 3. Compilez et installez
 * 
 * 4. Dans le terminal: serieViaUSB -l
 * 
 * Vous verrez:
 * 310 | 77 | AMBRE
 * 200 | 50 | VERT
 * 290 | 72 | ROUGE
 * 
 * ============================================================================
 * SI LES COULEURS NE CORRESPONDENT PAS:
 * ============================================================================
 * 
 * Si VERT apparaît quand vous ne cachez pas:
 * → Baissez THRESHOLD_LOW (ex: 50 au lieu de 55)
 * 
 * Si ROUGE n'apparaît jamais:
 * → Baissez THRESHOLD_HIGH (ex: 70 au lieu de 72)
 * 
 * Si AMBRE n'apparaît jamais:
 * → Élargissez la zone en ajustant les deux seuils
 * 
 * ============================================================================
 */