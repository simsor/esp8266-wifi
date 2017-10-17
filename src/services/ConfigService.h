#ifndef CONFIG_SERVICE_H
#define CONFIG_SERVICE_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <ArduinoJson.h>
#include "DNSServer.h"

#include "Service.h"
#include "utils.h"

class ConfigService : public Service {
public:
    ConfigService();

    void _createPortalsList();

    bool init();
    void update();

private:
    AsyncWebServer m_server;
    DNSServer m_dns;

    char m_portalsList[512];
};

#endif