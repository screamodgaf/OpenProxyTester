#include "goodproxystorage.h"
#include "socket.h"
#include "windows.h"
#include "getdata.h"
#include <vector>
#include <QCoreApplication>
#include <QDir>

GoodProxyStorage::GoodProxyStorage(QObject *parent) : QObject(parent)
{
    QDir directory  = QCoreApplication::applicationDirPath();
    relativePathToWorkingProxies = directory.absoluteFilePath("working_proxies.txt");



    mutex = new QMutex;


}

void GoodProxyStorage::storeReady(QString QproxyIP, QString QproxyPort)
{

    proxReady = QproxyIP;
    portReady = QproxyPort;
    qDebug() << "1 ^^^^^^^^^proxReady" << proxReady;
    qDebug() << "1 ^^^^^^^^^portReady" << portReady;



    if(std::find(IPs.begin(), IPs.end(), proxReady) == IPs.end())
    {
        IPs.push_back(proxReady); //proxies for vector
        Ports.push_back(portReady); //proxies for vector

        saveToFile(proxReady, portReady);  //proxies for file

        IPcounter++;
        // for startThreadforConnection
        QString ObjNr = QString::number(IPcounter);
        QString ObjName = "socket" + ObjNr;
        QString ThreadName = "thread" + ObjNr;

        // for startThreadforConnection2
        QString ObjName2 = "socket_2" + ObjNr;
        QString ThreadName2  = "thread_2" + ObjNr;

    //startThreadforConnection(proxReady, portReady);@@@
    }
}



void GoodProxyStorage::saveToFile(QString proxReady, QString portReady)
{
    std::ofstream file(relativePathToWorkingProxies.toStdString(), std::ios_base::app);

    if (!file.is_open()) {
        std::cout << "working_proxies file is NOT open" << "\n";
        qDebug() << "couldnt open the Http_proxies file";
        return;
    }

    std::cout << relativePathToWorkingProxies.toStdString() << " is open\n";

    std::cout << "%proxy being enterd into working proxies file: " << proxReady.toStdString() << ":" << portReady.toStdString() << "\n";
    file << proxReady.toStdString() << ":" << portReady.toStdString() << "\n";
    file.close();
}

void GoodProxyStorage::startThreadforConnection(QString proxReady, QString portReady)
{

    mNick = data.nicks[NickCounter];
    NickCounter++;

    qDebug() << "2 ************proxReady" << proxReady;
    qDebug() << "2 ************portReady" << portReady;


    QmNick = QString::fromStdString(mNick);


    QThread *ThreadName = new QThread;
    Socket ObjName(QmNick, proxReady, portReady);
    ObjName.moveToThread(ThreadName);
    ThreadName->start();
}

void GoodProxyStorage::startThreadforConnection2(QString proxReady, QString portReady)
{
    mNick = data.nicks[NickCounter];
    NickCounter++;

    qDebug() << "startThreadforConnection2 mNick::::>>> " << QmNick << " <<<<<<:::::";
    qDebug() << "startThreadforConnection2 proxReady, portReady>>> " << proxReady << ":" << portReady;
    QThread *ThreadName2 = new QThread;
    Socket ObjName2(QmNick, proxReady, portReady);
    ObjName2.moveToThread(ThreadName2);
    proxReady.clear();
    portReady.clear();
    ThreadName2->start();
}
