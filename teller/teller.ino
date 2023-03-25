#include <ESP8266WiFi.h>
#include <Ultrasonic.h>
#include "secrets.h"
#include "www/index.html.h"
#include "www/styles.ccs.h"
#include "www/teller.js.h"

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);

Ultrasonic sensorUitgaandkomend = Ultrasonic(D5);
Ultrasonic sensorUitgaand  = Ultrasonic(D8, D7);

#define MAX_WACHTENDEN 10
#define LED_GROEN D2
#define LED_ROOD D1
#define LED_GEEL D3
#define RESET_KNOP D6

int wachtenden = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_GROEN, OUTPUT);
  pinMode(LED_ROOD, OUTPUT);
  pinMode(LED_GEEL, OUTPUT);
  pinMode(RESET_KNOP, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 0);

  Serial.println();
  Serial.println();
  Serial.print("Wacht op WiFi ");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("");
  Serial.println("WiFi verbonden");
  // start de webserver:
  server.begin();
  Serial.print("Om de wachtrij te zien ga naar http://");
  Serial.println(WiFi.localIP());

}

void httpResponse(WiFiClient client, String contentType, String content) {
    client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/"));
    client.print(contentType);
    client.print("\r\n\r\n");
    client.print(content);
}

void loop() {
  WiFiClient client = server.accept();
  if (client) {
    String req = client.readStringUntil('\r');
    client.setTimeout(5000);
     while (client.available()) {
       client.read();
    }
    // HTTP response:
    if (req.indexOf(F("/styles.css")) != -1) {
      httpResponse(client, "css", CSS);
    } else if (req.indexOf(F("/teller.js")) != -1) {
      httpResponse(client, "javascript", JS);
    } else if (req.indexOf(F("/wachtenden")) != -1) {
      httpResponse(client, "plain", String(wachtenden));
    } else {
      httpResponse(client, "html", INDEX);
    }
  }

  if (wachtenden >= MAX_WACHTENDEN) {
    digitalWrite(LED_GEEL, LOW);
  } else {
    digitalWrite(LED_GEEL, HIGH);
  }

  digitalWrite(LED_ROOD, HIGH);
  digitalWrite(LED_GROEN, HIGH);

  if (digitalRead(RESET_KNOP) == HIGH) {
    wachtenden = 0 ;
    delay(500) ;
  }

  if (sensorUitgaand.read() < 20) {
    digitalWrite(LED_ROOD, LOW);
    // allow time to pass:
    delay(200);
    if (sensorUitgaand.read() > 100) {
      // passed
      digitalWrite(LED_ROOD, HIGH);
      wachtenden++;
    }
  }

  if (sensorUitgaandkomend.read() < 20) {
    digitalWrite(LED_GROEN, LOW);
    // allow time to pass:
    delay(200);
    if (sensorUitgaandkomend.read() > 100) {
      // passed
      digitalWrite(LED_GROEN, HIGH);
      wachtenden--;
    }
  }

  if (wachtenden < 0) wachtenden = 0;
}
