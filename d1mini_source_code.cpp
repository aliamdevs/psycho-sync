#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

// Local Vars :
const char* ssid = "Aliam_Server_001";
const char* password = "12345678"; 

#define LEDPIN D1
const int chipSelect = D8; 
String GG = "" ;

ESP8266WebServer server(80);  

void blinkLED(int N , int T) {
  for(int i=0; i<N; i++) {
    digitalWrite(LEDPIN, HIGH);
    delay(T);
    digitalWrite(LEDPIN, LOW);
    delay(T);
  }
}

void handleRoot(){
  String html = "<html><body>";
  html += "<h1>D1 Mini LED Control</h1>";
  html += "<p style="">AP IP: " + WiFi.softAPIP().toString() + "</p>";

  html += "<p>Text To Show : " + GG + "</p>";
  
  html += "<form action='/blink' method='post'>";
  html += "Num Of Blink : <input type='text' name='num'><br>";
  html += "<input type='submit' value='Blink LED (2)'>";
  html += "</form>";

  html += "<h2>Upload Text</h2>";
  html += "<form action='/' method='post'>";
  html += "Name: <input type='text' name='name'><br>";
  html += "Context: <input type='text' name='context' value=" "><br>";
  html += "<input type='text' name='mode' hidden disabled value='1'><br>";
  html += "<input type='submit' value='Submit'>";
  html += "</form>";

  html += "<h2>Download Text</h2>";
  html += "<form action='/' method='post'>";
  html += "Name: <input type='text' name='name'><br>";
  html += "<input type='text' name='context' hidden disabled value=" "><br>";
  html += "<input type='text' name='mode' hidden disabled value='0'><br>";
  html += "<input type='submit' value='Submit'>";
  html += "</form>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

void handleBlinkPost() {
  if (server.method() != HTTP_POST) {
    server.send(405, "application/json","{\"code\":\"405\",\"data\":\"Try_HTTP_POST\"}");
  } else {
    String num = server.arg("num");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"DONE\"}");
    blinkLED(num.toInt(), 100);
  }
}

void WriteInFile(String name,String text){
  File file;
  file = SD.open("/"+name+".txt", FILE_WRITE);
  if (file) {
    Serial.print("Writing to file...");
    file.print(text);
    file.close();
    Serial.println("---> done.");
    blinkLED(4 , 30);
  }else {
    blinkLED(20,30);
  }
}

String ReadFromFile(String name){
  File myFile;
  String ret ;
  myFile = SD.open("/"+name+".txt");
  if (myFile) {
    Serial.println(name+".txt:");
    while (myFile.available()) {
      ret = myFile.readString();
    }
    myFile.close();
    blinkLED(4 , 30);
  } else {
    blinkLED(20,30);
  }
  return ret;
}

String listDirectory(String dirname) {
  String Tree = "";
  Tree += " -> Listing directory: "+dirname+"\n";

  File root = SD.open(dirname);
  if (!root) {
    Tree+= " -> Failed to open directory\n";
    return "";
  }
  if (!root.isDirectory()) {
    Tree+= " -> Not a directory\n";
    return "";
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Tree+=  " -> DIR: ";
    } else {
      Tree+=  " -> FILE: " ;
    }
    Tree+= "-> " + String(file.name());
    if (!file.isDirectory()) {
      Tree+=  " -> (";
      Tree+=  file.size();
      Tree+=  " bytes)";
    }
    Tree+= "\n";
    file = root.openNextFile();
  }
  
  file.close();
  root.close();
  return(Tree);
  blinkLED(4 , 30);
}

void deleteAllFiles(String dirname) {
  File root = SD.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    String filePath = String(dirname) + "/" + String(file.name());
    
    if (file.isDirectory()) {
      // Recursively delete contents of subdirectory
      deleteAllFiles(filePath.c_str());
      // Then remove the empty directory
      SD.rmdir(filePath.c_str());
      Serial.print("Deleted directory: ");
      Serial.println(filePath);
    } else {
      // Delete the file
      SD.remove(filePath.c_str());
      Serial.print("Deleted file: ");
      Serial.println(filePath);
    }
    file = root.openNextFile();
  }
  if (String(dirname) != "/") {
    SD.rmdir(dirname);
  }
  file.close();
  root.close();
}

String retBytes(String dirname) {
  int b = 0;
  File root = SD.open(dirname);
  if (!root) {
    return String(b);
  }
  if (!root.isDirectory()) {
    return "NOROOT";
  }
  File file = root.openNextFile();
  while (file) {
    if (!file.isDirectory()) {
      b +=  file.size();
    }
    file = root.openNextFile();
  }
  file.close();
  root.close();
  return(String(b));
  blinkLED(4 , 30);
}


void handleRootPost(){
  String name = server.arg("name");
  String context = server.arg("context");
  String mde = server.arg("mode");
  int mode = mde.toInt();
  if(mode == 0){
  String tmp;
    tmp = ReadFromFile(name);
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\""+tmp+"\"}");
  }else if(mode == 1){
    WriteInFile(name , context);
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"OK\"}");
  }else if(mode == 10){
    String tmp = listDirectory('/');
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\""+tmp+"\"}");
  }else if(mode == 11){
    deleteAllFiles(name);
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"DONE!\"}");
  }else if(mode == 100){
    String tmp = retBytes("/");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"{\"total\":\"16777216\",\"used\":\""+tmp+"\"}/\"}");
  }
}

void setup() {
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW); 

  IPAddress apIP(44,0,0,4);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));

  Serial.begin(115200);
  delay(100);

  WiFi.softAP(ssid, password);

  Serial.println("\n");
  Serial.println("Creating Access Point...");
  Serial.println("");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/api",HTTP_POST, handleRootPost);
  // server.on("/blink", HTTP_GET, handleBlinkGet);
  server.on("/blink", HTTP_POST, handleBlinkPost);
  server.begin();

  if (!SD.begin(chipSelect)) {
      Serial.println("Initialization failed!");
      blinkLED(50 , 100);
      return;
    }
  Serial.println("Initialization done.");
}

void loop() {
  uint8_t connected_clients = WiFi.softAPgetStationNum();
  server.handleClient();

  if(connected_clients > 0) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }
}