/*
* Nom: pb1
* Copyright
* License
* Description: Ce programme fait tourner une seule roue du robot a l'aide d'un signal PWM.
* Le signal sera d'abord de 60 Hz, puis de 400 Hz pour terminer.
* Pour les deux etapes du probleme, le signal PWM aura un rapport
* a/b de 0%, 25%, 50%, 75% puis de 100% pendant 2 secondes chaque.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/


// Identification matérielle :
// connecter le port A 1 avec la borne positive de la DEL et
// connecter le Port A 2 avec la borne négative de la DEL.
#define MOTEUR_ETEINT 0x00
#define MOTEUR_ALLUME 0x01

#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include <avr/io.h>

const uint8_t PINS_MOTEUR = (1 << DDB0) | (1 << DDB1);
const uint8_t NB_INCREMENTATION = 4;
const uint8_t NB_SECONDES = 2; // s
const uint16_t FREQUENCE_INITIALE = 60; // Hz
const uint16_t FREQUENCE_FINALE = 400; // Hz
const uint16_t PERIODE_INITIALE = 1000 / FREQUENCE_INITIALE * 1000; // us
const uint16_t PERIODE_FINALE = 1000 / FREQUENCE_FINALE * 1000; // us
const uint16_t DELAIS = 10; // us

void setMoteur(uint8_t etat) { PORTB = etat; }

void genererOndePwm(uint16_t periodeTotale, uint16_t periodeActive) {
  setMoteur(MOTEUR_ALLUME);
  for (uint16_t i = 0; i < periodeActive / DELAIS; ++i) {
    _delay_us(DELAIS);
  }

  setMoteur(MOTEUR_ETEINT);
  for (uint16_t i = 0; i < (periodeTotale - periodeActive) / DELAIS; ++i) {
    _delay_us(DELAIS);
  }
}

int main()
{
  // ports en mode sortie (1)
  DDRB |= PINS_MOTEUR;

  while (true) {
    for (uint8_t j = 0; j <= NB_INCREMENTATION; ++j){
      for (uint16_t i = 0; i < NB_SECONDES * FREQUENCE_INITIALE; ++i){
        genererOndePwm(PERIODE_INITIALE, PERIODE_INITIALE / NB_INCREMENTATION * j);
      }
    }
    
    for (uint8_t j = 0; j <= NB_INCREMENTATION; ++j){
      for (uint16_t i = 0; i < NB_SECONDES * FREQUENCE_FINALE; ++i){
        genererOndePwm(PERIODE_FINALE, PERIODE_FINALE / NB_INCREMENTATION * j);
      }
    }
  }

  return 0;
}