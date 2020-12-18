#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
Adafruit_BME280 bme2; // I2C 2
/* Pins */
const int irPin = 12; // This is 'GPIO12' (D6 on NodeMCU)

/* Network settings */
const char* ssid = "SSID"; 
const char* password = "pass"; 
const char* device_name = "led"; 
IPAddress ip(192,168,43,72);  
IPAddress gateway(192,168,1,1); // Gatway
IPAddress subnet(255,255,255,0); // Network mask
unsigned long delayTime;

/* Objects */
MDNSResponder mdns;
ESP8266WebServer server(80);
IRsend irsend(irPin);

/* Website
  
 You can use following structure to put website into Arduino code:
    String webPage = R"=====(content of web page )=====";
 and you don't have to care about: ' " # < > / \  
 
 So below is Website code
  */

String webPage = R"=====(
  <!DOCTYPE HTML>
  <html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>LED CONTROLLER</title>
    <link href="https://fonts.googleapis.com/css?family=Lato:400,900&subset=latin-ext" rel="stylesheet">
    <link href="https://fonts.googleapis.com/icon?family=Material+Icons" rel="stylesheet">
    <style> 
      button {
      float: left;
      width: 60px;
      height: 60px;
      font-size: 15px;
      margin: 5px;
      border-radius: 30px;
      -webkit-transition-duration: 0.4s; /* Safari */
      transition-duration: 0.2s;
      cursor: pointer;
      text-align: center;
      font-family: 'Lato', sans-serif;
      font-weight: bold;
      outline:none;
      border: none;
      color:white;  }

    #container {
      max-width: 280px;
      margin: auto;
      margin-top: 50px;
      background-color: #BDB7AA;
      border: 2px solid #AAB0BD;
      border-radius: 15px;
      padding: 5px;
      padding-top: 20px;
      padding-bottom: 20px;
      margin-top: 20px; }

    .power {
      background-color: white;
      color: black; }
    .power:hover {
      border: 2px solid black;
      color: black; }

    #off {
      background-color: black;
      color: white; }
    #off:hover {
      background-color: white;
      border: 2px solid black;
      color: black; }

    #on {
      background-color: red;  
      color: white; }
    #on:hover {
      background-color: white;
      border: 2px solid red;
      color: black; }

    #red {  background-color: #FF191B;  }
    #red:hover {
      background-color: white;
      border: 2px solid #FF191B;
      color: black; }

    #red1 { background-color: #FF5319;  }
    #red1:hover {
      background-color: white;
      border: 2px solid #FF5319;  }

    #red2 { background-color: #FF6A5E;  }
    #red2:hover {
      background-color: white;
      border: 2px solid #FF6A5E;  }

    #red3 { background-color: #FF926B;  }
    #red3:hover {
      background-color: white;
      border: 2px solid #FF926B;  }

    #red4 { background-color: #F3F62F;  }
    #red4:hover {
      background-color: white;
      border: 2px solid #F3F62F;  }

    #green {  background-color: #288F00;  }
    #green:hover {
      background-color: white;
      border: 2px solid #288F00;
      color: black; }

    #green1 { background-color: #3CD45F;  }
    #green1:hover {
      background-color: white;
      border: 2px solid #3CD45F;
      color: black;
      }
    #green2 { background-color: #47E3C6;  }
    #green2:hover {
      background-color: white;
      border: 2px solid #47E3C6;
      color: black; }

    #green3 { background-color: #44C5DE;  }
    #green3:hover {
      background-color: white;
      border: 2px solid #44C5DE;  }

    #green4 { background-color: #289AE3;  }
    #green4:hover {
      background-color: white;
      border: 2px solid #289AE3;
      color: black; }

    #blue { background-color: #122B9C;  }
    #blue:hover {
      background-color: white;
      border: 2px solid #122B9C;
      color: black; }

    #blue1 {  background-color: #6C6CD4;  }
    #blue1:hover {
      background-color: white;
      border: 2px solid #6C6CD4;
      color: black; }

    #blue2 {  background-color: #5D2AE3;  }
    #blue2:hover {
      background-color: white;
      border: 2px solid #5D2AE3;
      color: black; }

    #blue3 {  background-color: #9025DE;  }
    #blue3:hover {
      background-color: white;
      border: 2px solid #9025DE;
      color: black; }

    #blue4 {  background-color: #BE11E3;  }
    #blue4:hover {
      background-color: white;
      border: 2px solid #BE11E3;
      color: black; }

    #white {
      background-color: white;
      color: black; }
    #white:hover {
      background-color: white;
      border: 2px solid black;
      color: black; }

    .function {
      background-color: #5A6170;
      font-size: 10px;  }
    .function:hover {
      background-color: white;
      border: 2px solid #5A6170;
      color: black;
      font-size: 10px;  }

  </style>
  </head>
  <body>
    <div id="container">
      <a href="lightup"><button class="power"><i class="material-icons">brightness_5</i></button></a>
      <a href="lightdown"><button class="power"><i class="material-icons">brightness_3</i></button></a>
      <a href="off"><button id="off"><i class="material-icons">lightbulb_outline</i></button></a>
      <a href="on"><button id="on"><i class="material-icons">wb_incandescent</i></button></a>
      <div style="clear : both"></div>

      <a href="red"><button id="red">R</button></a>
      <a href="green"><button id="green">G</button></a>
      <a href="blue"><button id="blue">B</button></a>
      <a href="white"><button id="white">W</button></a>
      <div style="clear : both"></div>

      <a href="red1"><button id="red1"></button></a>
      <a href="green1"><button id="green1"></button></a>
      <a href="blue1"><button id="blue1"></button></a>
      <a href="flash"><button class="function">FLASH</button></a>
      <div style="clear : both"></div>

      <a href="red2"><button id="red2"></button></a>
      <a href="green2"><button id="green2"></button></a>
      <a href="blue2"><button id="blue2"></button></a>
      <a href="strobe"><button class="function">STROBE</button></a>
      <div style="clear : both"></div>

      <a href="red3"><button id="red3"></button></a>
      <a href="green3"><button id="green3"></button></a>
      <a href="blue3"><button id="blue3"></button></a>
      <a href="fade"><button class="function">FADE</button></a>
      <div style="clear : both"></div>

      <a href="red4"><button id="red4"></button></a>
      <a href="green4"><button id="green4"></button></a>
      <a href="blue4"><button id="blue4"></button></a>
      <a href="smooth"><button class="function">SMOOTH</button></a>
      <div style="clear : both"></div>
      
      <a href="homfugg"><button class="function">Hom.Fugg</button></a>
      <div style="clear : both"></div>
    </div>
  </body>
  </html>
  )=====";


/* IR Codes*/
#define  IR_BPLUS  0xF700FF
#define IR_BMINUS 0xF7807F
#define IR_OFF    0xF740BF
#define IR_ON     0xF7C03F
#define IR_R    0xF720DF
#define IR_B    0xF7A05F
#define IR_G    0xF7609F
#define IR_W    0xF7E01F
#define IR_B1   0xF710EF
#define IR_B2   0xF7906F
#define IR_B3   0xF750AF
#define IR_FLASH  0xF7D02F
#define IR_B4   0xF730CF
#define IR_B5   0xF7B04F
#define IR_B6   0xF7708F
#define IR_STROBE 0xF7F00F
#define IR_B7   0xF708F7
#define IR_B8   0xF78877
#define IR_B9   0xF748B7
#define IR_FADE   0xF7C837
#define IR_B10    0xF728D7
#define IR_B11    0xF7A857
#define IR_B12    0xF76897
#define IR_SMOOTH 0xF7E817

char homfugg = 0;
void setup(void) {

  delay(1000);

  /* Begin some (un)important things */
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  irsend.begin();
  Serial.println("");
    /* Wait for WiFi connection */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
    unsigned status;
    unsigned status2;

    // default settings
    status = bme.begin();  
    status2 = bme2.begin();  
  /* Show some important information on Serial Monitor */
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

   /* Give name for the device */
  if (mdns.begin(device_name, WiFi.localIP())) {
    Serial.println("MDNS responder started");
  }

  /* Show website (in browser), than send IR code */
  server.on("/", []() {
    server.send(200, "text/html", webPage);
    Serial.println("Loaded main page");
    irsend.sendNEC(IR_ON, 32);
    
    Serial.print("Pressed: ");
    Serial.println("on");
  });
  server.on("/lightup", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_BPLUS, 32);
    irsend.sendNEC(IR_BPLUS, 32);
    homfugg = 0;
    irsend.sendNEC(IR_BPLUS, 32);
    Serial.print("Pressed: ");
    Serial.println("lightup");
    delay(1000);
  });
  server.on("/lightdown", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_BMINUS, 32);
    irsend.sendNEC(IR_BMINUS, 32);
    irsend.sendNEC(IR_BMINUS, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("lightdown");
    delay(1000);
  });
  server.on("/off", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_OFF, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("off");
    delay(1000);
  });
  server.on("/on", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_ON, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("on");
    delay(1000);
  });

  server.on("/red", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_R, 32);
    Serial.print("Pressed: ");
    Serial.println("red");
    homfugg = 0;
    delay(1000);
  });
  server.on("/green", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_G, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("green");
    delay(1000);
  });
  server.on("/blue", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("blue");
    delay(1000);
  });
  server.on("/white", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_W, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("white");
    delay(1000);
  });

  server.on("/red1", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B1, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("red1");
    delay(1000);
  });
  server.on("/green1", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B3, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("green1");
    delay(1000);
  });
  server.on("/blue1", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B2, 32);
    Serial.print("Pressed: ");
    Serial.println("blue1");
    delay(1000);
  });
  server.on("/flash", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_FLASH, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("flash");
    delay(1000);
  });

  server.on("/red2", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B4, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("red2");
    delay(1000);
  });
  server.on("/green2", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B6, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("green2");
    delay(1000);
  });
  server.on("/blue2", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B5, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("blue2");
    delay(1000);
  });
  server.on("/strobe", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_STROBE, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("strobe");
    delay(1000);
  });

  server.on("/red3", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B7, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("red3");
    delay(1000);
  });
  server.on("/green3", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B8, 32);
    Serial.print("Pressed: ");
    homfugg = 0;
    Serial.println("green3");
    delay(1000);
  });
  server.on("/blue3", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B8, 32);
    Serial.print("Pressed: ");
    Serial.println("blue3");
    delay(1000);
  });
  server.on("/fade", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_FADE, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("fade");
    delay(1000);
  });

  server.on("/red4", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B10, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("red4");
    delay(1000);
  });
  server.on("/green4", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B12, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("green4");
    delay(1000);
  });
  server.on("/blue4", []() {
    server.send(200, "text/html", webPage);
    irsend.sendNEC(IR_B11, 32);
    homfugg = 0;
    Serial.print("Pressed: ");
    Serial.println("blue4");
    delay(1000);
  });
  server.on("/smooth", []() {
    server.send(200, "text/html", webPage);
    homfugg = 0;
    irsend.sendNEC(IR_SMOOTH, 32);
    Serial.print("Pressed: ");
    Serial.println("smooth");
    delay(1000);
  });
  server.on("/homfugg", []() {
    server.send(200, "text/html", webPage);
    homfugg = 1;
    Serial.print("Pressed: ");
    Serial.println("Szunfuggo bekapcs");
    delay(1000);
  });

  server.begin();
  Serial.println("HTTP server started");
}


void printValues() {
    Serial.print("Temperature1 = ");
    Serial.print(bme.readTemperature());
    Serial.println(" *C");
    Serial.print("Temperature2 = ");
    Serial.print(bme2.readTemperature());
    Serial.println(" *C");

    Serial.print("Pressure1 = ");
    Serial.print(bme.readPressure() / 100.0F);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude1 = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    Serial.print("Pressure2 = ");
    Serial.print(bme2.readPressure() / 100.0F);
    Serial.println(" hPa");
    Serial.print("Approx. Altitude2 = ");
    Serial.print(bme2.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.println(" m");

    String pres1=String(bme.readPressure()/100.0F);  //10 et hozza kell majd adni

    String pres2=String(bme2.readPressure()/100.0F);

    Serial.print("Humidity1 = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");

    Serial.print("Humidity2 = ");
    Serial.print(bme2.readHumidity());
    Serial.println(" %");
    String hum1=String(bme.readHumidity());
    String hum2=String(bme2.readHumidity());

    Serial.println();

}

void colorTemp(){
  if(bme.readTemperature()<20){
   
  } else if (bme.readTemperature() >20.0 && bme.readTemperature() <21.0){
    Serial.println("20.5");
    irsend.sendNEC(IR_B, 32);

  }else if (bme.readTemperature() >21.0 && bme.readTemperature() <22.0 ){
    irsend.sendNEC(IR_B, 32);
        Serial.println("21.5");


  }else if (bme.readTemperature() >22.0 && bme.readTemperature() <23.0){
    irsend.sendNEC(IR_B5, 32);
    Serial.println("22.5");

  }else if (bme.readTemperature() >23.0 && bme.readTemperature() <24.0){
    irsend.sendNEC(IR_B8, 32);
    Serial.println("23.5");

  }else if (bme.readTemperature() >24.0 && bme.readTemperature() <25.0){
    irsend.sendNEC(IR_B11, 32);
    Serial.println("24.5");

  }else if (bme.readTemperature() >25.0 && bme.readTemperature() <26.0){
    irsend.sendNEC(IR_B7, 32);
    Serial.println("25.5");

  }else if (bme.readTemperature() >26.0 && bme.readTemperature() <27.0){
    irsend.sendNEC(IR_B4, 32);//4
    Serial.println("26.5");

  }else if (bme.readTemperature() >27.0 && bme.readTemperature() <28.0){
    irsend.sendNEC(IR_B1, 32);
    Serial.println("27.5");

  }
  else if (bme.readTemperature() >28.0 && bme.readTemperature() <29.0){
    irsend.sendNEC(IR_R, 32);
        Serial.println("28.5");

  }
  
}

void loop(void) {
  if(homfugg==1){
    Serial.println("adott hom alapjan adok inputot");
    colorTemp();
    printValues();
    delay(5000);
  }
  server.handleClient();
}
