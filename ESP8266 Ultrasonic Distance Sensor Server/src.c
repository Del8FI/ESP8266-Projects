// NodeMCU 1.0 esp-12E Module - esp8266 Ultrasonic Distance Sensor Server
// This code measures distance using an ultrasonic sensor and serves the distance readings over WiFi to connected clients.

#include <ESP8266WiFi.h>    // Include the ESP8266WiFi library
#include <WiFiClient.h>     // Include the WiFiClient library
#include <WiFiServer.h>     // Include the WiFiServer library

#define ep D0               // Echo pin (input)
#define tp D1               // Trigger pin (output)

char* ssid = "Hikari";     // WiFi network name (SSID)
char* password = "rdmc@3266";  // WiFi password

WiFiServer server(80);      // Create a server object listening on port 80

WiFiClient client;          // Create a WiFiClient object to handle incoming connections

void setup() {
  pinMode(ep, INPUT);       // Set echo pin as input
  pinMode(tp, OUTPUT);      // Set trigger pin as output
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
      long du, cm;
      digitalWrite(tp, LOW);                 // Send low signal to trigger pin
      delayMicroseconds(2);
      digitalWrite(tp, HIGH);                // Send high signal to trigger pin
      delayMicroseconds(10);
      digitalWrite(tp, LOW);                 // Send low signal to trigger pin
      du = pulseIn(ep, HIGH);                 // Measure the duration of the pulse in the echo pin
      cm =  du * 0.034 / 2;                   // Convert duration to distance (cm)
      Serial.println("Object at " + String(cm) + " cm"); // Print distance to serial monitor
      client.print("Object at " + String(cm) + " cm");   // Send distance to client
      delay(1000);
    } 
    Serial.println("Client disconnected");
  }    
}
