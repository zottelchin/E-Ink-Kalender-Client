// Bibliotheken für das Display
#include <GxEPD.h>
#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

// FreeFonts von Adafruit_GFX, TODO: durch u9g2 Fonts erstetzen, damit UTF-8 unterstützt wird
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>

// Bibliotheken um auf das Internet zugreifen zu können
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>

// Display einrichten
GxIO_Class io(SPI, /*CS=*/ 15, /*DC=*/ 4, /*RST=*/ 5);
GxEPD_Class display(io, /*RST=*/ 5, /*Busy=*/ 16);

void setup() {
  Serial.begin(115200);
  display.init(115200);
  // Erst ab hier kann Serial verwendet werden! Warum? Keine Ahnung, ist einfach so.

  // Anzeige drehen, sodass der Display im Hochformat angesteuert werden kann.
  display.setRotation(3);
  Serial.println("Hallo \nDer Kalender wird jetzt gestartet.");
  Serial.println("Der Display ist kann jetzt angesteuert werden");

  //Start Bildschirm anzeigen
  einkStart();

  //Mit dem Wlan verbinden
  WiFi.mode(WIFI_STA);
  WiFi.begin("WLAN-NAME", "WLAN-KEY");
  Serial.print("Verbinde mich mit dem WLAN.");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: "); Serial.println(WiFi.localIP());

  // Wir sind ab hier mit dem WLAN verbunden

}

void loop() {
  // Anfrage an Server stellen
  HTTPClient http;    
  http.begin("http://192.168.2.23:6342");
  int httpCode = http.GET();            
  String payload = http.getString();    

  Serial.println(httpCode);   // HTTP Response Code ausgeben
  Serial.println(payload);    // HTTP Response Body ausgeben

  http.end();  // Verbindung schließen

  if (httpCode != 200){
    displayConnectionError();
  }else{
    displayData(payload);
  }
  delay(900000);  //GET Data at every 15 Min

}

void einkStart() {
  const GFXfont* f = &FreeSansBold18pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println("E-Ink Kalender");
  display.println();
  display.println(char(0x81));
  display.println();
  display.println();
  display.println("Starte...");
  display.update();
}

void displayData(String data) {
  Serial.println("Kalender anzeigen");
  const GFXfont* f1 = &FreeSansBold12pt7b;
  const GFXfont* f2 = &FreeSansBold9pt7b;
  const GFXfont* f3 = &FreeSans9pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextWrap(false);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f1);
  display.setCursor(25, 0);
  display.println();
  display.println("    " + getValue(data, 0));
  display.drawFastHLine(20, 32, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 110, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 180, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 250, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 320, 260, GxEPD_BLACK);
  display.setFont(f2);
  display.setCursor(0, 60);
  display.println(getValue(data, 1));
  display.setFont(f3);
  display.println(getValue(data, 2));
  display.println(getValue(data, 3));
  display.setFont(f2);
  display.setCursor(0, 130);
  display.println(getValue(data, 4));
  display.setFont(f3);
  display.println(getValue(data, 5));
  display.println(getValue(data, 6));
  display.setFont(f2);
  display.setCursor(0, 200);
  display.println(getValue(data, 7));
  display.setFont(f3);
  display.println(getValue(data, 8));
  display.println(getValue(data, 9));
  display.setFont(f2);
  display.setCursor(0, 270);
  display.println(getValue(data, 10));
  display.setFont(f3);
  display.println(getValue(data, 11));
  display.println(getValue(data, 12));
  display.setFont(f2);
  display.setCursor(0, 340);
  display.println(getValue(data, 13));
  display.setFont(f3);
  display.println(getValue(data, 14));
  display.println(getValue(data, 15));
  display.update();
}

void displayConnectionError(){
  const GFXfont* f = &FreeSansBold12pt7b;
  display.fillScreen(GxEPD_WHITE);
  display.setTextColor(GxEPD_BLACK);
  display.setFont(f);
  display.setCursor(0, 0);
  display.println();
  display.println("E-Ink Kalender");
  display.println();
  display.println();
  display.println();
  display.println("Verbindung zum Server ist fehlgeschlagen");
  display.println(":(");
  display.update();
}

/* Code from stackoverflow
  https://stackoverflow.com/a/14824108
*/

String getValue(String data, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == ';' || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
