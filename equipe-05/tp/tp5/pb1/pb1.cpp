/*
* Nom: pb1
* Copyright -
* License -
* Description: Ce programme écrit la chaîne de caractères
* «*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*» suivi d'un 0x00 en mémoire
* externe du ATmega324PA. La chaîne commencera à l'adresse 0x0000.
* Le programme peut aller relire la chaîne en question et comparer la chaîne
* de caractères envoyée à la mémoire et celle relue. Il affiche une DEL en
* vert si elles sont identiques et en rouge dans le cas contraire.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/

#include "memoire_24.h"
#include <util/delay.h>
#include <avr/io.h>

// Identification matérielle :
// connecter le port A 1 avec la borne positive de la DEL et
// connecter le Port A 2 avec la borne négative de la DEL.
const uint8_t DEL_ETEINT = 0x00;
const uint8_t DEL_ROUGE  = 0x01;
const uint8_t DEL_VERT   = 0x02;
const uint8_t PINS_DEL = (1 << DDA0) | (1 << DDA1);
const uint8_t DELAIS_POST_ECRITURE = 5; // ms
const uint16_t ADRESSE_MSG = 0x0000;
const char MSG[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*\0";

void setCouleurDel(uint8_t couleur) { PORTA = couleur; }
bool estDelCouleur(uint8_t couleur) { return PORTA & couleur; }

int main()
{
  // ports en mode sortie (1)
  DDRA |= PINS_DEL;

  Memoire24CXXX mem;
  mem.ecriture(ADRESSE_MSG, (uint8_t*)MSG, sizeof(MSG));
  _delay_ms(DELAIS_POST_ECRITURE);

  char msgLecture[sizeof(MSG)];
  mem.lecture(ADRESSE_MSG, (uint8_t*)msgLecture, sizeof(msgLecture));

  for (uint8_t i = 0; i < sizeof(MSG); ++i){
    if (MSG[i] != msgLecture[i]){
      setCouleurDel(DEL_ROUGE);
      return 0;
    }
  }

  setCouleurDel(DEL_VERT);
  return 0;
}