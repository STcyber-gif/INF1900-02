/*
 * Nom: ExternalInterruptions.cpp
 * Description: Implémentation de la classe ExternalInterruptions
 * 
 * Auteur: Sidi Mohamed Toure
 * Date: Octobre 2025
 */

#include "ExternalInterruptions.h"

// Configure une interruption externe
void ExternalInterruptions::configurer(InterruptionExterne interrupt, TypeDeclenchement type) {
    // Sur ATmega324PA, toutes les interruptions (INT0, INT1, INT2) utilisent EICRA
    configurerEICRA(interrupt, type);
}

// Configure EICRA pour INT0, INT1 et INT2
void ExternalInterruptions::configurerEICRA(InterruptionExterne interrupt, TypeDeclenchement type) {
    uint8_t decalage = 0;
    
    // Déterminer le décalage de bits
    if (interrupt == InterruptionExterne::INT0_EXT) {
        decalage = 0;  // Bits ISC00 et ISC01
    } else if (interrupt == InterruptionExterne::INT1_EXT) {
        decalage = 2;  // Bits ISC10 et ISC11
    } else if (interrupt == InterruptionExterne::INT2_EXT) {
        decalage = 4;  // Bits ISC20 et ISC21
    }
    
    // Effacer les bits existants
    EICRA &= ~((3 << decalage));
    
    // Configurer les nouveaux bits selon le type
    EICRA |= (static_cast<uint8_t>(type) << decalage);
}

// Active une interruption externe
void ExternalInterruptions::activer(InterruptionExterne interrupt) {
    // Utiliser directement la valeur de l'enum comme bit
    EIMSK |= (1 << static_cast<uint8_t>(interrupt));
}

// Désactive une interruption externe
void ExternalInterruptions::desactiver(InterruptionExterne interrupt) {
    // Utiliser directement la valeur de l'enum comme bit
    EIMSK &= ~(1 << static_cast<uint8_t>(interrupt));
}

// Active les interruptions globalement
void ExternalInterruptions::activerGlobal() {
    sei();  // Set Enable Interrupt
}

// Désactive les interruptions globalement
void ExternalInterruptions::desactiverGlobal() {
    cli();  // Clear Interrupt
}

// Vérifie si une interruption est active
bool ExternalInterruptions::estActive(InterruptionExterne interrupt) {
    // Utiliser directement la valeur de l'enum comme bit
    return (EIMSK & (1 << static_cast<uint8_t>(interrupt)));
}
