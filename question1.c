#include <AFMotor.h>

AF_DCMotor motor(4); // Moteur sur le port M4

void setup()
{
    // Vitesse initiale
    motor.setSpeed(200);
    motor.run(RELEASE);
}

void loop()
{
    uint8_t i;

    // Allumer en marche avant
    motor.run(FORWARD);

    // Accélérer de 0 à 255
    for (i = 0; i < 255; i++)
    {
        motor.setSpeed(i);
        delay(10);
    }

    // Décélérer de 255 à 0
    for (i = 255; i != 0; i--)
    {
        motor.setSpeed(i);
        delay(10);
    }

    // Changer de direction
    motor.run(BACKWARD);

    // Accélérer de 0 à 255
    for (i = 0; i < 255; i++)
    {
        motor.setSpeed(i);
        delay(10);
    }

    // Décélérer de 255 à 0
    for (i = 255; i != 0; i--)
    {
        motor.setSpeed(i);
        delay(10);
    }

    // Éteindre
    motor.run(RELEASE);
    delay(1000);
}