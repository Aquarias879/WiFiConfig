#include <WiFiConfig.h>

//define lib
WiFiConfig config;

void setup() {
  Serial.begin(9600);
  while (WiFi.status() != WL_CONNECTED && attempts < 50 )
  {
    config.autoconnect();
    Serial.println("Connecting to WiFi...");
    delay(1000);
    attempts++;
  }
  config.ConnectMode();
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
