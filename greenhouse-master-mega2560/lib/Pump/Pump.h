#pragma once

void initializePump();
void updatePump();
void turnPumpOn();
void turnPumpOff();
void turnAutoPumpOn();
void turnAutoPumpOff();
bool isPumping();
bool isAutoPumping();
int getSecondsRemaingingAutoPumpTime();
uint32_t getAutoPumpStartTime();
uint32_t getAutoPumpEndTime();