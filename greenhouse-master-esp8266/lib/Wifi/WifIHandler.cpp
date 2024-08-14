#include <WiFiHandler.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>

#define DEBUG_SERIAL Serial
#define DEBUG_ENABLED false

#define DEBUG_PRINT(...) \
    if (DEBUG_ENABLED)   \
    DEBUG_SERIAL.printf(__VA_ARGS__)
#define DEBUG_PRINTLN(...) \
    if (DEBUG_ENABLED)     \
    DEBUG_SERIAL.println(__VA_ARGS__)

WifiHandler::WifiHandler() {}

void WifiHandler::setupWiFi()
{
    AsyncWebServer server(80);
    DNSServer dns;

    AsyncWiFiManager wifiManager(&server, &dns);

    wifiManager.setTimeout(180);
    if (!wifiManager.autoConnect())
    {
        DEBUG_PRINTLN("Failed to connect and hit timeout!");
        delay(3000);
        ESP.reset();
        delay(5000);
    }
    DEBUG_PRINTLN("Connected...");

    WiFi.onStationModeDisconnected([](const WiFiEventStationModeDisconnected &event) {
        DEBUG_PRINTLN("Disconnected from SSID: " + event.ssid + ", reason: " + String(event.reason));
        WiFi.reconnect();
    });
}