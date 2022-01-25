#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

const char* ssid = "";
const char* pass = "";

String formatHTML(String internals) {
  String toSend;
  toSend.reserve(1024);
  toSend =  "<!DOCTYPE HTML>\r\n<html>\r\n<head>\r\n<title>Lab 3 pt. A</title>\r\n</head>\r\n<body>\r\n";
  toSend += internals;
  toSend += "</body>\r\n</html>";
  return toSend;
}

void page_index() {
  server.send(200, "text/html", formatHTML("<h1>Jarrod Wardell - 000809938</h1>\r\n<a href=\"/reset\">Reset</a>\r\n<a href=\"/adc\">Display ADC</a>\r\n"));
}

void page_reset() {
  server.send(200, "text/html", formatHTML("<h1>Goodbye!</h1>\r\n<script>window.addEventListener('load', e => {window.location.replace('/')})</script>"));
  delay(2000);
  ESP.restart();
}

void page_adc() {
  server.send(200, "text/html", formatHTML("<h1>Jarrod Wardell - 000809938</h1>\r\n<a href=\"/reset\">Reset</a><br>\r\n<a href=\"/adc\">Display ADC</a>\r\n<p>Current ADC:" + String(analogRead(A0)) + "</p>\r\n"));
}

void page_not_found() {
  server.send(404, "text/html", formatHTML("<h1>No such page!</h1>"));
}

void connectWiFi() {
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("\nConnected: ");
  Serial.println(WiFi.localIP());

  server.on("/", page_index);
  server.on("/reset", page_reset);
  server.on("/adc", page_adc);
  server.onNotFound(page_not_found);
  server.begin();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Meep");

  connectWiFi();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (WiFi.status() != WL_CONNECTED) {
    WiFi.disconnect();
    connectWiFi();
  }
  server.handleClient();
}