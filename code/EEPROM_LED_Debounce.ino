#include <EEPROM.h>

// Declaration  de variables globales
const int buttonPin = 8; 
const int ledPin = 4;

int ledState;          // int car il est HAUT(1) ou BAS(0)
int buttonState;       
int lastButtonState = LOW; 

/*** 
  Les variables suivantes sont de type long car le temps est mesuré en millisecondes, qui
  deviendra rapidement un nombre plus important que ce qui peut être stocké dans une variable de type int.
 ***/

long lastDebounceTime = 0;
long debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, ledState);

  Serial.begin(9600);

  checkLedState();

}

void loop() {
  // Lire et attribuer l'état de commutation en une variable locale
  int reading = digitalRead(buttonPin);

  if (reading != lastButtonState)
  {
    lastDebounceTime = millis(); // Réinitialise la minuterie de débouchement
  }

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      // ne bascule que si le nouvel état du bouton est élevé
      if (buttonState == HIGH)
      {
        ledState = !ledState;
      }
    }
  }

  digitalWrite(ledPin, ledState);
  /*** 
    En utilisant eeprom.update () au lieu de eeprom.write () parce que, eeprom.update
 Écrit uniquement si la valeur a changé, réduisant les cycles d'écriture inutiles
 et prolonger la durée de vie de l'EEPROM (puisque les EEPROM ont un nombre limité
 des cycles de vie).

  ***/

  EEPROM.update(0, ledState); //Enregistrer l'état LED actuel dans l'EEPROM
  lastButtonState = reading;  
}

void checkLedState()
{
  Serial.println("Statut LED après redémarrage: ");
  ledState = EEPROM.read(0);
  if (ledState == 1)
  {
    Serial.println ("Allumée");
    digitalWrite(ledPin, HIGH);
  }
  if (ledState == 0)
  {
    Serial.println("Éteint");
    digitalWrite(ledPin, LOW);
  }
}
