question 6

#include <Servo.h>
#include <Stepper.h>

#define STEPS_PER_REV 2048

    Servo myServo;
Stepper myStepper(STEPS_PER_REV, 8, 10, 9, 11);

int stepperSpeed = 10; // vitesse par défaut (RPM)

bool isNumeric(String s)
{
    s.trim();
    if (s.length() == 0)
        return false;
    for (int i = 0; i < s.length(); i++)
    {
        if (!isDigit(s[i]))
            return false;
    }
    return true;
}

// ---- SERVO ----
// Positions prédéfinies (exigence : au moins 2)
void goToPreset(String name)
{
    name.toUpperCase();
    if (name == "OPEN")
    {
        myServo.write(0);
        Serial.println("[SERVO] Position preset -> OPEN (0 deg)");
    }
    else if (name == "MIDDLE")
    {
        myServo.write(90);
        Serial.println("[SERVO] Position preset -> MIDDLE (90 deg)");
    }
    else if (name == "CLOSED")
    {
        myServo.write(180);
        Serial.println("[SERVO] Position preset -> CLOSED (180 deg)");
    }
    else
    {
        Serial.println("[SERVO][ERREUR] Preset inconnu. Utiliser: OPEN / MIDDLE / CLOSED");
    }
}

void handleServo(String param)
{
    param.trim();

    // Si c'est un preset textuel
    if (!isNumeric(param))
    {
        goToPreset(param);
        return;
    }

    // Sinon c'est un angle numérique
    int angle = constrain(param.toInt(), 0, 180);
    myServo.write(angle);
    Serial.print("[SERVO] Position -> ");
    Serial.print(angle);
    Serial.println(" deg");
}

// ---- STEPPER ----
void handleStepper(String dirStr, String stepsStr)
{
    dirStr.trim();
    stepsStr.trim();
    dirStr.toUpperCase();

    if (!isNumeric(stepsStr))
    {
        Serial.println("[STEPPER][ERREUR] Steps invalide.");
        return;
    }

    int steps = stepsStr.toInt();
    myStepper.setSpeed(stepperSpeed); // appliquer la vitesse courante

    if (dirStr == "FORWARD")
    {
        Serial.print("[STEPPER] FORWARD -> ");
        Serial.print(steps);
        Serial.print(" steps @ ");
        Serial.print(stepperSpeed);
        Serial.println(" RPM");
        myStepper.step(steps);
    }
    else if (dirStr == "BACKWARD")
    {
        Serial.print("[STEPPER] BACKWARD -> ");
        Serial.print(steps);
        Serial.print(" steps @ ");
        Serial.print(stepperSpeed);
        Serial.println(" RPM");
        myStepper.step(-steps);
    }
    else
    {
        Serial.println("[STEPPER][ERREUR] Direction invalide. FORWARD ou BACKWARD.");
    }
}

// ---- VITESSE STEPPER ----
// Commande : V:<rpm>   ex: V:5  ou  V:15
void handleSpeed(String rpmStr)
{
    rpmStr.trim();
    if (!isNumeric(rpmStr))
    {
        Serial.println("[SPEED][ERREUR] Valeur invalide. Ex: V:10");
        return;
    }
    int rpm = rpmStr.toInt();
    rpm = constrain(rpm, 1, 15); // max 15 RPM pour 28BYJ-48
    stepperSpeed = rpm;
    myStepper.setSpeed(stepperSpeed);
    Serial.print("[SPEED] Vitesse stepper -> ");
    Serial.print(stepperSpeed);
    Serial.println(" RPM");
}

void printHelp()
{
    Serial.println("================================");
    Serial.println("   Interface serie unifiee      ");
    Serial.println("================================");
    Serial.println("-- SERVO --");
    Serial.println("  S:<angle>    ex: S:90     (0 a 180 deg)");
    Serial.println("  S:OPEN       -> 0 deg");
    Serial.println("  S:MIDDLE     -> 90 deg");
    Serial.println("  S:CLOSED     -> 180 deg");
    Serial.println("-- STEPPER --");
    Serial.println("  P:<DIR>:<steps>  ex: P:FORWARD:1024");
    Serial.println("  V:<rpm>          ex: V:10  (1 a 15 RPM)");
    Serial.println("-- AUTRE --");
    Serial.println("  HELP");
    Serial.println("================================");
}

void setup()
{
    Serial.begin(9600);
    myServo.attach(6);
    myStepper.setSpeed(stepperSpeed);
    myServo.write(90); // position initiale MIDDLE
    Serial.println("Initialisation OK. Servo: 90 deg. Stepper: 10 RPM.");
    printHelp();
}

void loop()
{
    if (Serial.available() > 0)
    {
        String cmd = Serial.readStringUntil('\n');
        cmd.trim();
        if (cmd.length() == 0)
            return;

        char prefix = cmd.charAt(0);

        // Servo : S:90  ou  S:OPEN
        if (prefix == 'S' || prefix == 's')
        {
            int sep = cmd.indexOf(':');
            if (sep == -1)
            {
                Serial.println("[ERREUR] Format: S:<angle> ou S:OPEN");
                return;
            }
            handleServo(cmd.substring(sep + 1));

            // Stepper : P:FORWARD:1024
        }
        else if (prefix == 'P' || prefix == 'p')
        {
            int sep1 = cmd.indexOf(':');
            int sep2 = cmd.indexOf(':', sep1 + 1);
            if (sep1 == -1 || sep2 == -1)
            {
                Serial.println("[ERREUR] Format: P:<DIR>:<steps>  ex: P:FORWARD:1024");
                return;
            }
            handleStepper(cmd.substring(sep1 + 1, sep2), cmd.substring(sep2 + 1));

            // Vitesse stepper : V:10
        }
        else if (prefix == 'V' || prefix == 'v')
        {
            int sep = cmd.indexOf(':');
            if (sep == -1)
            {
                Serial.println("[ERREUR] Format: V:<rpm>  ex: V:10");
                return;
            }
            handleSpeed(cmd.substring(sep + 1));

            // Help
        }
        else
        {
            String upper = cmd;
            upper.toUpperCase();
            if (upper == "HELP")
            {
                printHelp();
            }
            else
            {
                Serial.println("[ERREUR] Commande inconnue. Tapez HELP.");
            }
        }
    }
}