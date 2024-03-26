  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <WiFiClient.h>
  #include <FastLED.h>
  #include <DNSServer.h>
  #include <WiFiManager.h> 
 
  
  WiFiManager wifiManager;  
   
  #define LED_PIN 2
  #define NUM_LEDS 60
  #define BRIGHTNESS 50
  #define LED_TYPE WS2812B
  #define COLOR_ORDER GRB

  CRGB leds[NUM_LEDS];
  bool isLedStripOn = true; 
  String lastMode = "off"; 
  bool isRainbowModeOn = false; 
  const int flashButtonPin = 0;
  

  ESP8266WebServer server(80); 
  
  void setSolidColor(uint8_t r, uint8_t g, uint8_t b) {
  for(int i=0; i<NUM_LEDS; i++) {
  leds[i] = CRGB(r, g, b);
  }
  FastLED.show();
  }
  
void rainbow() {
  static long firstPixelHue = 0;
  
  for(int i = 0; i < NUM_LEDS; i++) {
    int pixelHue = firstPixelHue + (i * 65536L / NUM_LEDS);
    leds[i] = CHSV(pixelHue >> 8, 255, 255);
  }
  FastLED.show();
  firstPixelHue += 256; 
  if(firstPixelHue >= 5*65536) {
    firstPixelHue = 0; 
  }
}

  
  void handleRoot() {
    String page = F("<!DOCTYPE html><html>"
                    "<head><title>ESP8266 LED Control</title>"
                    "<style>"
                    "body { font-family: Arial; text-align: center; margin: 0 auto; padding-top: 50px; }"
                    ".button { display: inline-block; background-color: #008CBA; color: white; padding: 15px 25px; "
                    "text-align: center; text-decoration: none; font-size: 24px; margin: 4px 2px; cursor: pointer; }"
                    "</style>"
                    "</head>"
                    "<body>"
                    "<h1>ESP8266 LED Control</h1>"
                    "<button class='button' onclick='sendCommand(\"/red\")'>Red</button>"
                    "<button class='button' onclick='sendCommand(\"/green\")'>Green</button>"
                    "<button class='button' onclick='sendCommand(\"/blue\")'>Blue</button>"
                    "<button class='button' onclick='sendCommand(\"/rainbow\")'>Rainbow</button>"
                    "<button class='button' onclick='sendCommand(\"/on\")'>Turn ON</button>"
                    "<button class='button' onclick='sendCommand(\"/off\")'>Turn OFF</button>"
                    "<script>"
                    "var isLedOn = false;"
                    "function sendCommand(command) {"
                    "  if (!isLedOn && command !== '/on') {"
                    "    alert('Please turn on the LED strip first.');"
                    "    return;"
                    "  }"
                    "  var xhttp = new XMLHttpRequest();"
                    "  xhttp.onreadystatechange = function() {"
                    "    if (this.readyState == 4 && this.status == 200) {"
                    "      if (command === '/on') isLedOn = true;"
                    "      if (command === '/off') isLedOn = false;"
                    "      console.log(this.responseText);"
                    "    }"
                    "  };"
                    "  xhttp.open('GET', command, true);"
                    "  xhttp.send();"
                    "}"
                    "</script>"
                    "</body></html>");
  
    server.send(200, "text/html", page);
  }
  
  
  
  void handleRed() {
  if (isLedStripOn) {
  isRainbowModeOn = false;
  setSolidColor(255, 0, 0); 
  server.send(200, "text/plain", "Red");
  lastMode = "red";
  } else {
  server.send(200, "text/plain", "Please turn the LED strip on first");
  }
   }
  
  void handleGreen() {
  if (isLedStripOn) {
     isRainbowModeOn = false;
  setSolidColor(0, 255, 0);
  server.send(200, "text/plain", "Green");
  lastMode = "green";
  } else {
  server.send(200, "text/plain", "Please turn the LED strip on first");
   }
  }
  
  void handleBlue() {
  if (isLedStripOn) {
    isRainbowModeOn = false;
  setSolidColor(0, 0, 255);
  server.send(200, "text/plain", "Blue");
  lastMode = "blue";
  } else {
  server.send(200, "text/plain", "Please turn the LED strip on first");
  }
  }
  
 void handleRainbow() {
  if (isLedStripOn) {
    isRainbowModeOn = true; 
    server.send(200, "text/plain", "Rainbow mode is ON");
    lastMode = "rainbow";
  } else {
    server.send(200, "text/plain", "Please turn the LED strip on first");
  }
}
  void turnLedsOn() {
  isLedStripOn = true;
  }
  
  void turnLedsOff() {
  for(int i = 0; i < NUM_LEDS; i++) {
  leds[i] = CRGB::Black;
  }
  FastLED.show();
  isLedStripOn = false;
  isRainbowModeOn = false;
  }

  void handleTurnOn() {
  if (!isLedStripOn) {
    isLedStripOn = true;
    activateLastMode();
    server.send(200, "text/plain", (lastMode != "off") ? (lastMode + " mode is ON") : "LED Strip is ON");
  } else {
    server.send(200, "text/plain", "LED Strip is already ON");
  }
}

  void activateLastMode() {
  
  if (lastMode == "rainbow") {
    isRainbowModeOn = true;  
    rainbow(); 
  } else {
    isRainbowModeOn = false; 
    if (lastMode == "red") setSolidColor(255, 0, 0);
    else if (lastMode == "green") setSolidColor(0, 255, 0);
    else if (lastMode == "blue") setSolidColor(0, 0, 255);
  }
}

  

  
  void handleTurnOff() {
  turnLedsOff();
  server.send(200, "text/plain", "LED Strip is OFF");
  lastMode = "off";
  }
  
  void setup() {
  
    
  Serial.begin(115200);

  IPAddress _ip = IPAddress(192,168,0,101);
  IPAddress _gw = IPAddress(192,168,0,1);
  IPAddress _sn = IPAddress(225,225,225,0);

  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
  
  Serial.println();
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

   pinMode(flashButtonPin, INPUT);
  
  
   // wifiManager.resetSettings();

  
  wifiManager.setConfigPortalTimeout(60);

  if(!wifiManager.autoConnect("ESP8266")) {
    Serial.println("Не удалось подключиться к Wi-Fi и время ожидания истекло. Перезагрузка...");
    delay(2000); 
    ESP.restart(); 
  }
    
  Serial.println("Подключились к сети:)");
  
  Serial.println("");
  Serial.println("WiFi подключен");
  Serial.print("IP-адрес: ");
  Serial.println(WiFi.localIP());

  
  server.on("/", handleRoot);
  server.on("/red", handleRed);
  server.on("/green", handleGreen);
  server.on("/blue", handleBlue);
  server.on("/rainbow", handleRainbow);
  server.on("/on", handleTurnOn);
  server.on("/off", handleTurnOff);
  server.begin();

  isLedStripOn = false;
  lastMode = "off";
  turnLedsOff();
  }

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Вошли в режим настройки");
 
}
  
  void loop() {
    
    if (digitalRead(flashButtonPin) == LOW) {
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    ESP.reset();
    Serial.println("WiFi settings reset");
    delay(1000); 
  }

    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("Потеряно подключение к Wi-Fi сети, переключение в режим AP...");
        
     bool res = wifiManager.startConfigPortal("ESP8266");
        if (!res) {
            Serial.println("Не удалось подключиться к Wi-Fi сети и не выполнено ручное подключение в течение времени ожидания.");
            
            ESP.restart();
        } Serial.println("Успешно подключились к Wi-Fi сети после ручной настройки!");
    }
 

  server.handleClient();
  if (!isLedStripOn) {
    return;
  }

  if (isRainbowModeOn) {
    rainbow(); 
    delay(1); 
  }
}
