#include <QCoreApplication>
#include "socket.h"
#include <QThread>
#include "tester.h"
#include "getdata.h"
#include "goodproxystorage.h"
#include <QDebug>
#include <iostream>
#include "serveraddresses.h"
#include <atomic>

int GoodProxyStorage::IPcounter = -1;
int GoodProxyStorage::NickCounter = 0;
std::vector<QString> GoodProxyStorage::IPs;
std::vector<QString> GoodProxyStorage::Ports;
std::vector<QString> GoodProxyStorage::nickDatabase;
std::vector<QString> Tester::goodProxiesIP; // in Tester

std::string ServerAddresses::serverAddress;
int ServerAddresses::port;


int GetData::ipAmount=0;

std::atomic<int> remainingProxyCounter = 0;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    std::cout << "Enter IP of the server, you want to test proxy list against (ex. irc.xxxxxx.pl)";
    std::string address;
    std::getline(std::cin,  address);
    ServerAddresses::setServerAddress(address);

    std::cout << "Enter port of the server (ex. 6667)";
    int port;
    std::string portString;
    std::getline(std::cin, portString);
    port = std::stoi(portString);
    ServerAddresses::setServerPort(port);


    GetData data;
    data.nicksGet();
    int numberOfProxies = data.ircProxiesGet();
    std::cout << "data received - number of proxies to test: " << numberOfProxies << "\n";

    remainingProxyCounter = numberOfProxies;

    int nrThreads =30 ;
    std::vector<std::string> ip1= data.ircProxies;
    std::vector<std::string> prt1 = data.ircProxiesPort;
    std::cout << ip1.size();
    std::cout<< "aaip1: " << ip1[1] <<std::endl;
    std::cout<< "aaprt1: " << prt1[1] <<std::endl;

    QString threadName = "thread";
    QString objectName = "object";
    qDebug() << threadName;
    qDebug() << objectName;
//thread 1-------------------------:


    for(int i=0; i< nrThreads; i++)
    {

        threadName+=QString::number(i);
        objectName+=QString::number(i);


        QThread *threadName = new QThread;
        Tester *objectName = new Tester(
                    *threadName,
                          data.nicks[i],
                          ip1,
                          prt1, remainingProxyCounter);


        objectName->moveToThread(threadName);
        threadName->start();




     }

return a.exec();
}


