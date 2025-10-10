/*
* Nom: pb2
* Copyright -
* License -
* Description: Ce programme envoie des caractère ascii vers la console
* de l'ordinateur via un cable USB reliant le PC et le robot.
* La communication sera en RS232 et se fera à l'aide d'un
* autre programme fourni sur le PC "serieViaUSB".
* La communication RS232 se fera à 2400 bauds, sans parité et
* les trames de 8 bits seront séparées par un bit d'arrêt.
* 
* Version 1 par Frederic Gaudreault et Ethan Curnier, Septembre 2025
*/

#include <avr/io.h>

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

  char mots[21] = "Le robot en INF1900\n";
  uint8_t i, j;
  for ( i = 0; i < 100; i++ ) {
    for ( j=0; j < 20; j++ ) {
     transmissionUART ( mots[j] );
    }
  }
  return 0;
}