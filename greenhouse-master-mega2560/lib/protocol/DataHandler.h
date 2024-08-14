#pragma once

#include <Arduino.h>

void handleIncomingData(char* data);

long getWifiSignalStrength();

bool isSocketConnected();

uint32_t getAutoPumpTimeMinutes();