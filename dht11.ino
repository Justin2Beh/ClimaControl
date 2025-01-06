#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>

// DHT11 sensor setup
#define DHTPIN 2           // Pin where the DHT11 sensor is connected (D2 on Wemos D1)
#define DHTTYPE DHT11      // Define sensor type (DHT11)
DHT dht(DHTPIN, DHTTYPE); // Initialize DHT sensor

// WiFi credentials
const char* ssid = "myREZ - Guest";       // WiFi SSID
const char* password = "181lesterwifi";   // WiFi password

// Server details
const char* serverUrl = "http://example.com/submit-data"; // Replace with your server's URL

WiFiClient wifiClient; // Create a WiFi client object

void setup() {
  // Initialize the built-in LED pin as an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Start Serial communication
  Serial.begin(115200);
  Serial.println("Starting...");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  int timeout = 20; // Timeout after 10 seconds (20 * 500ms)
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi.");
  }

  // Initialize DHT sensor
  dht.begin();
}

void loop() {
  // Read temperature and humidity from DHT11
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature(); // Read temperature as well

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return; // Exit loop if the readings are invalid
  } 

  // Print the readings to Serial
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  // Send the data to the server
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http; // Create an HTTP client

    // Use the new API with WiFiClient
    http.begin(wifiClient, serverUrl); // Specify the server URL
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); // Set the content type

    // Prepare the data to send
    String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity);

    // Send the POST request
    int httpResponseCode = http.POST(postData);

    // Print the response
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response Code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error in sending POST: ");
      Serial.println(http.errorToString(httpResponseCode).c_str());
    }

    http.end(); // Close the connection
  } else {
    Serial.println("WiFi is not connected.");
  }

  // Wait 10 seconds before sending the next reading
  delay(10000);
}
