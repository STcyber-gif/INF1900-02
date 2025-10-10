/*
* Nom: pb1
* Copyright -
* License -
* Description: Ce programme attend qu l'utilisateur appuye sur le bouton sur le breadboard.
* Lorsque le bouton est enfoncé, un compteur qui incrémente 10 fois par seconde est activé.
* Quand le bouton est relâché ou lorsque le compteur atteint 120, la DEL du robot clignote
* vert pendant 1/2 seconde. Ensuite, la carte mère ne fait rien. Puis, deux secondes plus
* tard, la DEL rouge s'allume. Elle devra clignoter (compteur / 2) fois au rythme de 2
* fois par seconde. Ensuite, la DEL tourne au vert pendant une seconde. Finalement, elle
* s'éteint et le robot revient à son état original.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, octobre 2025
*/

#define SECONDE_EN_MS 1000
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Identification matérielle :
// connecter le port B 1 avec la borne positive de la DEL et
// connecter le Port B 2 avec la borne négative de la DEL.
const uint8_t DEL_ETEINT = 0x00;
const uint8_t DEL_ROUGE  = 0x01;
const uint8_t DEL_VERT   = 0x02;
const uint8_t PIN_BOUTON = (1 << DDD2);
const uint8_t PINS_DEL = (1 << DDB0) | (1 << DDB1);
const uint8_t NB_SECONDES_ATTENTE = 10; //s
const uint8_t MAX_COMPTEUR = 120;
const uint16_t PRESCALER = 1024;

enum class EtatSequence: uint8_t { ETAT_INITIAL, COMPTEUR, CLIGNOTE_VERT1, PAUSE, CLIGNOTE_ROUGE, CLIGNOTE_VERT2 };

volatile bool gMinuterieExpiree = true; // vrai si expiree, faux sinon.

void initialisation () {
  cli (); // cli est une routine qui bloque toutes les interruptions.

  // ports en mode sortie (1)
  DDRB |= PINS_DEL;

  // ports en mode entree (0)
  DDRD &= ~PIN_BOUTON;

  sei (); // sei permet de recevoir à nouveau des interruptions.
}

void setCouleurDel(uint8_t couleur) { PORTB = couleur; }

bool estBoutonAppuye() {
  static bool etaitAppuye = false;
  bool estAppuye = !(PIND & PIN_BOUTON);

  if (estAppuye == etaitAppuye) return estAppuye;

  _delay_ms(10);
  if (estAppuye != etaitAppuye) etaitAppuye = estAppuye;
  return estAppuye;
}

void partirMinuterieMs (uint16_t duree) {
  gMinuterieExpiree = false;
  
  // interruption après la durée spécifiée
  TCNT1 = 0;
  OCR1A = F_CPU / SECONDE_EN_MS * duree / PRESCALER;

  // mode CTC du timer 1 avec horloge divisée par 1024
  TCCR1A = 0;
  TCCR1B = 1 << WGM12 | 1 << CS10 | 1 << CS12;
  TCCR1C = 0;

  // autoriser la comparaison avec entre TCNT1 et OCR1A
  TIMSK1 = 1 << OCIE1A;
}

ISR (TIMER1_COMPA_vect) {
  gMinuterieExpiree = true;
}

int main()
{
  initialisation();

  uint8_t compteur = 0;
  EtatSequence etatSequence = EtatSequence::ETAT_INITIAL;
  setCouleurDel(DEL_ETEINT);

  while (true){

    switch (etatSequence){
      case (EtatSequence::ETAT_INITIAL):
        if (estBoutonAppuye()){
          etatSequence = EtatSequence::COMPTEUR;
        }
        break;

      case (EtatSequence::COMPTEUR):
        for (compteur = 0; compteur < MAX_COMPTEUR && estBoutonAppuye(); ++compteur){
          partirMinuterieMs(SECONDE_EN_MS / 10);
          do {} while (!gMinuterieExpiree);
        }
        etatSequence = EtatSequence::CLIGNOTE_VERT1;
        break;

      case (EtatSequence::CLIGNOTE_VERT1):
        setCouleurDel(DEL_VERT);
        partirMinuterieMs(SECONDE_EN_MS / 2);
        do {} while (!gMinuterieExpiree);
        setCouleurDel(DEL_ETEINT);
        etatSequence = EtatSequence::PAUSE;
        break;

      case (EtatSequence::PAUSE):
        partirMinuterieMs(SECONDE_EN_MS * 2);
        do {} while (!gMinuterieExpiree);
        etatSequence = EtatSequence::CLIGNOTE_ROUGE;
        break;

      case (EtatSequence::CLIGNOTE_ROUGE):
        compteur /= 2;
        for (uint8_t i = 0; i < compteur; ++i){
          setCouleurDel(DEL_ROUGE);
          partirMinuterieMs(SECONDE_EN_MS / 4);
          do {} while (!gMinuterieExpiree);
          setCouleurDel(DEL_ETEINT);
          partirMinuterieMs(SECONDE_EN_MS / 4);
          do {} while (!gMinuterieExpiree);
        }
        etatSequence = EtatSequence::CLIGNOTE_VERT2;
        break;

      case (EtatSequence::CLIGNOTE_VERT2):
        setCouleurDel(DEL_VERT);
        partirMinuterieMs(SECONDE_EN_MS);
        do {} while (!gMinuterieExpiree);
        setCouleurDel(DEL_ETEINT);
        etatSequence = EtatSequence::ETAT_INITIAL;
        break;
    }
  }

  return 0;
}