# arduino-security-teller
Schoolproject Esmé

## Setup
1. set WiFi Access Hotspot aan op iPhone
2. noteer wachtwoord en naam van hotspot
3. vul deze gegevens in op teller/teller.ino (`STASSID "your-ssid"` en `STAPSK "your-password"`) en verbind je laptop ook met dat acces point
4. verbind board met laptop
5. open Arduino IDE software en laadt `teller` code
6. upload `teller.ino` naar board, open Serial Monitor en wacht tot het IP adres van de webserver getoond wordt
6. pas in `teller.js` op regel 1 het IP adres aan naar de waarde die je board toont in de Serial Monitor
7. start op je laptop "Go Live"
8. Ga naar je netwerk instellen op je laptop (`Systeeminstellingen -> Netwerk`), klik op `WiFi -> Details` en noteer het IP adres
9. Typ het IP adres op je telefoon met het poortnummer van je `Go Live` server (bv `'http://192.168.2.30:5000')

