// tp/tp7/exec/main.cpp
#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>  // ← Ajouté pour les ISR
#include <util/delay.h>

// Librairie de l'équipe
#include "../lib/BicoloredLed.h"
#include "../lib/can.h" // classe Can avec lecture(uint8_t)
#include "../lib/ExternalInterruptions.h"  // ← Ajouté
#include "../lib/debug.h"

// ==================== VARIABLES GLOBALES POUR LES ISR ====================
//Variables volatiles partagées entre ISR et main
volatile bool int0Triggered = false;
volatile bool int1Triggered = false;
volatile bool int2Triggered = false;
volatile uint8_t int0Counter = 0;
volatile uint8_t int1Counter = 0;
volatile uint8_t int2Counter = 0;

// ==================== ISR (Interrupt Service Routines) ====================

// ISR pour INT0 (PD2)
ISR(INT0_vect) {
    // Anti-rebond matériel
    _delay_ms(30);
    
    int0Triggered = true;
    int0Counter++;
}

// ISR pour INT1 (PD3)
ISR(INT1_vect) {
    // Anti-rebond matériel
    _delay_ms(30);
    
    int1Triggered = true;
    int1Counter++;
}

// ISR pour INT2 (PB2)
ISR(INT2_vect) {
    // Anti-rebond matériel
    _delay_ms(30);
    
    int2Triggered = true;
    int2Counter++;
}

// ==================== FONCTION PRINCIPALE (CODE ORIGINAL) ====================


int main()
{
    print("test 0x%x", &PORTC);

    // ========== CONFIGURATION DES INTERRUPTIONS (NOUVEAU) ==========
    
    // Configurer les broches des boutons en entrée avec pull-up
    DDRD &= ~((1 << PD2) | (1 << PD3));  // PD2 et PD3 en entrée
    DDRB &= ~(1 << PB2);                 // PB2 en entrée
    
    // Configurer INT0 sur front descendant (appui du bouton)
    ExternalInterruptions::configurer(
        InterruptionExterne::INT0_EXT,
        TypeDeclenchement::FRONT_DESCENDANT
    );
    ExternalInterruptions::activer(InterruptionExterne::INT0_EXT);
    
    // Configurer INT1 sur front descendant
    ExternalInterruptions::configurer(
        InterruptionExterne::INT1_EXT,
        TypeDeclenchement::FRONT_DESCENDANT
    );
    ExternalInterruptions::activer(InterruptionExterne::INT1_EXT);
    
    // Configurer INT2 sur front descendant
    ExternalInterruptions::configurer(
        InterruptionExterne::INT2_EXT,
        TypeDeclenchement::FRONT_DESCENDANT
    );
    ExternalInterruptions::activer(InterruptionExterne::INT2_EXT);
    
    // Activer les interruptions globales
    sei();
    
    // ========== CODE ORIGINAL (NON MODIFIÉ) ==========
    
    // LED bicolore/monocouleur sur PORTC, bit 0 (PC0)
    BicoloredLed led(&PORTC, 0, 1);
    
    // Convertisseur analogique-numérique (lecture sur ADC0 = PA0)
    can adc;

    for (;;)
    {
        // ========== TRAITEMENT DES INTERRUPTIONS (NOUVEAU) ==========
        
        //Vérifier si INT0 a été déclenchée
        if (int0Triggered) {
            // Action à effectuer lors de l'interruption INT0
            // Par exemple: faire clignoter la LED
            led.turnRed();
            _delay_ms(1000);
            led.turnOff();
            _delay_ms(100);
            
            int0Triggered = false;  // Réinitialiser le flag
        }
        
        // Vérifier si INT1 a été déclenchée
        if (int1Triggered) {
            // Action à effectuer lors de l'interruption INT1
            // Par exemple: clignoter rapidement
            for (uint8_t i = 0; i < 3; i++) {
                led.turnGreen();
                _delay_ms(50);
                led.turnOff();
                _delay_ms(50);
            }
            
            int1Triggered = false;  // Réinitialiser le flag
        }
        
        // Vérifier si INT2 a été déclenchée
        if (int2Triggered) {
            // Action à effectuer lors de l'interruption INT2
            // Par exemple: allumer pendant 1 seconde
            led.turnGreen();
            _delay_ms(1000);
            
            int2Triggered = false;  // Réinitialiser le flag
        }
        
        // ========== CODE ORIGINAL (NON MODIFIÉ) ==========
        
        // Lire ADC0 (0..1023)
        uint16_t v = adc.lecture(0);
        
        // Seuil simple : > mi-échelle => LED ON, sinon OFF
        if (v > 512)
            led.turnGreen();
        else
            led.turnOff();
    }
    
    // (jamais atteint)
    return 0;
}
