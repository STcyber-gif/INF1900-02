/*
 * Nom: RS232.h
 * Description: Classe pour la communication série UART0 sur ATmega324PA
 *              Basé sur le code du TP5 pb2 et pb3
 *              Configuration: 2400 bauds, 8 bits, 1 stop bit, sans parité
 * 
 * Auteur: Sidi Mohamed Toure
 * Date: Octobre 2025
 * Version: 1.0
 */

#ifndef RS232_H
#define RS232_H

#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdint.h>

/**
 * Classe RS232
 * Gère la communication série via UART0
 * 
 * Utilisation:
 *   RS232 uart;
 *   uart.transmettreChaine("Hello World\n");
 */
class RS232 {
public:
    /**
     * Constructeur
     * Initialise automatiquement l'UART à 2400 bauds
     */
    RS232();

    /**
     * Transmet un octet via UART
     * @param donnee L'octet à transmettre
     */
    void transmettreOctet(uint8_t donnee);

    /**
     * Transmet une chaîne de caractères ASCII
     * @param chaine Chaîne terminée par '\0'
     */
    void transmettreChaine(const char* chaine);

    /**
     * Transmet un tableau d'octets (données binaires)
     * @param donnees Pointeur vers les données
     * @param taille Nombre d'octets à transmettre
     */
    void transmettreTableau(const uint8_t* donnees, uint16_t taille);

    /**
     * Transmet un nombre en texte ASCII
     * @param nombre Nombre à transmettre (0-65535)
     */
    void transmettreNombre(uint16_t nombre);

    /**
     * Reçoit un octet via UART (bloquant)
     * @return L'octet reçu
     */
    uint8_t recevoirOctet();

    /**
     * Vérifie si des données sont disponibles
     * @return true si des données sont en attente
     */
    bool donneesDisponibles();

private:
    /**
     * Initialise les registres UART0
     * Appelé automatiquement par le constructeur
     */
    void initialisation();

    // Constantes de configuration
    static const uint8_t UBRR_VALUE_H = 0x00;
    static const uint8_t UBRR_VALUE_L = 0xCF;  // 2400 bauds @ 8MHz
};

#endif // RS232_H// TODO
