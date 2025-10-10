/*
* Nom: pb2
* Copyright -
* License -
* Description: ...
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, octobre 2025
*/

#define F_CPU 8000000UL  // 8 MHz
#include "can.h"
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Identification matérielle :
// connecter le port B 1 avec la borne positive de la DEL et
// connecter le Port B 2 avec la borne négative de la DEL.
const uint8_t DEL_ETEINT = 0x00;
const uint8_t DEL_ROUGE  = 0x01;
const uint8_t DEL_VERT   = 0x02;
const uint8_t PINS_DEL   = (1 << DDB0) | (1 << DDB1);
const uint8_t PIN_PHOTORESISTANCE = 1 << DDA0;

//                                                 - Si Vref a cinq volts  - Vref avec l'usb 
// Noir total -> 4.34 v  (5 / 4.34 * 2 ^ 10) = 888 = 0x0378                = 0x03F9     
// Normal     -> 4.35 v  (5 / 4.35 * 2 ^ 10) = 891 = 0x037b                = 0x03FD
const uint16_t LIMITE_LUMIERE_BASSE_BONNE = 0x03F9;
const uint16_t LIMITE_LUMIERE_BONNE_FORTE = 0x03FD;

enum class EtatDel { ETEINT, ROUGE, VERT, AMBRE };

volatile EtatDel gEtatDel = EtatDel::ETEINT;

void initialisation () {
  cli (); // cli est une routine qui bloque toutes les interruptions.

  // ports en mode sortie (1)
  DDRB |= PINS_DEL;

  // ports en mode entree (0)
  DDRD &= ~PIN_PHOTORESISTANCE;

  sei (); // sei permet de recevoir à nouveau des interruptions.
}

void setCouleurDel(uint8_t couleur) { PORTB = couleur; }
bool estDelCouleur(uint8_t couleur) { return PORTB & couleur; }

int main()
{
  initialisation();

  can convertisseur;
  uint16_t resultat;

  while (true){

    resultat = convertisseur.lecture(PIN_PHOTORESISTANCE);

    if (resultat < LIMITE_LUMIERE_BASSE_BONNE) {
      gEtatDel = EtatDel::VERT;
    }
    else if (resultat < LIMITE_LUMIERE_BONNE_FORTE){
      gEtatDel = EtatDel::AMBRE;
    }
    else{
      gEtatDel = EtatDel::ROUGE;
    }
    
    switch(gEtatDel){
      case (EtatDel::ETEINT):
        setCouleurDel(DEL_ETEINT);
        break;
      
      case (EtatDel::ROUGE):
        setCouleurDel(DEL_ROUGE);
        break;

      case (EtatDel::VERT):
        setCouleurDel(DEL_VERT);
        break;

      case (EtatDel::AMBRE):
        setCouleurDel(estDelCouleur(DEL_ROUGE) ? DEL_VERT : DEL_ROUGE);
        break;
    }
  }

  return 0;
}