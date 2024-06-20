#include <Pins.h>
#include <SoundPlayer.h>
#include <Buttons.h>
#include <LCDModule.h>
#include <RTCModule.h>
#include <DHTModule.h>
#include <Pump.h>
#include <Protocol.h>
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define BAUDRATE 9600
#define CONTROLS_UPDATE_RATE_MS 10
#define MAIN_LCD_REFRESH_RATE_MS 250
#define PUMP_LCD_REFRESH_RATE_MS 1000
#define DHT_REFRESH_RATE_MS 10 * 1000
#define COMMUNICATION_REPEAT_RATE_MS 1000

TaskHandle_t TaskHandle_Controls;
TaskHandle_t TaskHandle_LCD;
TaskHandle_t TaskHandle_DHT;
TaskHandle_t TaskHandle_Pump;
TaskHandle_t TaskHandle_Pump_LCD;
TaskHandle_t TaskHandle_Connection;
TaskHandle_t TaskHandle_Communication;

SemaphoreHandle_t xLCDMutex;

void Task_UpdateControls(void *pvParameters) {
    for (;;) {
        updateControls();
        vTaskDelay(pdMS_TO_TICKS(CONTROLS_UPDATE_RATE_MS));
    }
}

void Task_WriteDateAndTimeToLCD(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xLCDMutex, portMAX_DELAY) == pdTRUE) {
            writeDateAndTimeToLCD();
            xSemaphoreGive(xLCDMutex);
        } else {
            // Handle mutex acquisition failure if needed
        }
        vTaskDelay(pdMS_TO_TICKS(MAIN_LCD_REFRESH_RATE_MS));
    }
}

void Task_WriteDHTReadingsToLCD(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xLCDMutex, portMAX_DELAY) == pdTRUE) {
            writeDHTReadingsToLCD();
            xSemaphoreGive(xLCDMutex);
        } else {
            // Handle mutex acquisition failure if needed
        }
        vTaskDelay(pdMS_TO_TICKS(DHT_REFRESH_RATE_MS));
    }
}

void Task_WritePumpStatusToLCD(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xLCDMutex, portMAX_DELAY) == pdTRUE) {
            writePumpStatusToLCD();
            xSemaphoreGive(xLCDMutex);
        } else {
            // Handle mutex acquisition failure if needed
        }
        vTaskDelay(pdMS_TO_TICKS(MAIN_LCD_REFRESH_RATE_MS));
    }
}

void Task_WriteConnectionStatusToLCD(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xLCDMutex, portMAX_DELAY) == pdTRUE) {
            writeConnectionStatusToLCD();
            xSemaphoreGive(xLCDMutex);
        } else {
            // Handle mutex acquisition failure if needed
        }
        vTaskDelay(pdMS_TO_TICKS(PUMP_LCD_REFRESH_RATE_MS));
    }
}

void Task_UpdatePump(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xLCDMutex, portMAX_DELAY) == pdTRUE) {
            updatePump();
            xSemaphoreGive(xLCDMutex);
        } else {
            // Handle mutex acquisition failure if needed
        }
        vTaskDelay(pdMS_TO_TICKS(MAIN_LCD_REFRESH_RATE_MS));
    }
}

void Task_LoopCommunicationProtocol(void *pvParameters) {
    for (;;) {
        if (xSemaphoreTake(xLCDMutex, portMAX_DELAY) == pdTRUE) {
            loopCommunicationProtocol();
            xSemaphoreGive(xLCDMutex);
        } else {
            // Handle mutex acquisition failure if needed
        }
        vTaskDelay(pdMS_TO_TICKS(COMMUNICATION_REPEAT_RATE_MS));
    }
}

void setup ()
{
    Serial.begin(BAUDRATE);

    initializeSpeaker(SPEAKER);
    initializeRTC();
    initializeDHT();
    initializeLCD();
    initializePump();
    initializeButtons();
    initializeCommunicationProtocol();

    xLCDMutex = xSemaphoreCreateMutex();
    if (xLCDMutex == NULL) {
        Serial.println("Error: Failed to create mutex!");
        while (1);
    }

    xTaskCreate(Task_UpdateControls, "UpdateButtons", 400, NULL, 1, &TaskHandle_Controls);
    xTaskCreate(Task_UpdatePump, "UpdatePump", 400, NULL, 1, &TaskHandle_Pump);
    xTaskCreate(Task_WriteDateAndTimeToLCD, "UpdateLCD", 500, NULL, 1, &TaskHandle_LCD);
    xTaskCreate(Task_WriteDHTReadingsToLCD, "UpdateDHT", 500, NULL, 1, &TaskHandle_DHT);
    xTaskCreate(Task_WritePumpStatusToLCD, "UpdatePumpLCD", 500, NULL, 1, &TaskHandle_Pump_LCD);
    xTaskCreate(Task_WriteConnectionStatusToLCD, "UpdateConnection", 500, NULL, 1, &TaskHandle_Connection);
    xTaskCreate(Task_LoopCommunicationProtocol, "UpdateCommunication", 500, NULL, 3, &TaskHandle_Communication);

    // playTheLick();
}

void loop() {}