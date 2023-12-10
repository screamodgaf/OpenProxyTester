#ifndef SOCKET_H
#define SOCKET_H
#include <QDebug>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QAbstractSocket>
#include <cstring>
#include <QtNetwork/QNetworkProxy>

class Socket : public QObject
{
    Q_OBJECT
public:
    explicit Socket(std::string nk, QString prx,
                    int prt, QObject *parent = nullptr);

    void runCon();
    void ping(QString data);
signals:

public slots:
    void connected();
    void disconnected();
    void readyReady();
    void proxyHttp();
    void proxySocks();
    void proxyVPN();
private:
    QTcpSocket *mSocket;
    QString address;
    qint16 port;
    std::string room;
    std::string nick;

    QString ipVPN;
    int portVPN;

    QString testPrxyAdrs;
    qint16 testPrxyPort;

};

#endif // SOCKET_H
