#ifndef PORTAL_SERVICE_H
#define PORTAL_SERVICE_H

#include <ESPAsyncWebServer.h>
#include <Arduino.h>
#include <FS.h>
#include "DNSServer.h"

#include "CredentialsDatabase.h"
#include "Service.h"
#include "utils.h"

class PortalService : public Service {
public:
    PortalService(char *portalFolder, CredentialsDatabase *database);

    bool init();
    void update();

private:
    AsyncWebServer m_server;
    DNSServer m_dns;
    CredentialsDatabase *m_database;

    char m_folderPath[32];
    char m_ssid[32];
    char m_indexPath[32];
};

#endif