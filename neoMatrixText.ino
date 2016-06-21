// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>

#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>

#define USE_SERIAL Serial


#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 2

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// Example for NeoPixel Shield.  In this application we'd like to use it
// as a 5x8 tall matrix, with the USB port positioned at the top of the
// Arduino.  When held that way, the first pixel is at the top right, and
// lines are arranged in columns, progressive order.  The shield uses
// 800 KHz (v2) pixels that expect GRB color data.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(4, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255) };


const char* ssid = "Be_Maker";
const char* password = "pulpoparacomer";

String payload="hola";
long lon= 10*payload.length();
  
HTTPClient http;

void setup() {
   
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

  
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(40);
  matrix.setTextColor(colors[0]);
}

int x    = matrix.width();
int pass = 0;
long last_time_exec=0;
void loop() {

  if(millis() > last_time_exec + 200){
    last_time_exec=millis();
    matrix.fillScreen(0);
    matrix.setCursor(x, 0);
    matrix.print(payload);
    x--;
    http.begin("http://justmine.me/arduino/getLedInfo.php"); //HTTP
     int httpCode = http.GET();
      if(httpCode == HTTP_CODE_OK) {
        String payload_aux = http.getString();
        if(payload != payload_aux){
           x = matrix.width();
           payload= payload_aux;
           lon= 7*payload.length();
           USE_SERIAL.println(lon); 
        }
           
      }
    if(x < -lon) { //palabra acabada
      x = matrix.width();
    }
    matrix.show();

    }
}
