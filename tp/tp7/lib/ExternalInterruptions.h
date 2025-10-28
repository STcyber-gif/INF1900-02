/*
 * Nom: ExternalInterruptions.h
 * Description: Classe pour configurer les interruptions externes INT0, INT1, INT2
 *              Basé sur le code du TP4
 *              IMPORTANT: Ce module configure UNIQUEMENT les interruptions,
 *                         les ISR doivent être définies dans le code utilisateur
 * 
 * Auteur: Sidi Mohamed Toure
 * Date: Octobre 2025
 * Version: 1.0
 */

#ifndef EXTERNAL_INTERRUPTIONS_H
#define EXTERNAL_INTERRUPTIONS_H

#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

/**
 * Énumération des types de déclenchement d'interruption
 */
enum class TypeDeclenchement : uint8_t {
    BAS_NIVEAU = 0,      // Interruption sur niveau bas
    CHANGEMENT = 1,      // Interruption sur tout changement
    FRONT_DESCENDANT = 2,// Interruption sur front descendant
    FRONT_MONTANT = 3    // Interruption sur front montant
};

/**
 * Énumération des interruptions externes disponibles sur ATmega324PA
 * Les valeurs correspondent directement aux bits dans EIMSK
 */
enum class InterruptionExterne : uint8_t {
    INT0_EXT = INT0,  // Utilise directement la macro (valeur 0)
    INT1_EXT = INT1,  // Utilise directement la macro (valeur 1)
    INT2_EXT = INT2   // Utilise directement la macro (valeur 2)
};

/**
 * Classe ExternalInterruptions
 * Configure les interruptions externes INT0, INT1, INT2
 * 
 * ATTENTION: Cette classe NE contient PAS les ISR!
 * Vous devez définir vous-même:
 *   ISR(INT0_vect) { ... }
 *   ISR(INT1_vect) { ... }
 *   ISR(INT2_vect) { ... }
 * 
 * Note: Sur l'ATmega324PA, toutes les interruptions (INT0, INT1, INT2)
 *       sont configurées via le registre EICRA
 * 
 * Utilisation:
 *   ExternalInterruptions::configurer(
 *       InterruptionExterne::INT0_EXT,
 *       TypeDeclenchement::FRONT_DESCENDANT
 *   );
 *   ExternalInterruptions::activer(InterruptionExterne::INT0_EXT);
 *   sei(); // Activer les interruptions globales
 */
class ExternalInterruptions {
public:
    /**
     * Configure une interruption externe
     * @param interrupt L'interruption à configurer (INT0, INT1, INT2)
     * @param type Le type de déclenchement
     */
    static void configurer(InterruptionExterne interrupt, TypeDeclenchement type);

    /**
     * Active une interruption externe spécifique
     * @param interrupt L'interruption à activer
     */
    static void activer(InterruptionExterne interrupt);

    /**
     * Désactive une interruption externe spécifique
     * @param interrupt L'interruption à désactiver
     */
    static void desactiver(InterruptionExterne interrupt);

    /**
     * Active toutes les interruptions globalement
     * Équivalent à sei()
     */
    static void activerGlobal();

    /**
     * Désactive toutes les interruptions globalement
     * Équivalent à cli()
     */
    static void desactiverGlobal();

    /**
     * Vérifie si une interruption est active
     * @param interrupt L'interruption à vérifier
     * @return true si active, false sinon
     */
    static bool estActive(InterruptionExterne interrupt);

private:
    /**
     * Configure le type de déclenchement dans EICRA
     * Sur ATmega324PA, EICRA gère INT0, INT1 ET INT2
     * @param interrupt INT0, INT1 ou INT2
     * @param type Type de déclenchement
     */
    static void configurerEICRA(InterruptionExterne interrupt, TypeDeclenchement type);
};

#endif // EXTERNAL_INTERRUPTIONS_H// TODO
