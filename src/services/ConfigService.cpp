#include "ConfigService.h"

ConfigService::ConfigService() : m_server(80) {}

bool ConfigService::init()
{
    if (!WiFi.softAP("iPhone de Marge", "conserve")) {
        Serial.println("Failed to create AP iPhone de Marge :(");
        return false;
    }

    Serial.println("Created AP \"iPhone de Marge\"");

    m_dns.start(53, "*", IPAddress(192, 168, 4, 1));

    _createPortalsList();

    m_server.serveStatic("/i", SPIFFS, "/i");

    m_server.on("/", [] (AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/c/index.html", "text/html");
    });

    m_server.onNotFound([] (AsyncWebServerRequest *request) {
        request->redirect("http://192.168.4.1/");
    });

    m_server.on("/get/portals", [this] (AsyncWebServerRequest *request) {
        request->send(200, "text/json", this->m_portalsList);
    });

    m_server.on("/get/portal", [] (AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/portal.txt", "text/plain");
    });

    m_server.on("/get/beaconspam", [] (AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/beacon.txt", "text/plain");
    });

    m_server.on("/set/portal", [] (AsyncWebServerRequest *request) {
        if (request->hasParam("portal", true)) {
            writeToSpiffsFile("/portal.txt", request->getParam("portal", true)->value().c_str());
            request->redirect("/#Portal changed");
        } else {
            request->send(400, "text/plain", "You forgot the portal name");
        }
    });

    m_server.on("/state", [this] (AsyncWebServerRequest *request) {
        char *beaconConfig, *portalConfig;
        readSpiffsFile("/portal.txt", &portalConfig);
        readSpiffsFile("/beacon.txt", &beaconConfig);

        DynamicJsonBuffer buffer;
        JsonObject &obj = buffer.createObject();

        obj["portal"] = String(portalConfig);
        obj["beacon"] = String(beaconConfig);

        free(beaconConfig);
        free(portalConfig);

        obj["portals"] = String(m_portalsList);

        char ret[512];
        obj.printTo(ret, sizeof(ret));

        request->send(200, "text/json", ret);
    });

    m_server.on("/set/beaconspam/spam", [] (AsyncWebServerRequest *request) {
        if (request->hasParam("ssid", true)) {
            String fileContents = "spam;" + request->getParam("ssid", true)->value();
            writeToSpiffsFile("/beacon.txt", fileContents.c_str());
            request->redirect("/#Beacon spam changed");
        } else {
            request->send(400, "text/plain", "you forgot ssid");
        }
    });

    m_server.on("/set/beaconspam/list", [] (AsyncWebServerRequest *request) {
        if (request->hasParam("list", true)) {
            String list = request->getParam("list", true)->value();
            list.replace("\n", ";");
            list.replace("\r", "");
            String fileContents = "list;" + list;
            writeToSpiffsFile("/beacon.txt", fileContents.c_str());
            request->redirect("/#Beacon list changed");
        } else {
            request->send(400, "text/plain", "you forgot list");
        }
    });

    m_server.on("/set/service", [] (AsyncWebServerRequest *request) {
        if (request->hasParam("service", true)) {
            setNextService(request->getParam("service", true)->value().c_str());
            request->send(200);

            delay(1000);
            ESP.restart();
        } else {
            request->send(400, "text/plain", "you forgot something");
        }
    });

    m_server.begin();

    return true;
}

void ConfigService::update()
{
    m_dns.processNextRequest();
}

void ConfigService::_createPortalsList()
{
    Dir dir = SPIFFS.openDir("/p/");
    DynamicJsonBuffer buffer;

    JsonArray &array = buffer.createArray();

    while (dir.next()) {
        if (!dir.fileName().endsWith("/c.txt")) {
            continue;
        }
        
        char *configFile;
        char ssid[32], folderName[16];
        
        JsonObject &obj = array.createNestedObject();

        readSpiffsFile(dir.fileName().c_str(), &configFile);

        int folder_i;
        for (int i=3, folder_i=0; i < dir.fileName().length() - 6; i++, folder_i++) {
            folderName[folder_i] = dir.fileName().charAt(i);
        }

        strcpy(ssid, configFile);

        trim(ssid);

        obj.set("ssid", String(ssid));
        obj.set("folder", String(folderName));

        free(configFile);

        Serial.printf("Read %s (%s)\n", folderName, ssid);
    }

    array.printTo(m_portalsList, sizeof(m_portalsList));
    Serial.println(m_portalsList);
}