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

#ifndef MOTOR_H
#define MOTOR_H

#include <avr/io.h>

/*
 * Classe Motors:
 *
 */

class Motors
{
public:

    Motors();
    ~Motors();

    setRightMotorForwards();
    setRightMotorBackwards();

    setLeftMotorForwards();
    setLeftMotorBackwards();

    setRightMotorSpeed(uint8_t speed);
    setLeftMotorSpeed(uint8_t speed);

private:
   // Donnees membres - aucun
}