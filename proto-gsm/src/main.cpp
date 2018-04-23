#include <Arduino.h>

#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <ArduinoJson.h>

#define SerialMon Serial
#define SerialAT Serial1

//#include <SoftwareSerial.h>
//SoftwareSerial SerialAT(2, 3); // RX, TX

const char apn[]  = "web.vmc.net.co";
const char user[] = "";
const char pass[] = "";

// Server details
const char server[] = "190.29.76.174";
const char resource[] = "/";
const int  port = 8000;

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
HttpClient http(client, server, port);

int postData(HttpClient &http, String req, String &res){
  if (modem.isNetworkConnected()) {
    if (!modem.gprsConnect(apn, user, pass)) {
      SerialMon.println(" fail");
      delay(10000);
      return -1;
    }
    http.beginRequest();
    http.post("/");
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader(HTTP_HEADER_CONTENT_LENGTH, req.length());
    http.sendHeader("api-key", "Hello Wold");
    http.endRequest();
    http.write((const byte*)req.c_str(), req.length());

    int statusCode = http.responseStatusCode();
    res = http.responseBody();
    return statusCode;
  }
  else {
    return -1;
  }
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
  SerialMon.begin(115200);
  delay(10);

  SerialAT.begin(115200);
  delay(3000);

  SerialMon.println(F("Initializing modem..."));
  modem.init();

  modem.simUnlock("1234");
}


void loop() {
  if(modem.isNetworkConnected()){
    if (!modem.gprsConnect(apn, user, pass)) {
      SerialMon.println(" fail");
      delay(10000);
    }
    String response;
    int code = postData(http, readGPS(), response);
    Serial.print("code: ");Serial.println(code);
    Serial.println(response);
    modem.gprsDisconnect();
  }
  else{
    Serial.println("Error in WiFi connection");
  }
  delay(120000);  //Send a request every 30 seconds
}
