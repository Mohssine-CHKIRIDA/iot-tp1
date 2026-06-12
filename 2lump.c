int led1 = 10;
int led2 = 11;

bool etat1 = false;
bool etat2 = false;

unsigned long dernierChgt1 = 0;
unsigned long dernierChgt2 = 0;

unsigned long periode1 = 1000;
unsigned long periode2 = 1000;

bool enabled = true; // NEW: global on/off flag

void setup()
{
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
}

void loop()
{
  // Only update LEDs if system is enabled
  if (enabled)
  {
    // Non‑blocking blinking for LED1
    if (millis() - dernierChgt1 >= periode1)
    {
      etat1 = !etat1;
      digitalWrite(led1, etat1);
      dernierChgt1 = millis();
    }

    // Non‑blocking blinking for LED2
    if (millis() - dernierChgt2 >= periode2)
    {
      etat2 = !etat2;
      digitalWrite(led2, etat2);
      dernierChgt2 = millis();
    }
  }

  // Serial command handling
  if (Serial.available() > 0)
  {
    String commande = Serial.readString();
    commande.trim();
    commande.toLowerCase();

    if (commande == "off")
    {
      enabled = false;
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      Serial.println("LEDs OFF – blinking stopped");
    }
    else if (commande == "on")
    {
      enabled = true;
      // Reset timers to avoid an immediate unexpected toggle
      dernierChgt1 = millis();
      dernierChgt2 = millis();
      Serial.println("LEDs ON – blinking resumed");
    }
    else if (commande.startsWith("p1"))
    {
      int espace = commande.indexOf(' ');
      if (espace != -1)
      {
        String valStr = commande.substring(espace + 1);
        unsigned long nouvellePeriode = valStr.toInt();
        if (nouvellePeriode >= 50 && nouvellePeriode <= 10000)
        {
          periode1 = nouvellePeriode;
          Serial.print("Période LED1 = ");
          Serial.print(periode1);
          Serial.println(" ms");
        }
        else
        {
          Serial.println("Erreur : période hors limite (50-10000 ms)");
        }
      }
      else
      {
        Serial.println("Format : P1 <ms>");
      }
    }
    else if (commande.startsWith("p2"))
    {
      int espace = commande.indexOf(' ');
      if (espace != -1)
      {
        String valStr = commande.substring(espace + 1);
        unsigned long nouvellePeriode = valStr.toInt();
        if (nouvellePeriode >= 50 && nouvellePeriode <= 10000)
        {
          periode2 = nouvellePeriode;
          Serial.print("Période LED2 = ");
          Serial.print(periode2);
          Serial.println(" ms");
        }
        else
        {
          Serial.println("Erreur : période hors limite (50-10000 ms)");
        }
      }
      else
      {
        Serial.println("Format : P2 <ms>");
      }
    }
    else
    {
      Serial.println("Commandes disponibles : ON, OFF, P1 <ms>, P2 <ms>");
    }
  }
}