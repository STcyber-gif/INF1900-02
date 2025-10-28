/*
 * Nom: Button.h
 * Description: Classe minimaliste pour la gestion des boutons
 * Auteur: Sidi Mohamed Toure
 * Date: Octobre 2025
 */

#ifndef BUTTON_H
#define BUTTON_H

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

class Button {
public:
    /**
     * Constructeur
     * @param port Pointeur vers le registre PIN (ex: &PIND)
     * @param pin Numéro de la broche (0-7)
     * @param pullup true si pull-up activé (défaut), false si pull-down
     */
    Button(volatile uint8_t* port, uint8_t pin, bool pullup = true);
    
    bool estAppuye();
    bool frontMontant();
    bool frontDescendant();
    
private:
    volatile uint8_t* port_;
    uint8_t masque_;
    bool etatPrecedent_;
    bool pullup_;  // Mémorise si pull-up ou pull-down
};

#endif // BUTTON_H
