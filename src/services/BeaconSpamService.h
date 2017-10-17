#ifndef BEACON_SPAM_H
#define BEACON_SPAM_H

#include <ESP8266WiFi.h>

#include "Service.h"
#include "utils.h"

enum class BeaconSpamType {
    Spam,
    List
};

class BeaconSpamService : public Service {
public:
    BeaconSpamService();

    void sendBeacon(char* ssid);
    void sendFuzzedBeacon(char* ssid, int n);

    bool init();
    void update();

private:
    BeaconSpamType m_type;

    char m_spamBeacon[32];
    char **m_listBeacons;
    int m_nBeacons;
};

#endif