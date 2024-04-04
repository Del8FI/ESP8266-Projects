// NodeMCU 1.0 ESP8266 Temperature, Humidity, and Light Monitoring with RFID Validation
// This Arduino sketch reads temperature and humidity values from a DHT11 sensor, light intensity from a photoresistor, and validates RFID tags.
// It communicates over WiFi and displays information through the serial monitor.

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <DHT.h>

// Pin definitions
int ld = A0; // Photoresistor pin
int b = D0;  // Buzzer pin
int r = D2;  // Red LED pin
int l = D3;  // Green LED pin
int DHT_pin = D1; // DHT sensor pin

#define Type DHT11  // Define type of DHT sensor

DHT HT(DHT_pin, Type); // Initialize DHT sensor object

float t, h; // Variables to store temperature and humidity values

char* ssid = "Hikari";       // WiFi SSID
char* password = "rdmc@3266"; // WiFi password

WiFiServer server(80); // Initialize WiFi server on port 80

WiFiClient client;

void setup() {
  HT.begin(); // Initialize DHT sensor
  pinMode(DHT_pin, INPUT); // Set DHT pin as input
  pinMode(ld, INPUT);       // Set photoresistor pin as input
  pinMode(b, OUTPUT);       // Set buzzer pin as output
  pinMode(r, OUTPUT);       // Set red LED pin as output
  pinMode(l, OUTPUT);       // Set green LED pin as output
  
  Serial.begin(9600); // Initialize serial communication
  WiFi.mode(WIFI_STA); // Set WiFi mode to station (client)
  WiFi.begin(ssid, password); // Connect to WiFi network
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  server.begin(); // Start the server
}

void loop() {
  WiFiClient client = server.available(); // Check for client connections
  
  if(client) {
    Serial.println("New client connected..");
    client.print("New client connected..");
    
    while (client.connected()) {
      if(Serial.available()) {
        String ch = Serial.readString(); // Read RFID tag from serial monitor
        
        Serial.print("ID = " + String(ch));
        client.print("ID = " + String(ch));
        delay(1000);
        
        if(ch == "19006FD36ACF") { // RFID tag validation
          Serial.println("VALID");
          client.print("VALID");
          digitalWrite(b, LOW); // Turn off buzzer
          int lv = analogRead(ld); // Read light intensity from photoresistor
          t = HT.readTemperature(); // Read temperature from DHT sensor
          h = HT.readHumidity();    // Read humidity from DHT sensor
          
          Serial.println("Temperature value = " + String(t));
          Serial.println("Humidity value = " + String(h));
          client.print("Temperature value = " + String(t));
          client.print("Humidity value = " + String(h));
          delay(2000);
          
          if(lv < 500) { // Check light intensity
            digitalWrite(l, HIGH); // Turn on green LED
            delay(5000);
          }
          
          if(t > 18) { // Check temperature
            digitalWrite(r, HIGH); // Turn on red LED
            delay(5000);
          } else {
            digitalWrite(r, LOW); // Turn off red LED
            digitalWrite(l, LOW); // Turn off green LED
            delay(2000);
          }
          delay(2000);
        } else {
          Serial.println("INVALID");
          client.print("INVALID");
          digitalWrite(r, LOW); // Turn off red LED
          digitalWrite(l, LOW); // Turn off green LED
          digitalWrite(b, HIGH); // Turn on buzzer
          delay(2000);
        }
      }
    }
    Serial.println("Client disconnected");
  }    
}
