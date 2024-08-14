#include <ESP8266WiFi.h>
#include <WifiHandler.h>
#include <WebSocketHandler.h>
#include <TaskManager.h>
#include <Protocol.h>

#define BAUD_RATE 115200

#define DEBUG_SERIAL Serial
#define DEBUG_ENABLED false

#define DEBUG_PRINT(...) \
  if (DEBUG_ENABLED)     \
  DEBUG_SERIAL.printf(__VA_ARGS__)
#define DEBUG_PRINTLN(...) \
  if (DEBUG_ENABLED)       \
  DEBUG_SERIAL.println(__VA_ARGS__)

WifiHandler wifiHandler;
TaskManager *taskManager;
WebSocketHandler *webSocketHandler;
Protocol *protocol;

void setup()
{
  DEBUG_SERIAL.begin(BAUD_RATE);

  //  DEBUG_SERIAL.setDebugOutput(true);

  DEBUG_PRINTLN();
  DEBUG_PRINTLN();
  DEBUG_PRINTLN();

  for (uint8_t t = 4; t > 0; t--)
  {
    DEBUG_PRINT("[SETUP] BOOT WAIT %d...\n", t);
    DEBUG_SERIAL.flush();
    delay(1000);
  }

  taskManager = &TaskManager::getInstance();
  protocol = &Protocol::getInstance();

  wifiHandler.setupWiFi();

  webSocketHandler = &WebSocketHandler::getInstance();
}

void loop()
{
  taskManager->execute();
}