#ifndef CREDENTIALS_DATABASE_H
#define CREDENTIALS_DATABASE_H

#include <Arduino.h>

struct Credentials {
    String login;
    String password;
};

class CredentialsDatabase {
public:
    CredentialsDatabase();
    
    void add_credentials(String login, String password);
    struct Credentials* credentials();
    int size();

private:
    struct Credentials m_credentials[255];
    int m_index;
};

#endif