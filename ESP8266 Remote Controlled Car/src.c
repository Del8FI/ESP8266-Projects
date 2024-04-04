// NodeMCU 1.0 esp-12E Module - esp8266 Remote Controlled Car
// This code controls the movement of a car with four motors (m1, m2 for right wheel, m3, m4 for left wheel) using commands received over WiFi from connected clients.

#include <ESP8266WiFi.h>    // Include the ESP8266WiFi library
#include <WiFiClient.h>     // Include the WiFiClient library
#include <WiFiServer.h>     // Include the WiFiServer library

int m1 = D1;                // Pin connected to the first motor of the right wheel
int m2 = D2;                // Pin connected to the second motor of the right wheel
int m3 = D3;                // Pin connected to the first motor of the left wheel
int m4 = D4;                // Pin connected to the second motor of the left wheel

char* ssid = "Hikari";     // WiFi network name (SSID)
char* password = "rdmc@3266";  // WiFi password

WiFiServer server(80);      // Create a server object listening on port 80

WiFiClient client;          // Create a WiFiClient object to handle incoming connections

void setup() {
  pinMode(m1, OUTPUT);      // Set first motor of right wheel pin as output
  pinMode(m2, OUTPUT);      // Set second motor of right wheel pin as output
  pinMode(m3, OUTPUT);      // Set first motor of left wheel pin as output
  pinMode(m4, OUTPUT);      // Set second motor of left wheel pin as output
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
      Serial.println("Input = " + String(data)); // Print received data
      
      // Perform actions based on received data
      if(data == "f") {             // Move forward
        digitalWrite(m1, LOW);
        digitalWrite(m2, HIGH);
        digitalWrite(m3, LOW);
        digitalWrite(m4, HIGH);
        delay(5000);
      } else if(data == "b") {      // Move backward
        digitalWrite(m1, HIGH);
        digitalWrite(m2, LOW);
        digitalWrite(m3, HIGH);
        digitalWrite(m4, LOW);
        delay(5000);
      } else if(data == "r") {      // Turn right
        digitalWrite(m2, HIGH); 
        digitalWrite(m1, LOW);
        digitalWrite(m3, HIGH);
        digitalWrite(m4, LOW);
        delay(5000);
      } else if(data == "l") {      // Turn left
        digitalWrite(m1, HIGH); 
        digitalWrite(m2, LOW);
        digitalWrite(m4, HIGH);
        digitalWrite(m3, LOW);
        delay(5000);
      } else {                      // Stop
        digitalWrite(m1, HIGH); 
        digitalWrite(m2, HIGH);
        digitalWrite(m3, LOW);
        digitalWrite(m4, LOW);
        delay(5000);
      }    
    } 
    Serial.println("Client disconnected");
  }    
}
