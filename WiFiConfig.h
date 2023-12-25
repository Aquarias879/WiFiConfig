// WiFiConfig.h
#ifndef WIFICONFIG_H
#define WIFICONFIG_H

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>

class WiFiConfig {
public:
	WiFiConfig();
	void autoconnect();
	void ConnectMode();
	
private:
	void saveCredentials(const String& ssid, const String& password);
	void loadCredentials(String& ssid, String& password);
	void handleRoot(AsyncWebServerRequest* request);
	void handleWifiConfig(AsyncWebServerRequest* request);
	void handleWifiScan(AsyncWebServerRequest* request);
	
	const char* PARAM_SSID = "ESP32-Config";
	const char* PARAM_PASSWORD = "12345678";
	const int ssidAddress = 0;
	const int passwordAddress = 32;
	const char* wifi_config_html;
	AsyncWebServer server;
};
#endif  // WIFICONFIG_H
