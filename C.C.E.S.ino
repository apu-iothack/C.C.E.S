/*
 * Interface LDR with NodeMCU
 * By TheCircuit
 */
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"
WiFiClient client;

const char* ssid     = "who are weyh";
const char* password = "11333355555577777777";
int status = WL_IDLE_STATUS;

unsigned long myChannelNumber = 758073;
const char * myWriteAPIKey ="1RIG73E4TXM2V5CL";


const char* host = "wifitest.adafruit.com";

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  delay(100);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  ThingSpeak.begin (client);
}

void LDR() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage = sensorValue * (5.0 / 1023.0);
  
  // print out the value you read:
  Serial.println(voltage);
    ThingSpeak.setField(1,voltage);
  ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
}

int value = 0;
void loop(){
  LDR();
  delay(2000);

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/testwifi/index.html";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(500);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
}
