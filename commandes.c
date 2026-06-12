question 4 :

    int led = 9;
bool etatLed = false;
unsigned long dernierChangement = 0;
int intervalleClignotement = 500; // 500 ms pour le clignotement
bool modeClignotement = false;

void setup()
{
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}

void loop()
{
    // Gestion du clignotement non bloquant
    if (modeClignotement)
    {
        if (millis() - dernierChangement >= intervalleClignotement)
        {
            etatLed = !etatLed;
            digitalWrite(led, etatLed);
            dernierChangement = millis();
        }
    }

    // Lecture des commandes série
    if (Serial.available() > 0)
    {
        String commande = Serial.readString(); // lit toute la ligne
        commande.trim();                       // enlève espaces, \n, \r
        commande.toLowerCase();                // insensible à la casse

        if (commande == "allumer")
        {
            modeClignotement = false;
            etatLed = true;
            digitalWrite(led, HIGH);
            Serial.println("LED allumée");
        }
        else if (commande == "eteindre")
        {
            modeClignotement = false;
            etatLed = false;
            digitalWrite(led, LOW);
            Serial.println("LED éteinte");
        }
        else if (commande == "inverser")
        {
            modeClignotement = false;
            etatLed = !etatLed;
            digitalWrite(led, etatLed);
            Serial.print("LED inversée : ");
            Serial.println(etatLed ? "allumée" : "éteinte");
        }
        else if (commande == "clignoter")
        {
            modeClignotement = true;
            // Force le premier changement immédiat
            dernierChangement = millis();
            Serial.println("Mode clignotement activé");
        }
        else
        {
            Serial.println("Commande inconnue. Utilisez : allumer, eteindre, inverser, clignoter");
        }
    }
}