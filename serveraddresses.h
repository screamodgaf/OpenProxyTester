#ifndef SERVERADDRESSES_H
#define SERVERADDRESSES_H

#include <iostream>

class ServerAddresses
{
public:
    ServerAddresses();
    static std::string getServerAddress();
    static void setServerAddress(std::string a);

    static int getServerPort();
    static int setServerPort(int p);
private:
    static std::string serverAddress;
    static int port;

};

#endif // SERVERADDRESSES_H
