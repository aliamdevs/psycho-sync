#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

// Local Vars :
const char* ssid = "psycho-sync";
const char* password = "33003400"; 
const int chipSelect = D8; 
String GG = "" ;

ESP8266WebServer server(80);  

/*
    Functions' Headers Details Code :
*/

String macToString(const unsigned char* mac);
void blinkLED(int N , int T);
void WriteInFile(String name,String text);
String ReadFromFile(String name);
String listDirectory(String dirname);
void deleteAllFiles(String dirname);
String retBytes(String dirname);

void handleRoot(){
 String html = "<html><body>
    <h1>D1 Mini LED Control</h1>
    <p>AP IP:  + WiFi.softAPIP().toString() + </p>
    <p>Text To Show:  + "+String(GG)+" + </p>
    <form action='/blink' method='post'>
      Num Of Blink: <input type='text' name='num'><br>
      <input type='submit' value='Blink LED (2)'>
    </form>
    <h2>Upload Text</h2>
    <form action='/' method='post'>
      Name: <input type='text' name='name'><br>
      Context: <input type='text' name='context' value= ><br>
      <input type='text' name='mode' hidden value='1'><br>
      <input type='submit' value='Submit'>
    </form>
    <h2>Download Text</h2>
    <form action='/' method='post'>
      Name: <input type='text' name='name'><br>
      <input type='text' name='context' hidden value= ><br>
      <input type='text' name='mode' hidden value='0'><br>
      <input type='submit' value='Submit'>
    </form>
  </body></html>";
  
  server.send(200, "text/html", html);
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
    String tmp = listDirectory("/dir/");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\""+tmp+"\"}");
  }else if(mode == 11){
    deleteAllFiles("/dir/");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"DONE!\"}");
  }else if(mode == 100){
    String tmp = retBytes("/dir/");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"{\"total\":\"16777216\",\"used\":\""+tmp+"\"}/\"}");
  }
}

/*
    Setup Function Details Code :
*/

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  if (!SD.begin(chipSelect)) {
      while(true){
        blinkLED(1,30);
      }
  } 
  IPAddress apIP(3,3,0,0);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(ssid, password);
  Serial.println(WiFi.softAPIP());
/*
    Server Routing Handler :
*/
  server.on("/", handleRoot);
  server.on("/3430",HTTP_POST, handleRootPost);

  server.begin();
}

/*
    Loop Function Details Code :
*/

void loop() {
  uint8_t connected_clients = WiFi.softAPgetStationNum();
  server.handleClient();

  if(connected_clients > 0) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    blinkLED(1,1000);
  }
}

/*
    Local Functions Details Code :
*/

String macToString(const unsigned char* mac) {
  char buf[20];
  snprintf(buf, sizeof(buf), "%02X:%02X:%02X:%02X:%02X:%02X",
           mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(buf);
}
void blinkLED(int N , int T) {
  for(int i=0; i<N; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(T);
    digitalWrite(LED_BUILTIN, LOW);
    delay(T);
  }
}
void WriteInFile(String name,String text){
  File file;
  file = SD.open("/dir/"+name+".psy", FILE_WRITE);
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
  myFile = SD.open("/dir/"+name+".psy");
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