// NodeMCU 1.0 esp-12E Module - esp8266 WiFi Data Relay
// This code establishes a WiFi connection, listens for client connections on port 80,
// and relays any received data back to the client.

#include <ESP8266WiFi.h>    // Include the ESP8266WiFi library
#include <WiFiClient.h>     // Include the WiFiClient library
#include <WiFiServer.h>     // Include the WiFiServer library

char* ssid = "Hikari";     // WiFi network name (SSID)
char* password = "rdmc@3266";  // WiFi password
WiFiServer server(80);      // Create a server object listening on port 80

WiFiClient client;          // Create a WiFiClient object to handle incoming connections

void setup() {
  Serial.begin(9600);      // Initialize serial communication
  WiFi.mode(WIFI_STA);     // Set WiFi mode to station (client)
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
      delay(1000);
    } 
    Serial.println("Client disconnected");
  }    
}
