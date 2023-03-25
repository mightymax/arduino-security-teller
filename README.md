# arduino-security-teller
Schoolproject Esmé

## Setup
1. set WiFi Access Hotspot aan op iPhone
2. noteer wachtwoord en naam van hotspot
3. vul deze gegevens in op teller/secrets.h (`STASSID "your-ssid"` en `STAPSK "your-password"`) 
4. verbind board met laptop
5. open Arduino IDE software en laadt `teller` code
6. upload `teller.ino` naar board, open Serial Monitor en wacht tot het IP adres van de webserver getoond wordt
7.  Typ het IP adres in in je broswer (bv `'http://192.168.2.30')

