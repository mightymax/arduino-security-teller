#include <ESP8266WiFi.h>
#include <Ultrasonic.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

WiFiServer server(80);

Ultrasonic sensorUitgaandkomend = Ultrasonic(D5);
Ultrasonic sensorUitgaand  = Ultrasonic(D8, D7);

#define MAX_WACHTENDEN 10
#define LED_GROEN D2
#define LED_ROOD D1
#define LED_ORANJE D3
#define RESET_KNOP D6

int wachtenden = 0;

void setup() {
  Serial.begin(9600);

  pinMode(LED_GROEN, OUTPUT);
  pinMode(LED_ROOD, OUTPUT);
  pinMode(LED_ORANJE, OUTPUT);
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
  // start our webserver:
  server.begin();
  Serial.print("Webserver gestart op IP adres ");
  Serial.println(WiFi.localIP());

}


void loop() {
  WiFiClient client = server.accept();
  if (client) {
    client.setTimeout(5000);
     while (client.available()) {
       client.read();
    }
    client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nAccess-Control-Allow-Origin: *\r\n\r\n"));
    client.print(wachtenden) ;
  }

  if (wachtenden >= MAX_WACHTENDEN) {
    digitalWrite(LED_ORANJE, LOW);
  } else {
    digitalWrite(LED_ORANJE, HIGH);
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
