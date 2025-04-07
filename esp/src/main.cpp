#include<Arduino.h>
#include<ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include<ArduinoJson.h>
#include "../lib/functions.h"

// pins
int valveP = D7;

// system variables
bool valve = true;
bool copy = false;
int humidity = 1000;
int duration = 0;

// cmd variables
String valveCmd = "";

// urls
String postUrl = "http://sed-smarthome.ir/dayi_hossein/server/postInfo.php";
String getUrl = "http://sed-smarthome.ir/dayi_hossein/server/getInfoWeb.php";


JsonDocument sysInfo,cmd;

// jsons
String sysInfoJson;
String cmdJson;

// time
long long int every5s = 0;
int valveTimer = 0;
int copyTimer = 0;

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
  
  // get duration from sysInfo on host
  sysInfoJson = httpGet(getUrl,"sys");
  sysInfo = json_decode(sysInfoJson);
  duration = sysInfo["duration"];

  //prepare data for sysInfo post
  sysInfo["time"] = time();
  sysInfo["valve"] = valve;
  sysInfo["humidity"] = humidity;
  sysInfo["duration"] = duration;
  sysInfo["copy"] = copy;
  sysInfoJson = json_encode(sysInfo);

  //post request to sysInfo to set new status 
  Serial.println("Posting init sysInfo");
  postResCode = httpPost(postUrl,"sysInfo",sysInfoJson);
  
  //prepare data for cmd post
  cmd["valveCmd"] = "noAction";
  cmd["restartCmd"] = false;
  cmd["espConfirm"] = false;
  cmd["durationCmd"] = duration;
  cmdJson = json_encode(cmd);

  //post request to cmd to set now command there
  Serial.println("Posting init cmdInfo");
  postResCode = httpPost(postUrl,"cmdInfo",cmdJson);
}

void loop(){
  // loop every 5 sec
  if(millis() - every5s >= 5000){
    Serial.println("");
    Serial.println("");
    Serial.print("--------");
    Serial.print("New loop");
    Serial.println("--------");
    Serial.println("");


    // get cmd file
    Serial.println("Getting cmd");
    cmdJson = httpGet(getUrl,"cmd");
    cmd = json_decode(cmdJson);
    valveCmd = String(cmd["valveCmd"]);

    // check if open command is sent
    if(String(cmd["valveCmd"])== "open"){
      Serial.println("Open command received \n Opening valve");

      // open valve
      valve = true;
      copy = true;
      duration = cmd["durationCmd"];
      Serial.println("du:"+String(duration));

      // set time variables
      valveTimer = millis();
      copyTimer = millis();

      // post new status
      Serial.println("Posting new status");
      sysInfo["time"] = time();
      sysInfo["valve"] = valve;
      sysInfo["copy"] = copy;
      sysInfo["duration"] = duration;
      sysInfoJson = json_encode(sysInfo);
      httpPost(postUrl,"sysInfo",sysInfoJson);

      // post confirm and no action in cmd
      Serial.println("Posting new cmd");
      cmd["valveCmd"] = "noAction";
      cmd["confirm"] = true;
      cmdJson = json_encode(cmd);
      httpPost(postUrl,"cmdInfo",cmdJson);
    }

    // check if close command is sent
    if(String(cmd["valveCmd"]) == "close"){
       Serial.println("Close command received \n Closing valve");

      // open valve
      valve = false;
      copy = true;
      duration = cmd["durationCmd"];

      // post new status
      Serial.println("Posting new status");
      sysInfo["time"] = time();
      sysInfo["valve"] = valve;
      sysInfo["copy"] = copy;
      sysInfo["duration"] = duration;
      sysInfoJson = json_encode(sysInfo);
      httpPost(postUrl,"sysInfo",sysInfoJson);

      // post confirm and no action in cmd
      Serial.println("Posting new cmd");
      cmd["valveCmd"] = "noAction";
      cmd["confirm"] = true;
      cmdJson = json_encode(cmd);
      httpPost(postUrl,"cmdInfo",cmdJson);
    }

    // valve status
    valve ? digitalWrite(valveP,HIGH) : digitalWrite(valveP,LOW);

    // check if it's time to close valve
    if(valve && (millis() - valveTimer) > duration * 60000)
    {
      Serial.println("Duration is deon\n Valve is closing");
      valve = false;
      digitalWrite(valveP,LOW);

      // post new status
      Serial.println("Posting new status");
      sysInfo["time"] = time();
      sysInfo["valve"] = valve;
      sysInfo["copy"] = copy;
      sysInfo["duration"] = duration;
      sysInfoJson = json_encode(sysInfo);
      httpPost(postUrl,"sysInfo",sysInfoJson);

    }
    // post status
    Serial.println("Posting status");
  
    humidity = humidity_read();
    sysInfo["time"] = time();
    sysInfo["valve"] = valve;
    sysInfo["duration"] = duration;
    sysInfo["copy"] = copy;
    sysInfo["humidity"] = humidity;

    sysInfoJson = json_encode(sysInfo);
    httpPost(postUrl,"sysInfo",sysInfoJson);
    

    Serial.println("sysInfoJson:");
    Serial.println(sysInfoJson);

    every5s = millis();
  }

  // check if copy time is done
  if (millis() - copyTimer > 10000){
    copy = false;
  }
}