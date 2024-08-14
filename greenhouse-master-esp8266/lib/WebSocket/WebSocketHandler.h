#pragma once

#include <WebSocketsClient.h>

class WebSocketHandler
{
public:
    WebSocketHandler(const WebSocketHandler &) = delete;
    WebSocketHandler &operator=(const WebSocketHandler &) = delete;

    static WebSocketHandler &getInstance();

    void loopWebsocket();
    void sendMessageToWebSocket(const char *message);
    bool isConnected();
    void setConnected(bool connected);

    int getAutoPumpTimeMinutes();

    char getNextCommand();

private:
    WebSocketHandler();
    WebSocketsClient webSocket;
    bool connected;

    static void webSocketEvent(WStype_t type, uint8_t *payload, size_t length);
};