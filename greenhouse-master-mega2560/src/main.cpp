#include <TaskManager.h>
#include <Pins.h>
#include <Curtains.h>
#include <Pump.h>
#include <Valve.h>
#include <DHTModule.h>
#include <LCDModule.h>
#include <RTCModule.h>
#include <Protocol.h>
#include <ManualMode.h>

#define BAUDRATE 115200

TaskManager* taskManager;
Curtains* curtains;
Pump* pump;
Valve* valve;
DHTModule* dhtModule;
LCDModule* lcdModule;
RTCModule* rtcModule;
Protocol* protocol;
ManualMode* manualMode;

void setup ()
{
  Serial.begin(BAUDRATE);
  
  taskManager = &TaskManager::getInstance();
  curtains = &Curtains::getInstance();
  pump = &Pump::getInstance();
  valve = &Valve::getInstance();
  dhtModule = &DHTModule::getInstance();
  lcdModule = &LCDModule::getInstance();
  rtcModule = &RTCModule::getInstance();
  protocol = &Protocol::getInstance();
  manualMode = &ManualMode::getInstance();
}

void loop() {
  taskManager->execute();
}