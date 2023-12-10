#ifndef SOCKET_H
#define SOCKET_H
#include <QDebug>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <cstring>
#include <QtNetwork/QNetworkProxy>
#include <QEventLoop>
#include "windows.h"
#include <QMutex>
class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(QString nk, QString prx,
                    QString prt, QObject *parent = nullptr);


    void ping(QString data);

signals:

public slots:
    void runCon();
    void connected();
    void disconnected();
    void readyReady();
    void setSocksProxy();
    void proxySocks();
    void proxyVPN();
private:
    QTcpSocket *mSocket;
    QString address;
    qint16 port;
    std::string room;
    std::string room_;
    std::string nickFinal;

    QString Port_final;
    QString Ip_final;

    int numberAttempts;
    QString testPrxyAdrs;
    qint16 testPrxyPort;


};

#endif // SOCKET_H
