#include <Arduino.h>
#include "MHZ19.h"
#include <SoftwareSerial.h>
#include <Adafruit_SSD1306.h>

#define RX_PIN 5      //MH-Z19 RX-PIN
#define TX_PIN 4      //MH-Z19 TX-PIN
#define BAUDRATE 9600 //Terminal Baudrate
#define REDPIN 13     //Pin für Rote LED
#define GREENPIN 12   //Pin für Grüne LED
#define SCHWELLE 1500 //Schwellwert wenn die LED von Grün auf Rot wechseln soll.

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)

//Klassen Initialisierung
MHZ19 myMHZ19;
SoftwareSerial mySerial(RX_PIN, TX_PIN);
unsigned long getDataTimer = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup()
{
  // Setup OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  //LED PIN Mode festlegen
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  //Serielle Intialisierung
  Serial.begin(BAUDRATE);
  mySerial.begin(BAUDRATE);
  myMHZ19.begin(mySerial);
}
void loop()
{
  //Prüfung ob 2 Sekunden um sind.
  if (millis() - getDataTimer >= 2000)
  {
    int CO2;                //Variable darf nur Zahlen enthalten
    CO2 = myMHZ19.getCO2(); //Variable schreiben
                            //Ermittel ob der Schwellwert überschritten wurde und Schalten der LED
    if (CO2 > SCHWELLE)
    {
      digitalWrite(GREENPIN, LOW);
      digitalWrite(REDPIN, HIGH);
      delay(100);
    }
    else
    {
      digitalWrite(REDPIN, LOW);
      digitalWrite(GREENPIN, HIGH);
      delay(100);
    }
    //Ausgabe auf den Seriellen Monitor
    Serial.print("CO2 (ppm): ");
    Serial.println(CO2);
    getDataTimer = millis();
  }
}