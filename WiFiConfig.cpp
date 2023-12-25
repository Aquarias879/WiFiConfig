// WiFiConfig.cpp
#include "WiFiConfig.h"

WiFiConfig::WiFiConfig() : server(80) {
    wifi_config_html = R"(
    <!DOCTYPE html>
    <html>
    <head>
    <title>Device Wi-Fi Config</title>
    <style>
        body {
        font-family: Arial, sans-serif;
        text-align: left;
        margin: 50px 50px 50px 0; /* Adjust the right margin to move the form to the left */
        background-color: #f4f4f4;
        }
        
        h2 {
        color: #333;
        text-align: center; /* Center the text */
        }
        
        form {
        background-color: #fff;
        padding: 20px;
        border-radius: 10px;
        box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
        max-width: 300px;
        margin: auto;
        }
        
        label {
        display: block;
        margin-bottom: 8px;
        color: #555;
        }
        
        input {
        width: 100%;
        padding: 8px;
        margin-bottom: 16px;
        box-sizing: border-box;
        border: 1px solid #ccc;
        border-radius: 4px;
        }
        
        input[type="submit"] {
        background-color: #4caf50;
        color: white;
        padding: 10px 15px;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        }
        
        input[type="submit"]:hover {
        background-color: #45a049;
        }
    </style>
    </head>
    <body>
    <h2>Device Wi-Fi Configuration</h2>
    <form action="/config" method="post">
        <label for="ssid">SSID:</label>
        <input type="text" id="ssid" name="ssid" required><br><br>
        <label for="password">Password:</label>
        <input type="password" id="password" name="password" required><br><br>
        <input type="submit" value="Submit">
    </form>
    </body>
    </html>
)";

}


void WiFiConfig::saveCredentials(const String& ssid, const String& password) {
    EEPROM.begin(64);
    for (int i = 0; i < ssid.length(); ++i) {
        EEPROM.write(ssidAddress + i, ssid[i]);
    }
    EEPROM.write(ssidAddress + ssid.length(), '\0');

    for (int i = 0; i < password.length(); ++i) {
        EEPROM.write(passwordAddress + i, password[i]);
    }
    EEPROM.write(passwordAddress + password.length(), '\0');

    EEPROM.commit();
    EEPROM.end();
}

void WiFiConfig::loadCredentials(String& ssid, String& password) {
    EEPROM.begin(64);
    ssid = "";
    password = "";

    for (int i = 0; i < 32; ++i) {
        char c = EEPROM.read(ssidAddress + i);
        if (c == '\0') {
            break;
        }
        ssid += c;
    }

    for (int i = 0; i < 64; ++i) {
        char c = EEPROM.read(passwordAddress + i);
        if (c == '\0') {
            break;
        }
        password += c;
    }

    EEPROM.end();
}

void WiFiConfig::handleRoot(AsyncWebServerRequest* request) {
    request->send(200, "text/html", wifi_config_html);
}

void WiFiConfig::handleWifiConfig(AsyncWebServerRequest* request) {
    String ssid = request->arg("ssid").c_str();
    String password = request->arg("password").c_str();

    saveCredentials(ssid, password);

    WiFi.begin(ssid.c_str(), password.c_str());

    int maxAttempts = 30;  // Set the maximum number of connection attempts
    for (int attempts = 0; attempts < maxAttempts && WiFi.status() != WL_CONNECTED; attempts++) {
        delay(1000);
        Serial.println("Connecting to " + ssid);
    }

    if (WiFi.status() == WL_CONNECTED){
        Serial.println("Connected to WiFi");
        Serial.println("IP address: " + WiFi.localIP().toString());
    }
}

/*void WiFiConfig::autoconnect() {
    int attempts = 0;
    String savedSSID, savedPassword;
    loadCredentials(savedSSID, savedPassword);
    Serial.println(savedSSID);
    //Serial.println(savedPassword);
    WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
    Serial.print("Connecting to WiFi");
    int maxAttempts = 50;  // Set the maximum number of connection attempts
    for (int attempts = 0; attempts < maxAttempts && WiFi.status() != WL_CONNECTED; attempts++) {
        delay(1000);
        Serial.print(".");
    }
}*/

void WiFiConfig::autoconnect() {
    String savedSSID, savedPassword;
    loadCredentials(savedSSID, savedPassword);

    Serial.println("SSID: " + savedSSID);

    WiFi.begin(savedSSID.c_str(), savedPassword.c_str());
    Serial.print("Connecting to WiFi");

    const int maxAttempts = 50;  // Set the maximum number of connection attempts
    for (int attempts = 0; attempts < maxAttempts && WiFi.status() != WL_CONNECTED; ++attempts) {
        delay(1000);
        Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConnected to WiFi");
        Serial.println("IP address: " + WiFi.localIP().toString());
    } else {
        Serial.println("\nNot Found Saved SSID");
    }
}


void WiFiConfig::ConnectMode() {
    if (WiFi.status() != WL_CONNECTED){
        WiFi.softAP(PARAM_SSID, PARAM_PASSWORD);

        for (; WiFi.softAPgetStationNum() == 0; delay(1000)) {
            Serial.println("Waiting for a client to connect...");
        }

        server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
            this->handleRoot(request);
        });

        server.on("/config", HTTP_POST, [this](AsyncWebServerRequest* request) {
            this->handleWifiConfig(request);
        });

        server.begin();
        Serial.println("SoftAP IP address: " + WiFi.softAPIP().toString());
        while (true) {
            if (WiFi.status() == WL_CONNECTED) {
                Serial.println("Connected to WiFi");
                Serial.println("IP address: " + WiFi.localIP().toString());
                break;
            }
    }
  }
}
