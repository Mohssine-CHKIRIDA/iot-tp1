question 3 :
#include <AFMotor.h>

    AF_DCMotor motor(4);

// Vitesses par mode
#define SPEED_OFF 0
#define SPEED_ECO 80
#define SPEED_NORMAL 170
#define SPEED_TURBO 255

String mode = "OFF";

void applyMode(String m)
{
    if (m == "OFF")
    {
        motor.run(RELEASE);
        motor.setSpeed(SPEED_OFF);
    }
    else
    {
        motor.run(FORWARD);
        if (m == "ECO")
            motor.setSpeed(SPEED_ECO);
        if (m == "NORMAL")
            motor.setSpeed(SPEED_NORMAL);
        if (m == "TURBO")
            motor.setSpeed(SPEED_TURBO);
    }
    Serial.print("Mode actuel : ");
    Serial.println(m);
}

void setup()
{
    Serial.begin(9600);
    motor.run(RELEASE);
    Serial.println("Modes: OFF / ECO / NORMAL / TURBO");
}

void loop()
{
    if (Serial.available() > 0)
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        cmd.toUpperCase();

        if (cmd == "OFF" || cmd == "ECO" || cmd == "NORMAL" || cmd == "TURBO")
        {
            mode = cmd;
            applyMode(mode);
        }
        else
        {
            Serial.println("Commande inconnue. Utilisez: OFF / ECO / NORMAL / TURBO");
        }
    }
}