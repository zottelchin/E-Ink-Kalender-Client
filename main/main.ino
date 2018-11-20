// Bibliotheken für das Display
#include <GxEPD.h>
#include <GxGDEW042T2/GxGDEW042T2.h>      // 4.2" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

// Bibliotheken um auf das Internet zugreifen zu können
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// Weil UTF-8
#include <U8g2_for_Adafruit_GFX.h>


// Display einrichten
GxIO_Class io(SPI, /*CS=*/ 15, /*DC=*/ 4, /*RST=*/ 5);
GxEPD_Class display(io, /*RST=*/ 5, /*Busy=*/ 16);
U8G2_FOR_ADAFRUIT_GFX utf8_display;

const char*  ssid= "FRITZ!Box 7362 SL";
const char* password = "CocaCola";

void setup() {
  Serial.begin(115200);
  display.init(115200);
  utf8_display.begin(display);
  // Erst ab hier kann Serial verwendet werden! Warum? Keine Ahnung, ist einfach so.

  // Anzeige drehen, sodass der Display im Hochformat angesteuert werden kann.
  display.setRotation(3);
  utf8_display.setFontMode(1);
  Serial.println(F("Hallo \nDer Kalender wird jetzt gestartet."));
  Serial.println(F("Der Display ist kann jetzt angesteuert werden"));

  //Start Bildschirm anzeigen
  einkStart();

  //Mit dem Wlan verbinden
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print(F("Verbinde mich mit dem WLAN."));
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.print(F("Connected, IP address: ")); Serial.println(WiFi.localIP());

  // Wir sind ab hier mit dem WLAN verbunden

  WiFiClientSecure client;
  Serial.println(F("connecting to e-ink.zottelchin.de"));
  if (!client.connect(F("e-ink.zottelchin.de"), 443)) {
    Serial.println(F("connection failed"));
    return;
  }



  client.print(F("GET /ErcHRbrXh6aE7KCOfbuFzfvP6lxyoA HTTP/1.1\r\nHost: e-ink.zottelchin.de\r\nUser-Agent: BuildFailureDetectorESP8266\r\nConnection: close\r\n\r\n"));

  //Serial.println("request sent");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println(F("headers received"));
      break;
    }
  }
  String line = client.readStringUntil('\n');

  displayData(line);
}

void loop() {


}

void einkStart() {
  display.fillScreen(GxEPD_WHITE);
  // Schriftarten verbrauchen richtig Speicher (dynamiscH)
  utf8_display.setFont(u8g2_font_helvB18_tf);
  // SetFontMode muss nach jedem ändern der Schriftart aufgeruden werden, sonst wird Fontmode 0 verwende
  utf8_display.setFontMode(1);
  utf8_display.setForegroundColor(GxEPD_BLACK);
  utf8_display.setCursor(30, 30);
  utf8_display.println(F("E-Ink Kalender"));
  utf8_display.setCursor(20, 350);
  utf8_display.println(F("Starte..."));
  utf8_display.setFont(u8g2_font_helvR14_tf);
  utf8_display.setFontMode(1);
  utf8_display.setCursor(160, 390);
  utf8_display.print(F("von @zottelchin"));
  display.fillRect(70, 100, 150, 150, GxEPD_BLACK);
  display.fillCircle(105, 105, 7, GxEPD_WHITE);
  display.fillCircle(185, 105, 7, GxEPD_WHITE);
  display.fillRect(80, 120, 130, 120, GxEPD_WHITE);
  display.drawChar(90, 130, 'J', GxEPD_BLACK, GxEPD_WHITE, 5);
  display.drawChar(120, 130, 'U', GxEPD_BLACK, GxEPD_WHITE, 5);
  display.drawChar(150, 130, 'L', GxEPD_BLACK, GxEPD_WHITE, 5);
  display.drawChar(175, 130, 'I', GxEPD_BLACK, GxEPD_WHITE, 5);
  display.drawChar(105, 175, '2', GxEPD_BLACK, GxEPD_WHITE, 7);
  display.drawChar(155, 175, '4', GxEPD_BLACK, GxEPD_WHITE, 7);
  display.update();
}

void displayData(String data) {
  Serial.println(F("Kalender anzeigen"));
  display.fillScreen(GxEPD_WHITE);
  display.setTextWrap(false);
  display.setTextColor(GxEPD_BLACK);
  utf8_display.setFont(u8g2_font_helvB18_tf);
  utf8_display.setFontMode(1);
  utf8_display.setCursor(25, 0);
  utf8_display.println();
  utf8_display.println("    " + getValue(data, 0));
  display.drawFastHLine(20, 32, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 110, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 180, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 250, 260, GxEPD_BLACK);
  display.drawFastHLine(20, 320, 260, GxEPD_BLACK);
  utf8_display.setFont(u8g2_font_helvB14_tf);
  utf8_display.setFontMode(1);
  utf8_display.setCursor(0, 60);
  utf8_display.println(getValue(data, 1));
  utf8_display.setFont(u8g2_font_helvR14_tf);
  utf8_display.setFontMode(1);
  utf8_display.println(getValue(data, 2));
  utf8_display.println(getValue(data, 3));
  utf8_display.setFont(u8g2_font_helvB14_tf);
  utf8_display.setFontMode(1);
  utf8_display.setCursor(0, 130);
  utf8_display.println(getValue(data, 4));
  utf8_display.setFont(u8g2_font_helvR14_tf);
  utf8_display.setFontMode(1);
  utf8_display.println(getValue(data, 5));
  utf8_display.println(getValue(data, 6));
  utf8_display.setFont(u8g2_font_helvB14_tf);
  utf8_display.setFontMode(1);
  utf8_display.setCursor(0, 200);
  utf8_display.println(getValue(data, 7));
  utf8_display.setFont(u8g2_font_helvR14_tf);
  utf8_display.setFontMode(1);
  utf8_display.println(getValue(data, 8));
  utf8_display.println(getValue(data, 9));
  utf8_display.setFont(u8g2_font_helvB14_tf);
  utf8_display.setFontMode(1);
  utf8_display.setCursor(0, 270);
  utf8_display.println(getValue(data, 10));
  utf8_display.setFont(u8g2_font_helvR14_tf);
  utf8_display.setFontMode(1);
  utf8_display.println(getValue(data, 11));
  utf8_display.println(getValue(data, 12));
  utf8_display.setFont(u8g2_font_helvB14_tf);
  utf8_display.setFontMode(1);
  utf8_display.setCursor(0, 340);
  utf8_display.println(getValue(data, 13));
  utf8_display.setFont(u8g2_font_helvR14_tf);
  utf8_display.setFontMode(1);
  utf8_display.println(getValue(data, 14));
  utf8_display.println(getValue(data, 15));
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
