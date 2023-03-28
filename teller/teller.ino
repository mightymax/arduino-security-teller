// Code uit de arduino IDE

// De bestanden die ingeladen moeten kunnen worden op de arduino
#include <ESP8266WiFi.h>
#include <Ultrasonic.h>
#include "secrets.h"
#include "www/index.html.h"
#include "www/styles.ccs.h"
#include "www/teller.js.h"

// Voor het invoeren van de wifi gegevens om verbinding te maken
// Als je dit dus wel hier doet wordt het geupload binnen github en staan de gegevens online
const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);

// #define laat hier zien welke onderdelen van de 'schakeling' verbonden zijn aan welke punten het arduino bord
// Daarnaast wordt aangegeven wat het maximaal aantal wachtenden is en hoe breed de poort is
Ultrasonic sensorUitgaandkomend = Ultrasonic(D5);
Ultrasonic sensorUitgaand  = Ultrasonic(D8, D7);

#define MAX_WACHTENDEN 10
#define BREEDTE_POORT 20
#define LED_GROEN D2
#define LED_ROOD D1
#define LED_GEEL D3
#define RESET_KNOP D6

// Op het punt van beginnen is het aantal wachtenden 0
// int = integer = geheel getal
// deze staat buiten de loop omdat dit alleen van belang is als je net begint, als deze in de loop iedere keer naar 0 gaat hou je in feite niets bij
int wachtenden = 0;

// De algemene informatie voor de setup van het hele project
// Met Serial. begin(9600); wordt de communicatiesnelheid ingesteld op 9600 bits/seconde.
void setup() {
  Serial.begin(9600);

// met pinmode wordt aangegeven wat de pin moet doen, of het element dat aangesloten is input of output moet geven
// de lampjes zijn output want die moeten een signaal ontvangen en daar iets mee doen
// de knop is input want die moet een signaal sturen naar de arduino dat er iets moet veranderen
  pinMode(LED_GROEN, OUTPUT);
  pinMode(LED_ROOD, OUTPUT);
  pinMode(LED_GEEL, OUTPUT);
  pinMode(RESET_KNOP, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

// Er moet als eerste verbinding worden gemaakt met wifi, anders werkt de site niet waar je het aantal kunt bijhouden
  Serial.println();
  Serial.println();
  Serial.print("Wacht op WiFi ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

// Als de wifi verbonden is wordt er een vertraging ingesteld van 500ms vorodat het nieuwe signaal wordt geladen
// Doe je dit niet en zet je dit heel laag? Dan moet de arduino continu opnieuw informatie ontvangen en sturen en dit kan de arduino niet aan
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Als de arduino verbonden is met wifi dan kan de webserver gestart worden
  Serial.println("");
  Serial.println("WiFi verbonden");
  // start de webserver:
  // Om de wachtrij te zien moet je verbinding maken met het lokale IP adres
  server.begin();
  Serial.print("Om de wachtrij te zien ga naar http://");
  Serial.println(WiFi.localIP());

}

// Het antwoord van de webserver
void httpResponse(WiFiClient client, String contentType, String content) {
    client.print("HTTP/1.1 200 OK\r\nContent-Type: text/");
// content type geeft aan wat voor bestand er wordt ingeladen - bijvoorbeeld html pagina
    client.print(contentType);
// \r houdt in: nieuwe regel
    client.print("\r\n\r\n");
    client.print(content);
}

//de 'client' moet de informatie van de server accepteren
void loop() {
  WiFiClient client = server.accept();
  if (client) {
    String req = client.readStringUntil('\r');
    client.setTimeout(5000);
     while (client.available()) {
       client.read();
    }
    // HTTP response:
    if (req.indexOf("/styles.css") != -1) {
      httpResponse(client, "css", CSS);
    } else if (req.indexOf("/teller.js") != -1) {
      httpResponse(client, "javascript", JS);
    } else if (req.indexOf("/wachtenden") != -1) {
      httpResponse(client, "plain", String(wachtenden));
    } else {
      httpResponse(client, "html", INDEX);
    }

    // bovenstaande houdt in: als de index styles.css bevat: open CSS
    // als die het niet is, open dan JavaScript etc.
    // als het allemaal niet is, open dan HTML.
  }

// als het aantal wachtenden het maximaal bereikt, wat eerder ingesteld is op 10, dan gaat het gele lampje branden
// led_geel, low houdt in dat het lampje brandt (wel ingedrukt dus maakt contact met arduino)
// als het aantal wachten niet op maximaal zit, brand het lampje niet dus led_geel, high (niet ingedrukt)
  if (wachtenden >= MAX_WACHTENDEN) {
    digitalWrite(LED_GEEL, LOW);
  } else {
    digitalWrite(LED_GEEL, HIGH);
  }
// rood en groen zijn niet ingedrukt
  digitalWrite(LED_ROOD, HIGH);
  digitalWrite(LED_GROEN, HIGH);

// als de reset knop wordt ingedrukt gaat het aantal wachtenden terug naar 0
  if (digitalRead(RESET_KNOP) == HIGH) {
    wachtenden = 0 ;
    delay(500) ;
  }

// als de sensor een afstand ziet die kleiner is dan de ingestelde afstand moet het rode lampje gaan branden - er komt iemand binnen
// een vertraging van 200ms want de persoon moet tijd krijgen om langs het poortje te lopen
  if (sensorUitgaand.read() < BREEDTE_POORT) {
    digitalWrite(LED_ROOD, LOW);
    // allow time to pass:
    delay(200);

// als de sensor een afstand meet die groter is dan de ingestelde afstand is er iemand langsgelopen en gaat het rode lampje vranden
    if (sensorUitgaand.read() > BREEDTE_POORT) {
      // passed
      digitalWrite(LED_ROOD, HIGH);
      wachtenden++;
    }
  }

// hetzelfde gelft voor de andere sensor en het groene lampje - er gaat iemand weg
  if (sensorUitgaandkomend.read() < BREEDTE_POORT) {
    digitalWrite(LED_GROEN, LOW);
    // allow time to pass:
    delay(200);
    if (sensorUitgaandkomend.read() > BREEDTE_POORT) {
      // passed
      digitalWrite(LED_GROEN, HIGH);
      wachtenden--;
    }
  }

// Het aantal wachtenden kan niet lager dan 0 dus ik heb ingesteld dat het aantal wachtenden onder 0 inhoudt dat er 0 wachtenden zijn 
  if (wachtenden < 0) wachtenden = 0;
}
