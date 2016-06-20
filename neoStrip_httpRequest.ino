#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <NeoPixelBus.h>
#define USE_SERIAL Serial

const uint16_t PixelCount = 8; // this example assumes 4 pixels, making it smaller will cause a failure
const uint8_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266

const char* ssid = "Be_Maker";
const char* password = "pulpoparacomer";
NeoPixelBus<NeoGrbFeature, NeoEsp8266Uart800KbpsMethod> strip(PixelCount, PixelPin);
int led = 13;
String payload;
HTTPClient http;

void setup() {
  strip.Begin();
    strip.Show();

    pinMode(led, OUTPUT);
    digitalWrite(led, 0);
    USE_SERIAL.begin(115200);
    USE_SERIAL.println("INICIO");
    WiFi.begin(ssid, password);
    
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      USE_SERIAL.print(".");
    }
    USE_SERIAL.println("");
    USE_SERIAL.print("Connected to ");
    USE_SERIAL.println(ssid);
    USE_SERIAL.print("IP address: ");
    USE_SERIAL.println(WiFi.localIP());
  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }



    
    USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
    http.begin("http://www.desenlace.net/node/node2.php"); //HTTP
   int httpCode = http.GET();
    if(httpCode == HTTP_CODE_OK) {
       payload = http.getString();
    }
   
}

void parseStrToColor(String str){
for(int i=0;i<8; i++){
  String pixelStr = str.substring(i*6,i*6+6);
   long number = strtol( &pixelStr[0], NULL, 16);

// Split them up into r, g, b values
 long r = number >> 16;
 long g = number >> 8 & 0xFF;
 long b = number & 0xFF;
  strip.SetPixelColor(i, RgbColor(r,g,b));
}
strip.Show();
  
}

void loop() {
    // wait for WiFi connection
    

        
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("(%ld)[HTTP] GET... code: %d\n",millis(), httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                for(int i=0 ; i<30; i++){
                  parseStrToColor(payload.substring(48*i, 48*i+48)); 
                  //USE_SERIAL.println(payload);
                  if(i<29)delay(30);
              }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    
}
