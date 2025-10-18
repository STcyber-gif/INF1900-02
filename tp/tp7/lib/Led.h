/*
 * Classe permettant (...)
 *
 * Ecole Polytechnique de Montreal
 * Departement de genie informatique
 * Cours inf1900
 *
 * Frederic Gaudreault, Ethan Curnier, Yoan Blime et (...)
 * 2025
 *
 * Code qui n'est sous aucune license.
 */

#ifndef LED_H
#define LED_H

#include <avr/io.h>

/*
 * Classe Led:
 *
 */

class Led
{
public:

   Led();
   ~Led();

   void turnOn();
   void turnOff();

   bool isOn();

private:
   // Donnees membres
   uint8_t* ddr_
   uint8_t pin_;
};

#endif /* LED_H */