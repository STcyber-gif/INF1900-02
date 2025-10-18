/*
 * Nom: tp1
 * Copyright
 * License
 * Description: Ce programme implémente un système logicie anti-rebond
 * pour détecter correctement la pression d'un bouton et change la
 * couleur de la DEL bicolore du robot de façon cyclique du vert,
 * rouge et ambre à chaque pression du bouton.
 * 
 * Version 1.2 par Frédéric Gaudreault et Ethan Curnier, Septembre 2025
 */

#define F_CPU 8000000UL  // 8 MHz
#include <util/delay.h>

#include <avr/io.h>

bool boutonAppuye() {
  if (PIND & 0x04) {
    _delay_ms(10);
    return (PIND & 0x04);
  }
  return false;
}

int main()
{
  // ports en mode sortie
  DDRA = 0xFF;
  DDRB = 0xFF;
  DDRC = 0xFF;

  // ports en mode entree
  DDRD = 0x00;
  
  uint8_t index = 0;
  bool estAppuye = false;
  bool etaitAppuye = false;

  while(true){
    estAppuye = boutonAppuye();

    if (estAppuye && !etaitAppuye)
      index = (index + 1) % 3;

    if (index == 0) 
      PORTA = 0x01;
    else if (index == 1)
      PORTA = 0x02;
    else {
      if (PORTA & 0x01)
        PORTA = 0x02;
      else
        PORTA = 0x01;
    }

    etaitAppuye = estAppuye;
  }

  return 0;
}