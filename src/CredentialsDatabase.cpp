#include "CredentialsDatabase.h"

CredentialsDatabase::CredentialsDatabase() : m_credentials(), m_index(0) {}

void CredentialsDatabase::add_credentials(String login, String password)
{
    Credentials c{login, password};
    m_credentials[m_index] = c;
    m_index++;

    Serial.printf("Got: %s\t%s\n", login.c_str(), password.c_str());
}

struct Credentials* CredentialsDatabase::credentials()
{
    return m_credentials;
}

int CredentialsDatabase::size()
{
    return m_index;
}