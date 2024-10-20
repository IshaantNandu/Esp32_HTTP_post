#include <WiFi.h>
#include <WebServer.h>
#include <Arduino.h>
#include <DHT.h>

// Replace with your WiFi credentials
const char* ssid = "SSID";
const char* password = "SecretPassword";

// DHT Sensor settings
#define DHTPIN 2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);

void notfound(){
  server.send(404,"text/plain","Not Found");
}

void handleRoot() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature(); 
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!")); 
    server.send(500, "text/plain", "Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // Create the JSON response as a String
  String json = "{";
  json += "\\"temperature\\":"  + String(t) + ",";
  json += "\\"humidity\\":"  + String(h) + ",";
  json += "\\"heatIndex\\":"  + String(hic); 
  json += "}";

  // Set the response content type to application/json
  server.sendHeader("Content-Type", "application/json");
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up the web server
  server.on("/", handleRoot);
  server.onNotFound(notfound);
  server.begin();
  Serial.println("Web server started");
  Serial.print("http://");Serial.print(WiFi.localIP());
}

void loop() {
  server.handleClient();

  // Send the JSON data every 2 seconds
  static unsigned long lastSendTime = 0;
  if (millis() - lastSendTime >= 2000) {
    handleRoot();
    lastSendTime = millis();
  }
}
