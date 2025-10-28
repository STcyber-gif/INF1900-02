/*
 * Nom: Button.cpp
 * Description: Implémentation minimaliste de la classe Button
 *              avec gestion robuste de l'état
 * Auteur: Sidi Mohamed Toure
 * Date: Octobre 2025
 */

#include "Button.h"

Button::Button(volatile uint8_t* port, uint8_t pin)
    : port_(port), masque_(1 << pin), etatPrecedent_(false) {
}

bool Button::estAppuye() {
    // Lire deux fois avec délai pour anti-rebond
    bool etat1 = !(*port_ & masque_);
    _delay_ms(10);
    bool etat2 = !(*port_ & masque_);
    
    // Les deux lectures doivent être identiques pour être stable
    if (etat1 == etat2) {
        etatPrecedent_ = etat1;  // Mise à jour de l'état mémorisé
        return etat1;
    }
    
    // Si instable, retourner l'état mémorisé (sécurité)
    return etatPrecedent_;
}

bool Button::frontMontant() {
    bool etatActuel = estAppuye();
    
    // Front montant SEULEMENT si:
    // - État précédent était false (relâché)
    // - État actuel est true (appuyé)
    bool front = (!etatPrecedent_ && etatActuel);
    
    // CRITIQUE: Mettre à jour l'état APRÈS la détection
    // Cela empêche les détections multiples
    etatPrecedent_ = etatActuel;
    
    return front;
}

bool Button::frontDescendant() {
    bool etatActuel = estAppuye();
    
    // Front descendant SEULEMENT si:
    // - État précédent était true (appuyé)
    // - État actuel est false (relâché)
    bool front = (etatPrecedent_ && !etatActuel);
    
    // CRITIQUE: Mettre à jour l'état APRÈS la détection
    // Cela empêche les détections multiples
    etatPrecedent_ = etatActuel;
    
    return front;
}
