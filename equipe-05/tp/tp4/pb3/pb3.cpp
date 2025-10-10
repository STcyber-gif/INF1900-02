/*
* Nom: pb3
* Copyright -
* License -
* Description: Ce programme génère une onde PWM de façon matérielle
* et envoie le résultat au moteurs du robot. On générera des signaux
* PWM de 0%, 25%, 50% et 75% et 100% pour des durées de 2 secondes chacun.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/

#define SECONDE_EN_MS 1000
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Identification matérielle :
// connecter le port B 3 avec la pin de puissance du moteur droit et
// connecter le port B 4 avec la pin de puissance du moteur gauche
const uint8_t PINS_MOTEUR = (1 << DDB3) | (1 << DDB4);
const uint8_t NB_SECONDES = 2;
const uint8_t NB_INCREMENTATION = 4;
const uint8_t PUISSANCE_MAX = UINT8_MAX;
const uint8_t INCREMENT_PUISSANCE = PUISSANCE_MAX / NB_INCREMENTATION;
const uint16_t MINUTERIE_PRESCALER = 1024;

volatile bool gMinuterieExpiree = false; // vrai si expiree, faux sinon.

void initialisation () {
  cli (); // cli est une routine qui bloque toutes les interruptions.

  // ports en mode sortie (1)
  DDRB |= PINS_MOTEUR;

  sei (); // sei permet de recevoir à nouveau des interruptions.
}

void partirMinuterieMs (uint16_t duree) {
  gMinuterieExpiree = false;
  
  // interruption après la durée spécifiée
  TCNT1 = 0;
  OCR1A = F_CPU / SECONDE_EN_MS * duree / MINUTERIE_PRESCALER;

  // mode CTC du timer 1 avec horloge divisée par 1024
  TCCR1A = 0;
  TCCR1B = 1 << WGM12 | 1 << CS10 | 1 << CS12;
  TCCR1C = 0;

  // autoriser l'interruption lorsque comparaison entre TCNT3 et OCR3A
  TIMSK1 = 1 << OCIE1A;
}

ISR (TIMER1_COMPA_vect) {
  gMinuterieExpiree = true;
}


void ajustementPwm (uint8_t moteurDroit, uint8_t moteurGauche) {
  // mise à un des sorties OC1A et OC1B sur comparaison
  // réussie en mode PWM 8 bits, phase correcte
  // et valeur de TOP fixe à 0xFF (mode #1 de la table 16-5
  // page 130 de la description technique du ATmega324PA)
  OCR0A = moteurDroit;
  OCR0B = moteurGauche;

  // division d'horloge par 8 - implique une fréquence de PWM fixe
  TCCR0A = 1 << COM0A1 | 1 << COM0B1 | 1 << WGM00;
  TCCR0B = 1 << CS01;
}

int main()
{
  uint8_t puissance;

  while (true) {
    for (uint8_t j = 0; j <= NB_INCREMENTATION; ++j){
      puissance = INCREMENT_PUISSANCE * j;
      ajustementPwm(puissance, puissance);
      partirMinuterieMs(NB_SECONDES * SECONDE_EN_MS);
      do {} while (!gMinuterieExpiree);
    }
  }

  return 0;
}