#include "serveraddresses.h"

ServerAddresses::ServerAddresses()
{

}

std::string ServerAddresses::getServerAddress()
{
    return serverAddress;
}

void ServerAddresses::setServerAddress(std::string a)
{
    serverAddress = a;
}


int ServerAddresses::getServerPort()
{
    return port;
}

int ServerAddresses::setServerPort(int p)
{
    std::cout << "ServerAddresses::setServerPort(int p)" << "\n";
    port = p;
}


static std::string serverAddress;
static int port;
