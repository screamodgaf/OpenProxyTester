#ifndef TESTER_H
#define TESTER_H

#include <QObject>
#include <QMutex>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <cstring>
#include <QtNetwork/QNetworkProxy>
#include "getdata.h"
#include <windows.h>
#include <QTimer>
#include <vector>
#include <QCoreApplication>
#include <QThread>
#include "goodproxystorage.h"
#include <algorithm>

class Tester : public QObject

{
    Q_OBJECT
public:
    explicit Tester(QThread &mThread,
                    std::string nick_,
                    std::vector<std::string> ircProxies,
                    std::vector<std::string> ircProxiesPort,
                    std::atomic<int>& remainingProxyCounter,
                    QObject *parent = nullptr);

    void ping(QString data);


signals:
    void proxyReady(QString QproxyIP,
                    QString QproxyPort);
    void createStableConnection();
    void disconMe();
    void connectME();
    //PingMe(QString data);
public slots:
    void runCon();
    void proxyList();

    void connected();
    void disconnected();
    void readyReady();
    void proxyHttp();


    void pingPre();


 private:
    QString address;
    qint16 port;

    std::string proxyIP;
    QString QproxyIP;
    QString QproxyPort;
    std::string nick;
    std::string proxyPort;
    std::string room;
    QTcpSocket *testSocket;
    GetData Gdata;

    QTimer *timer;


    bool pongReceived;
    bool endOfList;

    QString data;

    int pingCounter;

    std::vector<std::string> proxyIPvec;
    std::vector<std::string> proxyPortVec;
    static std::vector<QString> goodProxiesIP;
    static int ipAmountLeft;
    static int proxyVecIndex;
    GoodProxyStorage storage; //object for class

    std::atomic<int>& m_remainingProxyCounter;
};

#endif // TESTER_H
