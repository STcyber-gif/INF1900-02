/*
* Nom: pb1
* Copyright -
* License -
* Description: Ce programme itere a travers une sequence de couleurs DEL
* au fur et a mesure que le bouton-poussoir est appuye et relache.
* Ce programme detecte les interactions du bouton a l'aide d'un systeme
* d'interruption au lieu d'un sondage.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/

/*
+------+--------------+--------------+--------+
| Etat | BoutonAppuye | Etat Suivant | Sortie |
+------+--------------+--------------+--------+
| INIT | 0            | INIT         | R      |
| INIT | 1            | A1           | A      |
| A1   | 0            | R1           | V      |
| A1   | 1            | A1           | A      |
| R1   | 0            | R1           | V      |
| R1   | 1            | A2           | R      |
| A2   | 0            | R2           | E      |
| A2   | 1            | A2           | R      |
| R2   | 0            | R2           | E      |
| R2   | 1            | A3           | V      |
| A3   | 0            | INIT         | R      |
| A3   | 1            | A3           | V      |
+------+--------------+--------------+--------+
*/


// Identification matérielle :
// connecter le port A 1 avec la borne positive de la DEL et
// connecter le Port A 2 avec la borne négative de la DEL.
#define DEL_ETEINT 0x00
#define DEL_ROUGE 0x01
#define DEL_VERT 0x02

#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>


const uint8_t PIN_BOUTON = (1 << DDD2);
const uint8_t PINS_DEL = (1 << DDA0) | (1 << DDA1);

enum class EtatSequence: uint8_t { ETAT_INITIAL, APPUYE1, RELACHE1, APPUYE2, RELACHE2, APPUYE3 };
enum class EtatDel : uint8_t { ETEINT, ROUGE, VERT, AMBRE };

volatile bool gEtatBouton = false; // vrai si appuye, faux sinon.


ISR (INT0_vect) {
  _delay_ms (30); // laisser un délai avant de confirmer la réponse du bouton-poussoir.
  gEtatBouton = PIND & PIN_BOUTON;
  EIFR |= (1 << INTF0); // Empecher le ISR d'etre appelle une seconde fois a cause du rebond du bouton.
}

void initialisation () {
  cli (); // cli est une routine qui bloque toutes les interruptions.

  // ports en mode sortie (1)
  DDRA |= PINS_DEL;

  // ports en mode entree (0)
  DDRD &= ~PIN_BOUTON;

  // cette procédure ajuste le registre EIMSK
  // de l’ATmega324PA pour permettre les interruptions externes
  EIMSK |= (1 << INT0);

  // il faut sensibiliser les interruptions externes aux
  // changements de niveau du bouton-poussoir
  // en ajustant le registre EICRA
  EICRA |= (1 << ISC00);
  //EICRA &= ~(1 << ISC01); // valeur initiale est zero (voir documentation)

  sei (); // sei permet de recevoir à nouveau des interruptions.
}

void setCouleurDel(uint8_t couleur) { PORTA = couleur; }
bool estDelCouleur(uint8_t couleur) { return PORTA & couleur; }

int main()
{
  initialisation();

  bool estBoutonAppuye = false;
  EtatSequence etatSequence = EtatSequence::ETAT_INITIAL;
  EtatDel etatDel = EtatDel::ROUGE;

  while (true){
    
    estBoutonAppuye = gEtatBouton;

    switch (etatSequence){
      case (EtatSequence::ETAT_INITIAL):
        if (estBoutonAppuye){
          etatSequence = EtatSequence::APPUYE1;
          etatDel = EtatDel::AMBRE;
        }
        break;
      case (EtatSequence::APPUYE1):
        if (!estBoutonAppuye){
          etatSequence = EtatSequence::RELACHE1;
          etatDel = EtatDel::VERT;
        }
        break;
      case (EtatSequence::RELACHE1):
        if (estBoutonAppuye){
          etatSequence = EtatSequence::APPUYE2;
          etatDel = EtatDel::ROUGE;
        }
        break;
      case (EtatSequence::APPUYE2):
        if (!estBoutonAppuye){
          etatSequence = EtatSequence::RELACHE2;
          etatDel = EtatDel::ETEINT;
        }
        break;
      case (EtatSequence::RELACHE2):
        if (estBoutonAppuye){
          etatSequence = EtatSequence::APPUYE3;
          etatDel = EtatDel::VERT;
        }
        break;
      case (EtatSequence::APPUYE3):
        if (!estBoutonAppuye){
          etatSequence = EtatSequence::ETAT_INITIAL;
          etatDel = EtatDel::ROUGE;
        }
        break;
    }

    switch(etatDel){
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
        if (estDelCouleur(DEL_ROUGE)) setCouleurDel(DEL_VERT);
        else setCouleurDel(DEL_ROUGE);
        break;
    }
  }

  return 0;
}