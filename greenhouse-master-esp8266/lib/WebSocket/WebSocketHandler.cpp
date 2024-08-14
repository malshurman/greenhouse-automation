#include <ESP8266WiFi.h>
#include <WebSocketHandler.h>
#include <TaskManager.h>
#include <ArduinoQueue.h>

#define DEBUG_SERIAL Serial
#define DEBUG_ENABLED true

#define DEBUG_PRINT(...) \
  if (DEBUG_ENABLED)     \
  DEBUG_SERIAL.printf(__VA_ARGS__)
#define DEBUG_PRINTLN(...) \
  if (DEBUG_ENABLED)       \
  DEBUG_SERIAL.println(__VA_ARGS__)
#define DEBUG_HEXDUMP(payload, length) \
  if (DEBUG_ENABLED)                   \
  hexdump(payload, length)

#define WEBSOCKET_LOOP_INTERVAL 10

#define WEBSOCKET_SERVER_HOST "192.168.0.19"
#define WEBSOCKET_SERVER_PORT 3000

#define MAX_PAYLOAD_SIZE 100

#define NO_OPERATION 'X'

const char ESP_IDENTIFIER = 'E';
const char SERVER_IDENTIFIER = 'S';

static uint32_t autoPumpTimeMinutes = 10;
static ArduinoQueue<char> commandsQueue(MAX_PAYLOAD_SIZE);

void setReceivedData(uint8_t *payload);

static void loopWebSocketTask()
{
  WebSocketHandler::getInstance().loopWebsocket();
}

void WebSocketHandler::loopWebsocket()
{
  webSocket.loop();
}

WebSocketHandler &WebSocketHandler::getInstance()
{
  static WebSocketHandler instance;
  return instance;
}

WebSocketHandler::WebSocketHandler()
{
  connected = false;
  DEBUG_PRINT("Local IP: ");
  DEBUG_PRINTLN(WiFi.localIP().toString().c_str());
  // server address, port and URL
  webSocket.begin(WEBSOCKET_SERVER_HOST, WEBSOCKET_SERVER_PORT, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  TaskManager::getInstance().createTask(WEBSOCKET_LOOP_INTERVAL, loopWebSocketTask);
}

void WebSocketHandler::webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    DEBUG_PRINT("[WSc] Disconnected!\n");
    WebSocketHandler::getInstance().setConnected(false);
    break;
  case WStype_CONNECTED:
    DEBUG_PRINT("[WSc] Connected to url: %s\n", payload);
    WebSocketHandler::getInstance().setConnected(true);
    DEBUG_PRINT("[WSc] SENT: Connected\n");
    WebSocketHandler::getInstance().sendMessageToWebSocket("Connected");
    break;
  case WStype_TEXT:
    DEBUG_PRINT("[WSc] RESPONSE: %s\n", payload);
    setReceivedData(payload);
    break;
  case WStype_BIN:
    DEBUG_PRINT("[WSc] RESPONSE: %s\n", payload);
    setReceivedData(payload);
    break;
  case WStype_PING:
    // pong will be send automatically
    DEBUG_PRINT("[WSc] get ping\n");
    break;
  case WStype_PONG:
    // answer to a ping we send
    DEBUG_PRINT("[WSc] get pong\n");
    break;
  default:
    DEBUG_PRINT("[WSc] Invalid WStype [%d]\n", type);
    break;
  }
}

void WebSocketHandler::sendMessageToWebSocket(const char *message)
{
  size_t messageLength = strlen(message);
  size_t bufferSize = messageLength + 2;
  char buffer[bufferSize];

  buffer[0] = ESP_IDENTIFIER;
  strncpy(buffer + 1, message, messageLength);
  buffer[bufferSize - 1] = '\0';

  webSocket.sendTXT(message);
}

void WebSocketHandler::setConnected(bool connected)
{
  this->connected = connected;
}

bool WebSocketHandler::isConnected()
{
  return connected;
}

int WebSocketHandler::getAutoPumpTimeMinutes()
{
  return autoPumpTimeMinutes;
}

char WebSocketHandler::getNextCommand()
{
  if (!commandsQueue.isEmpty())
  {
    return commandsQueue.dequeue();
  }
  else
  {
    return NO_OPERATION;
  }
}

void setReceivedData(uint8_t *payload)
{
  if (payload[0] == SERVER_IDENTIFIER)
  {
    int newAutoPumpTime;
    char action;

    if (sscanf(reinterpret_cast<const char *>(payload), "S%4d%c", &newAutoPumpTime, &action) == 2)
    {
      autoPumpTimeMinutes = newAutoPumpTime;
      commandsQueue.enqueue(action);
    }
  }
}