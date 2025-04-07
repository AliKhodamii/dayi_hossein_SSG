#include <Arduino.h>
#include<ESP8266WiFi.h>
#include<ArduinoJson.h>
#include<ESP8266HTTPClient.h>

bool wifi_connect(String ssid , String password){
    WiFi.begin(ssid,password);
    Serial.print("Connecting to ");
    Serial.print(ssid);
    
    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());

    if(WiFi.status() == WL_CONNECTED){
        return true;
    }
    else{
        return false;
    }
}

String json_encode(JsonDocument data){
    String result = "";
    serializeJson(data , result);
    return result;
}

JsonDocument json_decode (String json){
    JsonDocument data;
    deserializeJson(data , json);
    return data;
}

int httpPost(String url , String destination , String data){
    data = destination+"="+data;

    WiFiClient client;
    HTTPClient http;
    http.begin(client , url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResCode = http.POST(data);
    Serial.print("Http post response code: ");
    Serial.println(httpResCode);
    return httpResCode;
}

String httpGet(String url , String filename) {
  WiFiClient client;
  HTTPClient http;
  url = url+"?file="+filename;
  http.begin(client, url);
  int httpResponseCode = http.GET();
  String payload = "{}"; 
  if (httpResponseCode>0) {
    Serial.print("Http get response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}

int humidity_read(){
    int humidity = 0;
    for(int i=0 ; i<10 ; i++){
        humidity += analogRead(A0);
    }
    humidity = humidity/10;
    return humidity;
}

String time(){
    int mil = millis();
    int d = mil/86400000;
    mil -= (d*86400000);
    int h = mil/3600000;
    mil -=h*3600000;
    int m = mil/60000;
    mil -= m*60000;
    int s = mil / 1000;

    String time = String(d) + " : " + String(h) + " : " + String(m) + " : " + String(s);
    return time;
}