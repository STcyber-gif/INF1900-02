/*
* Nom: pb1
* Copyright
* License
* Description: La DEL du robot est éteinte au départ. Ce programme fait en sorte que l'utilisateur
* doit appuyer et relâcher 3 fois le bouton-poussoir du robot avant que la DEL tourne au vert
* pendant exactement 2 secondes. Par la suite, on revient au départ pour pouvoir recommencer.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/

/*
+------+--------------+--------------+--------+
| Etat | BoutonAppuye | Etat Suivant | Sortie |
+------+--------------+--------------+--------+
| INIT | 0            | INIT         | 0      |
| INIT | 1            | A1           | 0      |
| A1   | 0            | R1           | 0      |
| A1   | 1            | A1           | 0      |
| R1   | 0            | R1           | 0      |
| R1   | 1            | A2           | 0      |
| A2   | 0            | R2           | 0      |
| A2   | 1            | A2           | 0      |
| R2   | 0            | R2           | 0      |
| R2   | 1            | A3           | 0      |
| A3   | 0            | R3           | 0      |
| A3   | 1            | A3           | 0      |
| R3   | 0            | INIT         | 1      |
| R3   | 1            | INIT         | 0      |
+------+--------------+--------------+--------+
*/

// Identification matérielle :
// connecter le port A 1 avec la borne positive de la DEL et
// connecter le Port A 2 avec la borne négative de la DEL.
#define DEL_ETEINT 0x00
#define DEL_VERT 0x02

#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>

#include <avr/io.h>

const uint8_t PIN_BOUTON = (1 << DDD2);
const uint8_t PINS_DEL = (1 << DDA0) | (1 << DDA1);

enum class EtatBouton: uint8_t { ETAT_INITIAL, APPUYE1, RELACHE1, APPUYE2, RELACHE2, APPUYE3, RELACHE3 };

void mettreDelVert() { PORTA = DEL_VERT; }
void eteindreDel() { PORTA = DEL_ETEINT; }

bool estBoutonAppuye() {
  static bool etaitAppuye = false;
  bool estAppuye = PIND & PIN_BOUTON;

  if (estAppuye == etaitAppuye) return estAppuye;

  _delay_ms(10);
  if (estAppuye != etaitAppuye) etaitAppuye = estAppuye;
  return etaitAppuye;
}

int main()
{
  // ports en mode sortie (1)
  DDRA |= PINS_DEL;

  // ports en mode entree (0)
  DDRD &= ~PIN_BOUTON;
  
  EtatBouton etatBouton = EtatBouton::ETAT_INITIAL;

  while (true){

    switch(etatBouton){
      case (EtatBouton::ETAT_INITIAL): 
        if (estBoutonAppuye()) etatBouton = EtatBouton::APPUYE1;
        break;
      case (EtatBouton::APPUYE1): 
        if (!estBoutonAppuye()) etatBouton = EtatBouton::RELACHE1;
        break;
      case (EtatBouton::RELACHE1):
        if (estBoutonAppuye()) etatBouton = EtatBouton::APPUYE2;
        break;
      case (EtatBouton::APPUYE2):
        if (!estBoutonAppuye()) etatBouton = EtatBouton::RELACHE2;
        break;
      case (EtatBouton::RELACHE2):
        if (estBoutonAppuye()) etatBouton = EtatBouton::APPUYE3;
        break;
      case (EtatBouton::APPUYE3):
        if (!estBoutonAppuye()) etatBouton = EtatBouton::RELACHE3;
        break;
      case (EtatBouton::RELACHE3):
        mettreDelVert();
        _delay_ms(2000);
        eteindreDel();
        etatBouton = EtatBouton::ETAT_INITIAL;
        break;
    }
  }

  return 0;
}