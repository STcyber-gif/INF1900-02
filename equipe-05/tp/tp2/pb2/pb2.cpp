/*
* Nom: pb2
* Copyright
* License
* Description: Ce programme fait en sorte que lorsque la carte mère démarre, la DEL libre
* doit s'allumer en rouge. Si le bouton-poussoir libre pour usage général est pesé, la DEL
* affiche la couleur ambre. Quand le bouton-poussoir est relâché, la DEL devient verte.
* Si le bouton est de nouveau pesé, la DEL prend la couleur rouge encore. Quand il est relâché,
* la DEL s'éteint. Si le bouton est de nouveau pesé, la DEL affiche la couleur verte.
* Quand il est relâché, la DEL tourne au rouge ce qui fait que la carte mère est de
* retour à son état initial et tout peut recommencer.
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

const uint8_t PIN_BOUTON = (1 << DDD2);
const uint8_t PINS_DEL = (1 << DDA0) | (1 << DDA1);

enum class EtatBouton: uint8_t { ETAT_INITIAL, APPUYE1, RELACHE1, APPUYE2, RELACHE2, APPUYE3 };

enum class EtatDel : uint8_t { ETEINT, ROUGE, VERT, AMBRE };

void mettreDelVert() { PORTA = DEL_VERT; }
void mettreDelRouge() { PORTA = DEL_ROUGE; }
void eteindreDel() { PORTA = DEL_ETEINT; }

bool estDelRouge() { return PORTA & DEL_ROUGE; }

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
  EtatDel etatDel = EtatDel::ROUGE;

  while (true){

    switch (etatBouton){
      case (EtatBouton::ETAT_INITIAL):
        if (estBoutonAppuye()){
          etatBouton = EtatBouton::APPUYE1;
          etatDel = EtatDel::AMBRE;
        }
        break;
      case (EtatBouton::APPUYE1):
        if (!estBoutonAppuye()){
          etatBouton = EtatBouton::RELACHE1;
          etatDel = EtatDel::VERT;
        }
        break;
      case (EtatBouton::RELACHE1):
        if (estBoutonAppuye()){
          etatBouton = EtatBouton::APPUYE2;
          etatDel = EtatDel::ROUGE;
        }
        break;
      case (EtatBouton::APPUYE2):
        if (!estBoutonAppuye()){
          etatBouton = EtatBouton::RELACHE2;
          etatDel = EtatDel::ETEINT;
        }
        break;
      case (EtatBouton::RELACHE2):
        if (estBoutonAppuye()){
          etatBouton = EtatBouton::APPUYE3;
          etatDel = EtatDel::VERT;
        }
        break;
      case (EtatBouton::APPUYE3):
        if (!estBoutonAppuye()){
          etatBouton = EtatBouton::ETAT_INITIAL;
          etatDel = EtatDel::ROUGE;
        }
        break;
    }

    switch(etatDel){
      case (EtatDel::ETEINT):
        eteindreDel();
        break;
      case (EtatDel::ROUGE):
        mettreDelRouge();
        break;
      case (EtatDel::VERT):
        mettreDelVert();
        break;
      case (EtatDel::AMBRE):
        if (estDelRouge()) mettreDelVert();
        else mettreDelRouge();
        break;
    }
  }

  return 0;
}