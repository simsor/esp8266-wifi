#include <Arduino.h>
#include <FS.h>

#include "CredentialsDatabase.h"
#include "services/Service.h"

#include "services/ConfigService.h"
#include "services/PortalService.h"
#include "services/BeaconSpamService.h"


CredentialsDatabase database;
Service *currentService;
bool initOk = false;

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    Serial.println("\nStarting up!");

    if(!SPIFFS.begin()) {
        Serial.println("Failed mounting the filesystem");
        return;
    }

    // First we check which service to start
    char *service;
    readSpiffsFile("/service.txt", &service);
    trim(service);

    Serial.printf("Service: %s\n", service);

    // Then we reset the service to "config"
    setNextService("config");

    // Finally we launch the service
    delay(1000);
    if (strcmp(service, "portal") == 0) {
        char *portalName;
        readSpiffsFile("/portal.txt", &portalName);
        trim(portalName);
        currentService = new PortalService(portalName, &database);
        free(portalName);
    } else if (strcmp(service, "config") == 0) {
        currentService = new ConfigService();
    } else if (strcmp(service, "beaconspam") == 0) {
        currentService = new BeaconSpamService();
    } else {
        Serial.printf("Unknown service: %s\n", service);
        return;
    }

    initOk = currentService->init();
    
    if (!initOk) {
        Serial.println("An error occured running the service");
        return;
    }

    digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
    if (initOk) {
        currentService->update();
    }    
}