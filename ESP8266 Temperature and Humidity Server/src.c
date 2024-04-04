// NodeMCU 1.0 esp-12E Module - esp8266 Temperature and Humidity Monitoring
// This code reads temperature and humidity values from a DHT11 sensor and serves them over WiFi to connected clients.

#include <ESP8266WiFi.h>    // Include the ESP8266WiFi library
#include <WiFiClient.h>     // Include the WiFiClient library
#include <WiFiServer.h>     // Include the WiFiServer library

#include <DHT.h>            // Include the DHT library

int DHT_pin = D1;           // Pin connected to the DHT sensor
#define Type DHT11          // Define type of DHT sensor

DHT HT(DHT_pin, Type);      // Initialize DHT sensor object

float t, h;                 // Variables to store temperature and humidity values

char* ssid = "Hikari";      // WiFi network name (SSID)
char* password = "rdmc@3266";  // WiFi password

WiFiServer server(80);       // Create a server object listening on port 80

WiFiClient client;           // Create a WiFiClient object to handle incoming connections

void setup() {
  HT.begin();               // Initialize DHT sensor
  pinMode(DHT_pin, INPUT);  // Set DHT pin as input
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
      t = HT.readTemperature();  // Read temperature from DHT sensor
      h = HT.readHumidity();     // Read humidity from DHT sensor
      Serial.println("Temperature value = " + String(t));
      Serial.println("Humidity value = " + String(h));
      client.print("Temperature value = " + String(t));  // Send temperature to client
      client.print("Humidity value = " + String(h));     // Send humidity to client
      delay(1000);
   } 
    Serial.println("Client disconnected");
  }    
}
