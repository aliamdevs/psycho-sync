#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SPI.h>
#include <SD.h>

// Local Vars :
const int chipSelect = D8; 
String GG = "" ;
ESP8266WebServer server(80);  

/*
    Functions' Headers Details Code :
*/

String macToString(const unsigned char* mac);
void blinkLED(int N , int T);
void WriteInFile(String name,String text,String type);
String ReadFromFile(String name);
String listDirectoryTree(String dirname, int depth = 0);
void deleteAllFiles(String dirname);
String retBytes(String dirname);
void serveFile(String path, String contentType);
void helpSender();
void simpleProgramSD();

void handleRootPost(){
  String nam = server.arg("name");
  String con = server.arg("content");
  String mde = server.arg("mode");
  String typ = server.arg("type");
  int mode = mde.toInt();
  if(mode == 0){
  String tmp;
    tmp = ReadFromFile(nam);
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\""+tmp+"\"}");
  }else if(mode == 1){
    WriteInFile(nam , con , typ);
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"OK\"}");
  }else if(mode == 2){
    String tmp = listDirectoryTree("/");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\""+tmp+"\"}");
  }else if(mode == 3){
    deleteAllFiles("/dir/");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"DONE!\"}");
  }else if(mode == 4){
    String tmp = retBytes("/dir/");
    server.send(200, "application/json","{\"code\":\"200\",\"data\":\"{\"total\":\"16777216\",\"used\":\""+tmp+"\"}/\"}");
  }
}

/*
    Setup Function Details Code :
*/
void treeHandle(){
  server.send(200,"text/plain",listDirectoryTree("/"));
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); 
  if (!SD.begin(chipSelect)) {
      while(true){
        blinkLED(1,30);
      }
  } 
  IPAddress apIP(44,0,0,4);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("psycho-sync", "12345678");
  Serial.println(WiFi.softAPIP());
/*
    Server Routing Handler :
*/
  server.on("/", HTTP_GET, []() { serveFile("/root/index.html", "text/html"); });
  server.on("/style.css", HTTP_GET, []() { serveFile("/root/style.css", "text/css"); });
  server.on("/script.js", HTTP_GET, []() { serveFile("/root/script.js", "application/javascript");}); 
  server.on("/sd/template/simple", HTTP_GET, simpleProgramSD );
  server.on("/tree", HTTP_GET, treeHandle );
  server.on("/help", HTTP_GET, helpSender );
  server.on("/api",HTTP_POST, handleRootPost);

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
void WriteInFile(String name,String text,String type){
  File file;
  file = SD.open("/"+name+"."+type, FILE_WRITE);
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
  myFile = SD.open("/"+name+".psy");
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
String listDirectoryTree(String dirname , int depth) {
  String tree = "";
  String indent = "";
  
  // Create indentation based on depth (for tree structure)
  for (int i = 0; i < depth; i++) {
    indent += "  |-- ";
  }

  File root = SD.open(dirname);
  if (!root) {
    tree += indent + "[ERROR] Failed to open: " + dirname + "\n";
    return tree;
  }
  if (!root.isDirectory()) {
    tree += indent + dirname + " -> ( File , " + String(root.size()) + " bytes)\n";
    root.close();
    return tree;
  }

  tree += indent + dirname + "/ -> (D)\n";

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      // Recursively list subdirectories
      tree += listDirectoryTree(file.name(), depth + 1);
    } else {
      tree += indent + "  |-- " + String(file.name());
      tree += " -> ( File , " + String(file.size()) + " bytes)\n";
    }
    file = root.openNextFile();
  }

  root.close();
  return tree;
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

void serveFile(String path, String contentType) {
  if (SD.exists(path)) {
    File file = SD.open(path, FILE_READ);
    server.streamFile(file, contentType);
    file.close();
  } else {
    server.send(404, "text/plain", "File not found");
  }
}

void helpSender(){
  server.send(200, "text/plain","HELLO WORLD");
}
void simpleProgramSD(){

// --------------------------------- just for sd setup once program then comment it --------------------------------------
        deleteAllFiles("/");
        // Create directories
        SD.mkdir("root");
        SD.mkdir("dir");
        SD.mkdir("settings");
        // Create index.html in root directory
        File indexFile = SD.open("root/index.html", FILE_WRITE);
        if (indexFile) {
          indexFile.println("<!DOCTYPE html>");
          indexFile.println("<html>");
          indexFile.println("<head>");
          indexFile.println("<title>Arduino SD Card</title>");
          indexFile.println("<link rel=\"stylesheet\" href=\"style.css\">");
          indexFile.println("<script src=\"script.js\"></script>");
          indexFile.println("</head>");
          indexFile.println("<body>");
          indexFile.println("<h1>Welcome to Arduino Web Server</h1>");
          indexFile.println("<p>This page is stored on the SD card</p>");
          indexFile.println("</body>");
          indexFile.println("</html>");
          indexFile.close();
          Serial.println("index.html created");
        } else {
          Serial.println("Error creating index.html");
        }
        // Create style.css in root directory
        File cssFile = SD.open("root/style.css", FILE_WRITE);
        if (cssFile) {
          cssFile.println("body {");
          cssFile.println("  font-family: Arial, sans-serif;");
          cssFile.println("  max-width: 800px;");
          cssFile.println("  margin: 0 auto;");
          cssFile.println("  padding: 20px;");
          cssFile.println("  background-color: #f5f5f5;");
          cssFile.println("}");
          cssFile.println("h1 { color: #0066cc; }");
          cssFile.close();
          Serial.println("style.css created");
        } else {
          Serial.println("Error creating style.css");
        }
        // Create script.js in root directory
        File jsFile = SD.open("root/script.js", FILE_WRITE);
        if (jsFile) {
          jsFile.println("console.log('Hello from Arduino SD card!');");
          jsFile.println("document.addEventListener('DOMContentLoaded', function() {");
          jsFile.println("  // Your JavaScript code here");
          jsFile.println("});");
          jsFile.close();
          Serial.println("script.js created");
        } else {
          Serial.println("Error creating script.js");
        }
        // Create settings.json in settings directory
        File settingsFile = SD.open("settings/settings.json", FILE_WRITE);
        if (settingsFile) {
          settingsFile.println("{");
          settingsFile.println("  \"code\": 0");
          settingsFile.println("}");
          settingsFile.close();
          Serial.println("settings.json created");
        } else {
          Serial.println("Error creating settings.json");
        }
// --------------------------------- just for sd setup once program then comment it --------------------------------------

  server.send(200, "text/plain","SD Card Seted and Programed Simple Template");
}