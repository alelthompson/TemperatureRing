#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Notes:
  // Make sure to install Adafruit ESP8266, ArduinoJson, and Adafruit NeoPixel libraries from Sketch > Include Library > Manage Libraries...
  // Arduino JSON Library MUST BE rollback version 5.12.0
  // Also add the following URLs to the Board Manager in Arduino > Prefences > Additional Board URLs
  // https://adafruit.github.io/arduino-board-index/package_adafruit_index.json,http://arduino.esp8266.com/versions/2.5.0/package_esp8266com_index.json, http://arduino.esp8266.com/stable/package_esp8266com_index.json

// Tutorials used in final solution:
  // Making HTTP Requests to Open Weather Maps
    //https://techtutorialsx.com/2018/03/17/esp32-arduino-getting-weather-data-from-api/
  // Parsing JSON data from HTTP Requests
    //https://www.instructables.com/id/ESP8266-Parsing-JSON/


// Enter current WiFi credentials 
  // The name of your network
  const char* ssid = "SSID Name";
  // Your password
  const char* password = "Password";

// Open Weather Maps location and Alex's API key
// Can change to any city with name and code like = "Ottawa,CA" or "Miami,US"
  const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Toronto,CA&APPID=";
  const String key = "0549a96e9ebfa92ca869522125b4403e";

// Set up NeoPixel ring with IN to Pin 14 
  #define PIN 14
  #define NUMPIXELS 24

// Set up the NeoPixel library 
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// In setup, we begin the serial monitor, start the Neopixels, and connect to WiFi
// Open Console as it will keep you updated on WiFi connection - may be slow
void setup() {
 
 Serial.begin(115200);

// Init the NeoPixel library
 pixels.begin(); // This initializes the NeoPixel library.

// Connect to Wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}


// In Loop we're going to run our API call function to get the current temperature 
void loop() {
  getTemp();
}


// GET request to the Open Weather server, store that data as JSON object, and parse it
// Then specify which data branch to grab (main > temp in this case)
void getTemp(){
    if ((WiFi.status() == WL_CONNECTED)) { // Check the current connection status
 
    HTTPClient http;
 
    http.begin(endpoint + key); // Specify the URL from the above URL and my API key
    int httpCode = http.GET();  // Make the request to that URL
 
    if (httpCode > 0) { // Error prevention before checking for payload return
        // Parsing
        const size_t bufferSize = JSON_ARRAY_SIZE(1) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 374;
        DynamicJsonBuffer jsonBuffer(bufferSize);
        JsonObject& root = jsonBuffer.parseObject(http.getString());

        int id = root["id"]; // 1
        // This line below is the key
        // It looks through the package of data we get back from Open Weather and specifically grabs Main > Temp 
        const int currentTemp = root["main"]["temp"]; // specifically grab temp
        Serial.println("The current temperature is ");
        Serial.println(currentTemp);


        // If statements to convert current Kelvin temperature into a color 
    // VERY COLD     
        if (currentTemp < 263){

            for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(0,0,102)); 
              pixels.show(); 
            }
  
    // COLD
          } else if (currentTemp < 264 && currentTemp < 270) {

            for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(51,51,204)); 
              pixels.show(); 
       }

    // CHILLY
          } else if (currentTemp < 271 && currentTemp < 273) {

            for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(102,153,255)); 
              pixels.show(); 
            }

    // FINE
          } else if (currentTemp < 274 && currentTemp < 278) {

            for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(102,204,153)); 
              pixels.show(); 
            }

    // COMFY
          } else if (currentTemp < 279 && currentTemp < 283) {

            for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(234,153,10)); 
              pixels.show(); 
            }

    // WARM
          } else if (currentTemp < 284 && currentTemp < 292) {

            for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(255,69,0)); 
              pixels.show(); 
            }

    // HOT
          } else if (currentTemp > 293) {

            for(int i=0;i<NUMPIXELS;i++){
              pixels.setPixelColor(i, pixels.Color(255,0,0)); 
              pixels.show(); 
            }
        }
        
      }
    else {
      Serial.println("Error on HTTP request"); // Print this if internal error occurred
    }
    http.end(); // Free the resources

  }

  // A delay of 60 seconds so we don't nake an unnecessary number of requests 
   // Note: This should actually be more like 10-30 minutes as temp doesn't change that often
    // Leaving it at 30 seconds for debugging and seeing it work
  delay(30000);
}


