/*
* Nom: pb2
* Copyright -
* License -
* Description: Lorsque le microcontrôleur demarre, il attend 10 secondes
* et fait clignoter la lumiere rouge pendant 1/10 de seconde. Quand la
* lumiere est eteinte, le joueur doit peser sur le bouton aussitot que
* possible. Si le joueur pese sur le bouton a l'interieur d'une seconde,
* la DEL devient verte. Sinon, la lumiere prend la couleur rouge.
* La DEL conserve sa couleur indefiniment. Il faut peser sur reset pour recommencer.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/


#define SECONDE_EN_MS 1000
#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Identification matérielle :
// connecter le port A 1 avec la borne positive de la DEL et
// connecter le Port A 2 avec la borne négative de la DEL.
const uint8_t DEL_ETEINT = 0x00;
const uint8_t DEL_ROUGE  = 0x01;
const uint8_t DEL_VERT   = 0x02;
const uint8_t PIN_BOUTON = (1 << DDD2);
const uint8_t PINS_DEL   = (1 << DDA0) | (1 << DDA1);
const uint8_t NB_SECONDES_ATTENTE = 10; //s
const uint16_t DUREE_CLIGNOTEMENT = 100; // ms
const uint16_t PRESCALER = 1024;

volatile bool gBoutonAppuye = false;     // vrai si appuye, faux sinon.
volatile bool gMinuterieExpiree = false; // vrai si expiree, faux sinon.


ISR (INT0_vect) {
  _delay_ms (30); // laisser un délai avant de confirmer la réponse du bouton-poussoir.
  gBoutonAppuye = PIND & PIN_BOUTON;
  EIFR |= (1 << INTF0); // Empecher le ISR d'etre appele une seconde fois a cause du rebond du bouton.
}

ISR (TIMER1_COMPA_vect) {
  gMinuterieExpiree = true;
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

  // il faut sensibiliser les interruptions au rising_edge (lorsque bouton est appuye)
  EICRA |= 1 << ISC00 | 1 << ISC01;

  sei (); // sei permet de recevoir à nouveau des interruptions.
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

void setCouleurDel(uint8_t couleur) { PORTA = couleur; }

int main()
{
  initialisation();

  setCouleurDel(DEL_ETEINT);
  for (uint8_t i = 0; i < NB_SECONDES_ATTENTE; ++i){
    partirMinuterieMs(SECONDE_EN_MS);
    do {
      // rien faire en attendant la muniterie
    } while (!gMinuterieExpiree);
  }

  setCouleurDel(DEL_ROUGE);
  partirMinuterieMs(DUREE_CLIGNOTEMENT);
  do {
    // rien faire en attendant la minuterie
  } while (!gMinuterieExpiree);
  setCouleurDel(DEL_ETEINT);

  partirMinuterieMs(SECONDE_EN_MS);
  do {
    // attendre qu'une des deux variables soit modifiée
    // par une ou l'autre des interruptions.
  } while (!gMinuterieExpiree && !gBoutonAppuye);

  // Une interruption s'est produite. Arrêter toute
  // forme d'interruption. Une seule réponse suffit.
  cli ();

  // Verifier la réponse
  setCouleurDel(gMinuterieExpiree ? DEL_ROUGE : DEL_VERT);

  return 0;
}