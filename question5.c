#include <AFMotor.h>

AF_DCMotor motor(4);
int vitesseActuelle = 0;

void rampTo(int targetSpeed, int stepDelay)
{
    if (targetSpeed > vitesseActuelle)
    {
        // Monter
        for (int i = vitesseActuelle; i <= targetSpeed; i++)
        {
            motor.setSpeed(i);
            delay(stepDelay);
        }
    }
    else
    {
        // Descendre
        for (int i = vitesseActuelle; i >= targetSpeed; i--)
        {
            motor.setSpeed(i);
            delay(stepDelay);
        }
    }
    vitesseActuelle = targetSpeed;
}

bool isNumericInput(String s)
{
    if (s.length() == 0)
        return false;
    for (int i = 0; i < s.length(); i++)
    {
        if (!isDigit(s[i]))
            return false;
    }
    return true;
}

void setup()
{
    Serial.begin(9600);
    motor.run(RELEASE);
    motor.setSpeed(0);
    Serial.println("=== Ventilateur avec securite ===");
    Serial.println("Entrez une vitesse (0-255):");
}

void loop()
{
    if (Serial.available() > 0)
    {
        String input = Serial.readStringUntil('\n');
        input.trim();

        // Securite 1 : input non numerique
        if (!isNumericInput(input))
        {
            Serial.println("[ERREUR] Entree non numerique ignoree.");
            Serial.println("Entrez une vitesse (0-255):");
            return;
        }

        int vitesse = input.toInt();

        // Securite 2 : valeur hors plage -> clamp automatique
        if (vitesse > 255)
        {
            Serial.println("[AVERTISSEMENT] Valeur > 255, forcee a 255.");
            vitesse = 255;
        }

        // Securite 3 : transition propre
        if (vitesse == 0)
        {
            Serial.println("[INFO] Arret progressif...");
            rampTo(0, 8);
            motor.run(RELEASE);
            Serial.println("[OK] Moteur arrete.");
        }
        else
        {
            motor.run(FORWARD);
            Serial.print("[INFO] Transition vers vitesse : ");
            Serial.println(vitesse);
            rampTo(vitesse, 8);
            Serial.print("[OK] Vitesse stabilisee : ");
            Serial.println(vitesseActuelle);
        }

        Serial.println("Entrez une vitesse (0-255):");
    }
}