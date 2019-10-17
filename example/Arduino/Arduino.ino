
/*
  Simple POST client for ArduinoHttpClient library
  Connects to server once every five seconds, sends a POST request
  and a request body

  created 14 Feb 2016
  modified 22 Jan 2019
  by Tom Igoe
  modified 2019/10/17
  by myuan
  this example is in the public domain
 */
#include <ArduinoHttpClient.h>
#include <WiFi101.h>
#include <ArduinoJson.h>

#include "arduino_secrets.h"

#define ProtocolVersion 2
#define DeviceID 1
#define ProjectID 1

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

char serverAddress[] = "10.18.52.137";  // server address
int port = 9997;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

const int capacity = JSON_OBJECT_SIZE(5);
StaticJsonDocument<capacity> doc;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  Serial.println("making POST request");
  
  
  String contentType = "application/json";
  doc["ProtocolVersion"] = ProtocolVersion;
  doc["DeviceID"] = DeviceID;
  doc["CreateTime"] = "";
  doc["ProjectID"] = ProjectID;
  doc["Data"] = millis();
  String postData = "";
  serializeJson(doc, postData);
  
  client.post("/", contentType, postData);

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

  Serial.println("Wait five seconds");
  delay(5000);
}
