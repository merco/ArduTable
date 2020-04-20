#include <NTPClient.h>

//http://davidemercanti.altervista.org/nownow.php
//04-07-2019 22:13:46

#include <NTPClient.h>
#include <time.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <DNSServer.h>

#include <ESP8266HTTPClient.h>
#include "FS.h"
#include <SPIFFSIniFile.h>
#include <Arduino.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRac.h>
#include <IRtext.h>
#include <AsyncDelay.h>

int CURRENT_MODE = 1;
int RECV_PIN = D4;
IRrecv irrecv(RECV_PIN);
decode_results results;




#include <WebSocketsServer.h>
#include <Hash.h>
WebSocketsServer webSocket = WebSocketsServer(81);
IPAddress apIP(192, 168, 4, 1);
String   urlStringMeteo = "http://davidemercanti.altervista.org/meteo/wemosmeteo.php?c=";
String   urlCittaString = "2890";

IPAddress ip(192, 168, 1, 100);
IPAddress ipGW(192, 168, 1, 1);
IPAddress ipMSK(255, 255, 255, 0);
IPAddress ipDNS(8, 8, 8, 8);
String ssid = "d_mercanti116";
String password = "0192837465";
MDNSResponder mdns;
const char *filenameINI = "/set.ini";
SPIFFSIniFile ini(filenameINI);
ESP8266WebServer server(80);
const size_t INIbufferLen = 80;
char bufferINI[INIbufferLen];

int keyRepeat = 0;

bool buttonClicked = false;
bool buttonLONGClicked = false;
//WiFiUDP ntpUDP;
// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionaly you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
//NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

const byte PROGMEM ledsAdress[12][12] = {
  {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
  {23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12},
  {24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35},
  {47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36},
  {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59},
  {71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60},
  {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83},
  {95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84},
  {96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107},
  {119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108},
  {120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131},
  {143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132}
};  //Adress of each led in the matrix (Progmem means stored in FLASH instead //of SRAM)
/* *** LED color  *** */
#define  GREEN  0x00FF00
#define  RED    0xFF0000
#define  BLUE   0x0000FF
#define  YELLOW 0xFFFF00
#define  CHOCOLATE  0xD2691E
#define  PURPLE 0xFF00FF
#define  WHITE  0XFFFFFF
#define  AQUA   0x00FFFF
#define  HOTPINK 0xFF1493
#define  DARKORANGE 0xFF8C00
unsigned int long colorLib[10] = {RED, GREEN, BLUE, YELLOW, CHOCOLATE, PURPLE, WHITE, AQUA, HOTPINK, DARKORANGE};
byte colorLib_MAXCOL = 10;
#include <FastLED.h>
#define NUM_LEDS 144
#define ROW_COUNT 12
#define COL_COUNT 12
#define DATA_PIN D3
CRGB leds[NUM_LEDS];

byte iiCol = 0;


#include "font3x5.h"
uint8_t charBuffer[8][8];

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long TABLE_previousMillis = 0;
unsigned long TABLE_currentMillis = 0;
long TABLE_INTERVAL = 125;  //in ms
const long INTERVAL = 125;  //in ms per il keyup

DNSServer dnsServer;
void handleRoot() {

  server.send(200, "text/plain", "hello from esp8266table.local !");

}
String ipToStr (  IPAddress IP) {
  if (IP[0] > 0)
    return  String(IP[0]) + '.' + String(IP[1]) + '.' + String(IP[2]) + '.' + String(IP[3]);
  else return "";
}
void handleRootSetup() {
  String content = "";

  content = "<!DOCTYPE HTML>\r\n<html><h1>SETUP</h1>";
  content += "<HR>";

  // content += ipStr;
  content += "<p>";
  content += "</p>";
  content += "<form method='get' action='setting'>";
  //content += "<label>SSID: </label><input name='ssid' length=32 value='"  + ssid + "'>";
  content += "<BR>";
  //content += "<label>PWD: </label><input name='pass' length=64 value='"  + password + "'>";
  content += "<BR>";
  content += "<BR>";
  content += "<label>Configurazione con IP Statico</label>";
  content += "<BR>";
  content += "<label>IP  : </label><input name='ip' length=15 value='" + ipToStr(ip) + "'>";
  content += "<BR>";
  content += "<label>GW  : </label><input name='gw' length=15 value='" + ipToStr(ipGW) + "'>";
  content += "<BR>";
  content += "<label>Mask: </label><input name='mk' length=15 value='" + ipToStr(ipMSK) + "'>";
  content += "<BR>";
  content += "<label>DNS: </label><input name='dns' length=15 value='" + ipToStr(ipDNS) + "'>";
  content += "<BR>";
  content += "<BR>";
  //urlCittaString
  content += "<label>Codice citta ilmeteo.it (vedi su https://www.ilmeteo.it/portale/servizi-web) </label>";
  content += "<BR>";
  content += "<label>Codice: </label><input name='codicecitta' length=15 value='" + urlCittaString + "'>";

  content += "<BR>";
  content += "<input type='submit'></form>";

  content += "</html>";
  server.send(200, "text/html", content);

}
void handleSettingSetup() {
  String content;
  int statusCode;
  String qsid = server.arg("ssid");
  String qpass = server.arg("pass");
  String ip = server.arg("ip");
  String gw = server.arg("gw");
  String mk = server.arg("mk");
  String dns = server.arg("dns");
  String c = server.arg("codicecitta");
  String strlum = server.arg("lumnotte");
  String strUrlT = server.arg("urlt");

  Serial.println("handleSettingSetup");
  Serial.println(qsid);
  Serial.println(qpass);


  content = "{\"Error\":\"404 not found\"}";
  statusCode = 404;
  server.send(statusCode, "application/json", content);
}
void handleNotFound() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}

void setup(void) {
  Serial.begin(115200);
  Serial.println("======================");
  Serial.println("ardu");

  WiFi.mode(WIFI_OFF); //added to start with the wifi off, avoid crashing
  WiFi.persistent(false);



  WiFi.disconnect();   //added to start with the wifi off, avoid crashing

  WiFi.mode(WIFI_STA);

  int numSsid = WiFi.scanNetworks();
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") ");
    Serial.print(WiFi.SSID(thisNet));
    Serial.print("\tSignal: ");
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(" dBm");
    Serial.println("");
  }
  Serial.println("Send id e pwd");


  WiFi.config(ip, ipGW, ipMSK, ipDNS);

  WiFi.begin(ssid, password);
  Serial.println("======================");
  int cnCnt = 10;
  //   Wait for connection
  //&& (cnCnt>0)
  while ((WiFi.status() != WL_CONNECTED) ) {
    yield();
    delay(500);
    yield();
    Serial.print("!");
    //  cnCnt=cnCnt-1;
  }
  Serial.print("count ");  Serial.println(cnCnt);

  if (cnCnt == 0) {
    Serial.print("Access point mode  ");
    WiFi.persistent(false);
    WiFi.disconnect(true);

    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, ipMSK);
    WiFi.softAP("esp8266table");

    Serial.println();
    Serial.print("Server IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Server MAC address: ");
    Serial.println(WiFi.softAPmacAddress());



  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  //dnsServer.start(53, "*", ip);

  if (mdns.begin("esp8266table"), WiFi.localIP()) {
    Serial.println("MDNS responder started");
  }
  mdns.addService("http", "tcp", 80);
  // MDNS.addService("http", "tcp", 81);
  //timeClient.begin();

  Serial.println("WebSocket server started.");


  server.on("/", handleRoot);
  server.on("/setup", handleRootSetup);
  server.on("/setting", handleSettingSetup);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  //server.onNotFound(handleNotFound);
  server.onNotFound([]() {
    server.send(200, "text/html", "NO!!");
  });

  server.begin();
  Serial.println("HTTP server started");


  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 100);


  irrecv.enableIRIn(); // Start the receiver
  clsTable(colorLib[iiCol]);

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  if (!SPIFFS.begin()) Serial.println("SPIFFS.begin() failed");


  if (!ini.open()) {
    Serial.print("Ini file ");  Serial.println("NOT FOUND");

  } else {
    Serial.print("Ini file ");  Serial.println("FOUND !!");
  }
  // Check the file is valid. This can be used to warn if any lines
  // are longer than the buffer.
  if (!ini.validate(bufferINI, INIbufferLen)) {
    Serial.print("ini file ");
    Serial.print(ini.getFilename());
    Serial.print(" not valid buffer len ");


  }

  if (ini.getValue("generale", "ip", bufferINI, INIbufferLen)) {
    Serial.print("section 'generale' has an entry 'ip' with value ");
    Serial.println(bufferINI);
  }
  //irrecv.setTolerance(30);

  Serial.println("==end setup==============");
    pinMode(D0, OUTPUT);
FiveBEEPS();
  //debugnr();
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  switch (type) {
    case WStype_DISCONNECTED:             // if the websocket is disconnected
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {              // if a new websocket connection is established
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        // rainbow = false;                  // Turn rainbow off when a new connection is established
      }
      break;
    case WStype_TEXT:                     // if new text data is received
      Serial.printf("[%u] get Text: %s\n", num, payload);
      if (payload[0] == '#') {            // we get RGB data
        uint32_t rgb = (uint32_t) strtol((const char *) &payload[1], NULL, 16);   // decode rgb data
        int r = ((rgb >> 20) & 0x3FF);                     // 10 bits per color, so R: bits 20-29
        int g = ((rgb >> 10) & 0x3FF);                     // G: bits 10-19
        int b =          rgb & 0x3FF;                      // B: bits  0-9

        //analogWrite(LED_RED,   r);                         // write it to the LED output pins
        //analogWrite(LED_GREEN, g);
        //analogWrite(LED_BLUE,  b);
      } else if (payload[0] == 'R') {                      // the browser sends an R when the rainbow effect is enabled
        //rainbow = true;
      } else if (payload[0] == 'N') {                      // the browser sends an N when the rainbow effect is disabled
        //rainbow = false;
      }
      break;
  }
}






/*
  char c=valArr[1];
  printText35(&c,1,3,0,RED);
  c=valArr[3];
  Serial.println("CCC");
  Serial.println(c);
  printText35(&c,1,6,0,BLUE);
  c=valArr[4];
  Serial.println("CCC");
  Serial.println(c);
  printText35(&c,1,9,0,RED);
*/

/*
  c=valArr[3];

  printText35(&c,1,6,0,RED);
  c=valArr[4];

  printText35(&c,1,9,0,RED);
  Serial.println("CCC");
  Serial.println(c);
*/

void debugnr(void) {
  String payload = "";
  char valArr[3];

  clsTable(0);
  for (int r = 0; r < 12 ; r++) {
    for (int c = 0; c < 12 ; c++) {
      setTablePixel(r, c, GREEN);
      delay(100);
      refreshTable();
    }
  }


  for (uint8_t i = 0; i < 8; i++) {
    yield();
    Serial.println("Clstable");
    clsTable(colorLib[2]);
    delay(1000);

    payload = String(i);
    //if (i<9) payload=" "+payload;
    payload.toCharArray(valArr, 3);


    for (uint8_t c = 0; c < 12; c++) {
      clsTable(0);
      yield();
      Serial.println("Printtext32");
      Serial.println(valArr);
      printText35(valArr, 1, c, i, RED);
      delay(1000);
    }

  }

}

void common_IR(void) {
  currentMillis = millis();
  if ((buttonClicked == true) && (currentMillis - previousMillis > INTERVAL)) {
    keyRepeat = 0;
    previousMillis = 0;
    Serial.println("RESET KEYREP");
    buttonClicked = false;
  }

  if (irrecv.decode(&results)) {



    if (!buttonClicked)   keyRepeat = 0;
    buttonClicked = true;
    keyRepeat = keyRepeat + 1;
    previousMillis = currentMillis;
    if (keyRepeat > 8) {
      Serial.println("LONG CLICK DETECTED");
      buttonLONGClicked = true;
    }
    if (buttonClicked)
      if (CURRENT_MODE!=4) microBIP();
    Serial.print("keyRepeat  "); Serial.println(keyRepeat);

    serialPrintUint64(results.value, HEX);
    Serial.println("");
    unsigned long long1 = (unsigned long)((results.value & 0xFFFF0000) >> 16 );
    unsigned long long2 = (unsigned long)((results.value & 0x0000FFFF));

    String hex = String(long1, HEX) + String(long2, HEX); // six octets

    yield();  // Feed the WDT as the text output can take a while to print.

    webSocket.broadcastTXT(hex);


    irrecv.resume();  // Receive the next value
  };



} //common_IR
void common_loop (void) {

  //dnsServer.processNextRequest();
  mdns.update();
  webSocket.loop();
  server.handleClient();
  yield();
  common_IR();


}
void loop(void) {


  clsTable(0);



  common_loop();

  if (CURRENT_MODE == 1) {

    TABLE_Time();
    CURRENT_MODE = 2;
  }
  if (CURRENT_MODE == 2) {

    TABLE_Colors();
    CURRENT_MODE = 3;
  }
  if (CURRENT_MODE == 3) {

    TABLE_Effects();
    CURRENT_MODE = 4;
  }
  if (CURRENT_MODE == 4) {

    TABLE_Weahter();
    CURRENT_MODE = 5;
  }
   if (CURRENT_MODE == 5) {

    TABLE_Timer();
    CURRENT_MODE = 1;
  }

  common_IR();

  yield();

}
