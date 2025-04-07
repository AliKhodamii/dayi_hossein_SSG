#include<Arduino.h>
#include<ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include<ArduinoJson.h>
#include "../lib/functions.h"

// system variables
bool valve = true;
int humidity = 1000;

// urls
String postUrl = "http://sed-smarthome.ir/dayi_hossein/server/postInfo.php";
String getCmdUrl = "http://sed-smarthome.ir/dayi_hossein/server/getInfoWeb.php";


JsonDocument sysInfo,cmd;

// jsons
String sysInfoJson;
String cmdJson;

// time
long long int every5s = 0;

// request codes
int postResCode = 0;

void setup(){
  Serial.begin(9600);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

  String ssid = "TP_Sed"; 
  String password = "87654321";

  wifi_connect(ssid,password);

  //set initial values
  valve = false;
  humidity = humidity_read();

  //prepare data for sysInfo post
  sysInfo["millis"] = millis();
  sysInfo["valve"] = valve;
  sysInfo["humidity"] = humidity;
  sysInfoJson = json_encode(sysInfo);

  //post request to sysInfo to set new status 
  Serial.println("Posting init sysInfo");
  postResCode = httpPost(postUrl,"sysInfo",sysInfoJson);
  
  //prepare data for cmd post
  cmd["valveCmd"] = "noAction";
  cmd["restart"] = false;
  cmd["espConfirm"] = false;
  cmdJson = json_encode(cmd);

  //post request to cmd to set now command there
  Serial.println("Posting init cmdInfo");
  postResCode = httpPost(postUrl,"cmdInfo",cmdJson);
}

void loop(){
  //loop every 5 sec
  if(millis() - every5s >= 5000){
    Serial.println("");
    Serial.println("");
    Serial.print("--------");
    Serial.print("New loop");
    Serial.println("--------");
    Serial.println("");


    //get cmd file
    cmdJson = httpGet(getCmdUrl,"cmd");
    Serial.print("cmd is : ");
    Serial.println(cmdJson);

    //check if there is cmd

    //post status


    every5s = millis();
  }
}