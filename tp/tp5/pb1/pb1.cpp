/*
 * TP5 Problème 1 - Mémoire externe
 * Écrire et relire une chaîne en mémoire I²C
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "memoire_24.h" // Fourni par le cours

const uint8_t LED_RED_PIN = PA1;
const uint8_t LED_GREEN_PIN = PA0;

void ledRed() {
    PORTA = (PORTA & ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN))) 
            | (1 << LED_RED_PIN);
}

void ledGreen() {
    PORTA = (PORTA & ~((1 << LED_RED_PIN) | (1 << LED_GREEN_PIN))) 
            | (1 << LED_GREEN_PIN);
}

bool compareStrings(const char* str1, const uint8_t* str2, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        if (str1[i] != str2[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    DDRA |= (1 << LED_RED_PIN) | (1 << LED_GREEN_PIN);
    
    Memoire24CXXX mem;
    
    // Chaîne à écrire
    const char message[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
    const uint8_t length = sizeof(message); // Inclut le \0
    
    // Écrire en mémoire
    for (uint8_t i = 0; i < length; i++) {
        mem.ecriture(i, message[i]);
        _delay_ms(5); // Délai nécessaire après écriture
    }
    
    // Relire de la mémoire
    uint8_t buffer[length];
    for (uint8_t i = 0; i < length; i++) {
        mem.lecture(i, &buffer[i]);
    }
    
    // Comparer
    if (compareStrings(message, buffer, length)) {
        ledGreen(); // Succès
    } else {
        ledRed();   // Échec
    }
    
    while (true) {
        // Résultat affiché
    }
    
    return 0;
}