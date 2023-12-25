#include <WiFiConfig.h>

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
  

}

void loop() {


}
