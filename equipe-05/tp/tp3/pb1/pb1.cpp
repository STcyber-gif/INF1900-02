/*
* Nom: pb2
* Copyright
* License
* Description: Ce programme attenue progressivement la luminosité
* de la DEL du robot jusqu'a l'eteingnement apres 3 secondes.
* Cela est realise a l'aide d'un signal PWM qui alimente la DEL.
* La fréquence du pulse devra demeurer constante a 1 kHz.
* Cela sera effectue pour la DEL rouge, puis la DEL verte.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
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

const uint8_t PINS_DEL = (1 << DDA0) | (1 << DDA1);
const uint8_t NB_SECONDES = 3; // s
const uint16_t FREQUENCE = 1000; // Hz
const uint16_t PERIODE = 1000 / FREQUENCE * 1000; // us
const uint16_t DELAIS = 10; // us

void setDel(uint8_t etat) { PORTA = etat; }

void genererOndePwm(uint16_t periodeTotale, uint16_t periodeActive, uint8_t etatDel) {
  setDel(etatDel);
  for (uint16_t i = 0; i < periodeActive / DELAIS; ++i) {
    _delay_us(DELAIS);
  }

  setDel(DEL_ETEINT);
  for (uint16_t i = 0; i < (periodeTotale - periodeActive) / DELAIS; ++i) {
    _delay_us(DELAIS);
  }
}

int main()
{
  // ports en mode sortie (1)
  DDRA |= PINS_DEL;

  while (true) {
    for (uint16_t i = 0; i < FREQUENCE; ++i){
      for (uint8_t j = 0; j < NB_SECONDES; ++j){
        genererOndePwm(PERIODE, PERIODE - i, DEL_ROUGE);
      }
    }

    for (uint16_t i = 0; i < FREQUENCE; ++i){
      for (uint8_t j = 0; j < NB_SECONDES; ++j){
        genererOndePwm(PERIODE, PERIODE - i, DEL_VERT);
      }
    }
  }

  return 0;
}