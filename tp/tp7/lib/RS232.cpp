/*
 * Nom: RS232.cpp
 * Description: Implémentation de la classe RS232 pour communication série
 * 
 * Auteur: Frederic Gaudreault et Ethan Curnier
 * Date: Octobre 2025
 */

#include "RS232.h"

// Constructeur - initialise l'UART automatiquement
RS232::RS232() {
    initialisation();
}

// Initialisation des registres UART0
void RS232::initialisation() {
    // Configuration du baud rate à 2400 bauds
    // Valeurs données pour éviter les calculs complexes
    UBRR0H = UBRR_VALUE_H;
    UBRR0L = UBRR_VALUE_L;
    
    // Réinitialiser UCSR0A
    UCSR0A = 0;
    
    // Permettre la réception (RXEN0) et la transmission (TXEN0)
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Format des trames: 8 bits de données, 1 stop bit, sans parité
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);
}

// Transmet un seul octet via UART
void RS232::transmettreOctet(uint8_t donnee) {
    // Attendre que le registre de transmission soit vide (UDRE0 = 1)
    while (!(UCSR0A & (1 << UDRE0)));
    
    // Placer la donnée dans le registre, elle sera transmise automatiquement
    UDR0 = donnee;
}

// Transmet une chaîne de caractères terminée par '\0'
void RS232::transmettreChaine(const char* chaine) {
    // Parcourir la chaîne jusqu'au caractère nul
    for (uint16_t i = 0; chaine[i] != '\0'; i++) {
        transmettreOctet(chaine[i]);
    }
}

// Transmet un tableau d'octets de taille spécifiée
void RS232::transmettreTableau(const uint8_t* donnees, uint16_t taille) {
    // Transmettre chaque octet du tableau
    for (uint16_t i = 0; i < taille; i++) {
        transmettreOctet(donnees[i]);
    }
}

// Transmet un nombre en texte ASCII
void RS232::transmettreNombre(uint16_t nombre) {
    char buffer[6];  // Max 5 chiffres + '\0' pour uint16_t
    uint8_t i = 0;
    
    // Cas spécial : zéro
    if (nombre == 0) {
        transmettreOctet('0');
        return;
    }
    
    // Extraire les chiffres (en ordre inverse)
    uint16_t temp = nombre;
    while (temp > 0) {
        buffer[i++] = '0' + (temp % 10);
        temp /= 10;
    }
    
    // Transmettre dans le bon ordre
    while (i > 0) {
        transmettreOctet(buffer[--i]);
    }
}

// Reçoit un octet via UART (fonction bloquante)
uint8_t RS232::recevoirOctet() {
    // Attendre qu'une donnée soit disponible (RXC0 = 1)
    while (!(UCSR0A & (1 << RXC0)));
    
    // Lire et retourner la donnée reçue
    return UDR0;
}

// Vérifie si des données sont disponibles pour lecture
bool RS232::donneesDisponibles() {
    // Retourne true si le flag RXC0 est levé (données reçues)
    return (UCSR0A & (1 << RXC0));
}
