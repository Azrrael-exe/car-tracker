#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

uint8_t postData(String inp, String &res){
  HTTPClient http;
  http.begin("http://192.168.0.130:8000/");
  http.addHeader("Content-Type", "application/json");
  http.addHeader("api-key", "Hello Wold");
  int httpCode = http.POST(inp);
  String payload = http.getString();
  // Serial.println(httpCode);
  // Serial.println(payload);
  res = payload;
  http.end();
  return httpCode;
}
String readGPS(){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject&  req = jsonBuffer.createObject();
  req["lat"] = random(0, 100);
  req["lon"] = random(0, 100);
  String out;
  req.printTo(out);
  return out;
}

void setup() {
  Serial.begin(115200);                         //Serial connection
  WiFi.begin("Azrrael2GHz", "1128264989ABC");   //WiFi connection
  while (WiFi.status() != WL_CONNECTED) {       //Wait for the WiFI connection completion
    delay(500);
    Serial.println("Waiting for connection");
  }
}

void loop() {
  if(WiFi.status()== WL_CONNECTED){
    String response;
    uint8_t code = postData(readGPS(), response);
    Serial.print("code: ");Serial.println(code);
    Serial.println(response);
  }
  else{
    Serial.println("Error in WiFi connection");
  }
  delay(120000);  //Send a request every 30 seconds
}
