#include <ESP8266WebServer.h>
#include <Servo.h>
#include <FS.h>

ESP8266WebServer server(80);
Servo servo;

void handleRoot() {
  if(SPIFFS.exists("/index.html"))
  {
    File file = SPIFFS.open("/index.html","r");
    server.streamFile(file, "text/html");
  } else {
    server.send(404,"File not found" , "text/html");
  }
}

void fahre(){
  server.send(200, "text/plain", "Fahrbefehl erkannt! ("+server.arg("geschwindigkeit")+")");
  int geschwindigkeit = server.arg("geschwindigkeit").toInt();
  if (geschwindigkeit < 0){
    digitalWrite(D6,LOW);
    digitalWrite(D7,HIGH);
  }else if(geschwindigkeit > 0){
    digitalWrite(D6,HIGH);
    digitalWrite(D7,LOW);
  }else{
    digitalWrite(D6,LOW);
    digitalWrite(D7,LOW);
  }
  analogWrite(D8,abs(constrain(geschwindigkeit, -1024, 1024)));

  int grad = server.arg("grad").toInt();
    servo.write(constrain(grad, 50, 130));


  Serial.print("grad = ");
  Serial.println(grad);
  Serial.print("geschwindigkeit = ");
  Serial.println(geschwindigkeit);
  Serial.println();
}

void setup(void){
  Serial.begin(115200);
  WiFi.softAP("Wemos", "passwort");
  
  SPIFFS.begin();
  server.on("/", handleRoot);
  server.on("/fahre", fahre);
  server.begin();
  
  servo.attach(D1);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  
}

void loop(void){
  server.handleClient();
}

