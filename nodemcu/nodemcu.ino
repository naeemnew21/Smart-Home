
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <Wire.h>
#include <SPI.h>
#include "RTClib.h"
#include "Ticker.h"
#include "SD.h"

#include "main.h"
#include "room1.h"
#include "room2.h"
#include "Schedule.h"
#include "addtable.h"

//********************************************************
//------------- Enter your network credentials -----------

const char* ssid = "2021";
const char* password = "1234567000";
// Set your Static IP address
IPAddress local_IP(1, 1, 1, 200);
// Set your Gateway IP address
IPAddress gateway(1, 1, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional


//************************************************
//-----------------  RTC --------------------

RTC_DS1307 RTC;
DateTime now;

//************************************************
//-----------------  SD card --------------------

const int chipSelect = D8;  // used for ESP8266

//**************************************************
//--------  Define paramaters -----------------------

const char* PARAM_INPUT_1 = "output";
const char* PARAM_INPUT_2 = "state";
const char* PARAM_INPUT_3 = "edit";
const char* PARAM_INPUT_4 = "del";
const char* PARAM_INPUT_5 = "save";
const char* PARAM_INPUT_6 = "sure";

//***************************************************************
//--------------- gpios status of serial communication ---------

String gpios = "{\"relay2\":null, \"relay3\":null, \"relay4\":null, \"relay5\":null, \"relay6\":null, \"relay7\":null, \"relay8\":null, \"relay9\":null, \"relay10\":null, \"relay11\":null, \"relay12\":null, \"relay13\":null }";
JSONVar GPIOs = JSON.parse(gpios);

String names[] = {"led1","led2","led3","led4","led5","led6","led7","led8","led9","led10","led11","led12"};
//----------------------
//days : [Sat, Sun, Mon, Tue, Wed, Thu, Fri]
JSONVar table_json; // = JSON.parse(main_json);
//overWrite("schedule.txt" , JSON.stringify(table_json));

String ShTime = "--:--"; 
String last_backup; // to prevent repeat alarm execution

//---------  variables used in loop to save received data from arduino  ----------
byte buf[13]; // buf[13] to check if array terminated
int count = 0;

JSONVar trans;
int current_change = 0;
int last_change = 0;
byte Device_io[13];

//**************************************************
//------ Create AsyncWebServer object on port 80 ----
AsyncWebServer server(80);
 

// Replaces placeholder with button section in your web page
String processor(const String& var)
{
 for(int i = 2; i<14; i++){
  String btn = "BUTTONPLACEHOLDER" + String(i);
  if(var == btn){
    String rely = "relay" + String(i);
    String button = "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id="+String(i)+" "+getState(rely)+"><span class=\"slider\"></span></label>";
    return button;
  }
 }
 
 if(var == "TABLE")
  {
    String table = "";
    String id;
    String val;
    int keys_len; 
    JSONVar Keys;
    
    if (JSON.stringify(table_json) == "{}"){
      keys_len = 0; 
    } else {
      Keys = table_json.keys();
      keys_len = Keys.length();
      // arrange list for times =================
      Keys = sort_array(Keys);
    }
    for (int i = 0; i < keys_len; i++) {
      JSONVar value = table_json[Keys[i]];
      
      table += "<div style=\" color: black; padding:20px; background-color:";
      if (i%2 == 0){table += "#ccccff" ; } //color of div
      else {table += "#f0f0f5" ;}     
      table += "\">";
      table += "<label class=\"label\">"+String(i+1)+"-&nbsp;&nbsp<span style=\"color:blue;font-weight:bold;font-size: 20px\">";
      id = JSON.stringify(Keys[i]);
      id.replace("\"", "");
      String show = H24_12(id); //edit time format
      table += show;
      table += "</span></label>";
      table += "<label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id="+id+" "+checkAlarm(Keys[i])+"><span class=\"slider\"></span></label>";
      table += "<a href=\"/red?edit="+id+"\" class=\"button button1\">Edit</button></a>" ;
      table += "<a href=\"/red?del="+id+"\" class=\"button button1\">Del</button></a>" ; 
      table += "<button class=\"collapsible "  ;
      if (i%2 == 0){table += "collapsible2\">" ; }
      else {table += "\">" ;}
      table += "Show details</button>";
      table += "<div class=\"content\">";
      
      // add remain time 
      table += "<p>Days : ";
      String days_show =JSON.stringify(value["days"]);
      days_show.replace("\"", "");
      table += days_show;

      JSONVar devices_array = value["devices"];
      JSONVar deviceKeys = devices_array.keys();
      for (int j = 0; j < deviceKeys.length(); j++) {
        table += "<p>";
        String dev = JSON.stringify(deviceKeys[j]);
        dev.replace("\"", "");
        table += names[dev.toInt()-2];
        table += " : ";
        if (int(devices_array[deviceKeys[j]]) == 1){
            table += "on";} else {table += "off";}
        table += "</p>";
      }
      table += "</div>";
      table += "</div>";
    }
    return table;
 }
 if(var == "TIMEINPUT")
  {
    String tim = "";
    if (ShTime != "--:--"){
      tim = "<input type=\"time\" id=\"appt\" name=\"appt\" value="+ ShTime +" style=\"font-size: 20px\">";
    }
    else {tim = "<input type=\"time\" id=\"appt\" name=\"appt\" value=\"06:00\" style=\"font-size: 20px\">";}
    return tim;
 }
 if(var == "WEEKDAYS")
  {
    String week_days = "";
    String DAYS[] = {"Sat","Sun","Mon","Tue","Wed","Thu","Fri"};
    for(int i=0; i<7; i++){
      String day_check = dayChecked(DAYS[i]);
      week_days += "<input type=\"checkbox\" id="+DAYS[i]+" class=\"weekday\" onchange=\"selectDay(this)\" "+day_check+"><label for="+DAYS[i]+">"+ DAYS[i].substring(0,2) +"</label>";
    }
    return week_days;
 }
 
 if(var == "JSONARRAY")
  {
    String vars = "";
    if (ShTime != "--:--"){
      String dys = JSON.stringify(table_json[ShTime]["days"]);
      String dvs = JSON.stringify(table_json[ShTime]["devices"]);
      vars += "const js = '{\"time\":\""+ShTime+"\", \"status\":1, \"days\":"+dys+", \"devices\":"+ dvs+"}';";
    }
    else { vars = "const js = '{\"time\":\"06:00\", \"status\":1, \"days\":[], \"devices\":{}}';";}
    vars += "const data = JSON.parse(js);";
    return vars;
 }
return String();
}

//================================================================
String getState(String relay)
{ 
  if((int)GPIOs[relay] == 1){return "checked";}
  else{return "";}
}

String checkAlarm(JSONVar key)
{ 
  int check = int(table_json[key]["status"]);
  if( check == 1){return "checked";}
  else{return "";}
}

String dayChecked(String dn){
  String dy = "";
  if (ShTime != "--:--"){  
      JSONVar Days = table_json[ShTime]["days"];
      for (int i=0; i < Days.length(); i++ ){
          if ( Days[i] == dn){ dy = "checked"; }
      }
  }
  return dy;
}

String getStringByIndex(String data, char separator, int index){   
    // spliting a string and return the part nr index
    // split by separator  
    int stringData = 0;        //variable to count data part nr 
    String dataPart = "";      //variable to hole the return text   
    for(int i = 0; i<data.length(); i++) {    //Walk through the text one letter at a time     
      if(data[i]==separator) {
        //Count the number of times separator character appears in the text
        stringData++;      
      }else if(stringData==index) {
        //get the text when separator is the rignt one
        dataPart.concat(data[i]);      
      }else if(stringData>index) {
        //return text and stop if the next separator appears - to save CPU-time
        return dataPart;
        break;     
      }
    }
    //return text if this is the last part
    return dataPart;
}

String H24_12(String str){
  String part1 = getStringByIndex(str, ':', 0);
  String part2 = getStringByIndex(str, ':', 1);
  String part3 = " AM";
  int hr = part1.toInt();
  if(hr >= 12){
    part3 = " PM";
  }
  if(hr > 12){
    hr = hr-12;
  }
  if(hr == 0){
    hr = 12;
  }
  String total = String(hr)+":"+part2+part3;
  return total;
}

JSONVar sort_array(JSONVar arr){
  JSONVar myarr;
  int conter = 0;
  for(int i=0; i<24; i++){
    for(int j=0; j<59; j++){
      for(int k=0; k<arr.length(); k++){
        String cloc = JSON.stringify(arr[k]);
        cloc.replace("\"", "");
        int x = getStringByIndex(cloc, ':', 0).toInt();
        int y = getStringByIndex(cloc, ':', 1).toInt();
        if( x == i && y == j){
          myarr[conter] = cloc;
          conter++;
        }
      }
    }
  }
  return myarr;
}
//================================================================
//========================= SD ===================================
String readFile(String file){
  File myFile = SD.open(file);  
  String data = "";
  while (myFile.available()) {
       data = data + (char)myFile.read();
  }
  myFile.close();
  if (data == ""){
    data = "{}";
  }
  return data;
}

void overWrite(String file, String data){
  String r = readFile(file);
  int x = r.length();
  int y = data.length();
  File myFile = SD.open(file, FILE_WRITE);  
  myFile.seek(0);
  if(myFile){
     myFile.print(data);
     if (x > y){
        for (int i =0; i<(x-y); i++){
            myFile.print(" ");
        }
     }
     myFile.close();
  }
}

//----------------------------------------------------------------
//--------------------------- RTC --------------------------------
String check_schedule(){
  now = RTC.now();
  char buf[] = "hh:mm";
  String key = now.toString(buf);
  if (key == last_backup){
    return "";
  }
  last_backup = key;
  if (table_json.hasOwnProperty(key)){
       JSONVar val = table_json[key];
       String devices_list = JSON.stringify(val["devices"]);
       if ((int)val["status"] == 1){
          if (val["days"].length() == 0){
            val["status"] = 0;
            table_json[key] = val;
            overWrite("schedule.txt" , JSON.stringify(table_json));
            return devices_list;
          }
          char Buf[] = "DDD";
          String Day = now.toString(Buf);
          for (int i = 0; i < val["days"].length(); i++) {
              if( val["days"][i] == Day){
                return devices_list;
              }  }  }  }
  return "";
}

unsigned long interv;

void execute_schedule(){
  JSONVar get_devices = JSON.parse(check_schedule()); 
  if(JSON.typeof(get_devices) != "undefined" ){
    trans = get_devices;
    current_change = !current_change;
  }
}

//===================================================================

Ticker timer(execute_schedule, 1000); 

//####################################################################################
void setup()
{
  Wire.begin(4,5); // Inicia el puerto I2C
  if (! RTC.begin()) {
    //Serial.println("Couldn't find RTC");
    while (1);
  }
  if (! RTC.begin()) {
    //Serial.println("Couldn't find RTC");
    while (1);
  }

  //
  if (!RTC.isrunning()) {
    //Serial.println("RTC lost power, lets set the time!");
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //RTC.adjust(DateTime(2021, 10, 23, 21, 22, 0));
  }
  
  delay(1000);
  // Serial port for debugging purposes
  Serial.begin(9600);
  while (!Serial) {
  }
  if (!SD.begin(chipSelect)) {
    //Serial.println("Initialization failed!");
    while (1);
  }

  String main_json = readFile("schedule.txt");
  table_json = JSON.parse(main_json);

  Device_io[12] = 3;
  
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    //Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
  delay(1000);
  //Serial.println("Connecting to WiFi..");
  }
  
  // Print ESP Local IP Address
  //Serial.println(WiFi.localIP());

  //----------------------------------------------------------------------------
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  {  request->send_P(200, "text/html", main_html, processor);  });

  server.on("/room1", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send_P(200, "text/html", room1_html, processor); });

  server.on("/room2", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send_P(200, "text/html", room2_html, processor); });

  server.on("/Schedule", HTTP_GET, [](AsyncWebServerRequest *request)
  { request->send_P(200, "text/html", Schedule_html, processor); });
  
  server.on("/addtable", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    if(table_json.hasOwnProperty("06:00")){ShTime = "06:00";}
    else {ShTime = "--:--";}
    request->send_P(200, "text/html", addtable_html, processor); });


  server.on("/red", HTTP_GET, [](AsyncWebServerRequest *request)
  { 
    String inputMessage;
    // PARAM_INPUT_3 = "edit";
    if (request->hasParam(PARAM_INPUT_3)){ 
      inputMessage = request->getParam(PARAM_INPUT_3)->value();
      if (table_json.hasOwnProperty(inputMessage) == 1){
          ShTime = inputMessage; }
      request->send_P(200, "text/html", addtable_html, processor);
    }
    
    //PARAM_INPUT_4 = "del";
    if (request->hasParam(PARAM_INPUT_4)){
      inputMessage = request->getParam(PARAM_INPUT_4)->value();
      table_json[inputMessage] = undefined;
      overWrite("schedule.txt" , JSON.stringify(table_json));
      request->send_P(200, "text/html", Schedule_html, processor);
    }
    
    //PARAM_INPUT_5 = "save";
    if (request->hasParam(PARAM_INPUT_5)){
      inputMessage = request->getParam(PARAM_INPUT_5)->value();
      JSONVar addnew = JSON.parse(inputMessage);
      JSONVar ddd = addnew["days"];
      JSONVar DDD = addnew["devices"];
      if (JSON.stringify(DDD) == "{}"){
        return; }
      table_json[addnew["time"]]["status"] = 1;
      table_json[addnew["time"]]["days"] = ddd;
      table_json[addnew["time"]]["devices"] = DDD;
      overWrite("schedule.txt" , JSON.stringify(table_json));
      request->send(200, "text/plain", "OK");
    }

    //PARAM_INPUT_6 = "sure";
    if (request->hasParam(PARAM_INPUT_6)){
      inputMessage = request->getParam(PARAM_INPUT_6)->value();
      inputMessage.replace("\"", "");
      request->send(200, "text/html", String(table_json.hasOwnProperty(inputMessage)));
    }
    
   });

  server.on("/toggle", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    String inputMessage1;
    String inputMessage2;
    // PARAM_INPUT_3 = "edit"; PARAM_INPUT_2 = "state";
    if (request->hasParam(PARAM_INPUT_3) && request->hasParam(PARAM_INPUT_2))
    {
      inputMessage1 = request->getParam(PARAM_INPUT_3)->value();
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      table_json[inputMessage1]["status"] = inputMessage2.toInt();
      overWrite("schedule.txt" , JSON.stringify(table_json));
    }
    request->send(200, "text/plain", "OK");
  });


  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    String param1;
    String param2;
    byte Device[2];
    // PARAM_INPUT_1 = "output"; PARAM_INPUT_2 = "state";
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2))
    {
      param1 = request->getParam(PARAM_INPUT_1)->value();
      param2 = request->getParam(PARAM_INPUT_2)->value();
      Device[0] = param1.toInt();
      Device[1] = param2.toInt();
      Serial.write(Device, 2);
    }
    request->send(200, "text/plain", "OK");
  });

  //----------------------------------------------------------------------------
  
  server.on("/refresh", HTTP_GET, [] (AsyncWebServerRequest *request)
  {
    String states_json = JSON.stringify(GPIOs); 
    request->send(200, "text/plain", states_json.c_str());  // send all states as json
  });

  // Start time check for schedules
  timer.start(); 
  // Start server
  server.begin();
}

void loop()
{
    timer.update();
    
    int Rx;
    while (Serial.available()) {
      Rx = Serial.read();
      if (count == 13 || Rx == 3){
        JSONVar key = GPIOs.keys();
        for (int i=0; i<12; i++){
          GPIOs[key[i]] = buf[i];
        }
        count = 0;
        break;
      }
      if (Rx==0 || Rx==1){
        buf[count] = Rx;
      }
      count++;
    }

    if (current_change != last_change){
    JSONVar K_arr = trans.keys();
    for(int i = 0; i<K_arr.length(); i++){
      byte Devi[2];
      String pin = JSON.stringify(K_arr[i]);
      pin.replace("\"", "");
      Devi[0] = pin.toInt();  // key : gpio number
      Devi[1] = int(trans[K_arr[i]]); // state : 0/1
      Serial.write(Devi, 2 ); // 
      interv = millis();
      while(millis() - interv < 100);
      int rx;
      while (Serial.available()) {
        rx = Serial.read();
        if (count == 13 || rx == 3){
          JSONVar key = GPIOs.keys();
          for (int i=0; i<12; i++){
            GPIOs[key[i]] = buf[i];
          }
          count = 0;
          break;
        }
        if (rx==0 || rx==1){
          buf[count] = rx;
        }
        count++;
      }
    }
    last_change = current_change;
  }

}
