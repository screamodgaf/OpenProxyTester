#ifndef GETDATA_H
#define GETDATA_H
#include <vector>
#include <QObject>
#include <fstream>
#include <QDebug>
#include <iostream>

class Goodproxylist;
class GetData
{


public:
    explicit GetData( );
    void nicksGet();
    int ircProxiesGet();

      //variable - should be setter getter
signals:

public slots:
protected:
public://--------------
    std::vector<std::string> ircProxies;
    std::vector<std::string> ircProxiesPort;
    std::vector<std::string> nicks;
    static int ipAmount;
    int ircProxiesSize;

public:


    friend class Tester;
    friend class GoodProxyStorage;
};

#endif // GETDATA_H
