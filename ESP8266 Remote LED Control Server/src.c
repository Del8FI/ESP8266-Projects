// NodeMCU 1.0 esp-12E Module - esp8266 Remote LED Control
// This code allows remote control of two LEDs connected to pins D0 and D2 using commands received over WiFi from connected clients.

#include <ESP8266WiFi.h>    // Include the ESP8266WiFi library
#include <WiFiClient.h>     // Include the WiFiClient library
#include <WiFiServer.h>     // Include the WiFiServer library

int l = D0;                 // Pin connected to the first LED
int l2 = D2;                // Pin connected to the second LED

char* ssid = "Hikari";     // WiFi network name (SSID)
char* password = "rdmc@3266";  // WiFi password

WiFiServer server(80);      // Create a server object listening on port 80

WiFiClient client;          // Create a WiFiClient object to handle incoming connections

void setup() {
  pinMode(l, OUTPUT);       // Set first LED pin as output
  pinMode(l2, OUTPUT);      // Set second LED pin as output
  Serial.begin(9600);       // Initialize serial communication
  WiFi.mode(WIFI_STA);      // Set WiFi mode to station (client)
  WiFi.begin(ssid, password); // Connect to WiFi network

  // Wait for connection to be established
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
 
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP()); // Print local IP address
  server.begin(); // Start the server
}

void loop() {
  WiFiClient client = server.available(); // Check for client connections

  if(client) {
    Serial.println("New client connected..");
    client.print("New client connected..");
    
    // Loop to handle incoming data from client
    while (client.connected()) {
      String data = client.readString(); // Read data from client
      Serial.println("Data received: " + String(data)); // Print received data
      client.print("Data received: " + String(data)); // Send received data back to client
      
      // Perform actions based on received data
      if(data == "red on") {          // Turn on the first LED
        digitalWrite(l, HIGH);
        delay(2000);
      } else if(data == "ye on") {    // Turn on the second LED
        digitalWrite(l2, HIGH);
        delay(2000);
      } else if(data == "red off") {  // Turn off the first LED
        digitalWrite(l, LOW);
        delay(2000);
      } else if(data == "ye off") {   // Turn off the second LED
        digitalWrite(l2, LOW);
        delay(2000);
      }
      
      delay(1000);
    } 
    Serial.println("Client disconnected");
  }    
}
