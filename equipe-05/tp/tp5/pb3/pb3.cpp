/*
* Nom: pb1
* Copyright -
* License -
* Description: Ce programme écrit une chaine de caracteres termine par 0xFF
* dans la memoire du robot, puis relit la chaine caractere par caractere jusqu'a
* la rencontre du 0xFF et l'envoi a l'ordinateur via communication RS232.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/

#include "memoire_24.h"
#include <util/delay.h>
#include <avr/io.h>

const uint8_t CHAR_FIN_LECTURE = 0xFF;
const uint8_t DELAIS_POST_ECRITURE = 5; // ms
const uint16_t ADRESSE_MSG = 0x0000;
const char MSG[] = "J'ecrit absolument n'importe quoi la-la-li la-lali-lale-re\n";

void initialisationUART ( void ) {
  // 2400 bauds. Nous vous donnons la valeur des deux
  // premiers registres pour vous éviter des complications.
  UBRR0H = 0;
  UBRR0L = 0xCF;
  // permettre la réception et la transmission par le UART0
  UCSR0A = 0;
  UCSR0B = 1 << RXEN0 | 1 << TXEN0;
  // Format des trames: 8 bits, 1 stop bits, sans parité
  UCSR0C = 1 << UCSZ00 | 1 << UCSZ01;
}

// Du USART vers le PC -> p.174 de la doc du atmega
void transmissionUART ( uint8_t donnee ) {
  // Attendre que le registre de transmission soit vide.
  while ( !( UCSR0A & (1<<UDRE0)) );

  // Mettre la donnee dans le registre, elle est ensuite transmise.
  UDR0 = donnee;
}

int main()
{
  initialisationUART();

  Memoire24CXXX mem;
  mem.ecriture(ADRESSE_MSG, (uint8_t*)MSG, sizeof(MSG));
  _delay_ms(DELAIS_POST_ECRITURE);

  mem.ecriture(ADRESSE_MSG + sizeof(MSG), CHAR_FIN_LECTURE);
  _delay_ms(DELAIS_POST_ECRITURE);

  uint8_t charLecture;
  mem.lecture(ADRESSE_MSG, &charLecture);

  for (uint8_t i = 1; charLecture != CHAR_FIN_LECTURE; ++i){
    transmissionUART(charLecture);
    mem.lecture(ADRESSE_MSG + i, &charLecture);
  }

  return 0;
}